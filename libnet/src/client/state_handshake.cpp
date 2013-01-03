/*********************************************************************************
 *
 * This file is part of ZelosOnline.
 *
 * ZelosOnline is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ZelosOnline is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright  2013  Rafael Dominguez (npcdoom)
 *
 *********************************************************************************/

#include "net/client/state_handshake.h"

#include <auth/authentication_sro.h>
#include <auth/packet_clt_authentication.h>
#include <auth/packet_whois.h>
#include <auth/security.h>
#include <auth/security_modes.h>

#include "net/connection.h"
#include "net/opcode/common_clt.h"

namespace clt
{

IStateHandshake::IStateHandshake (const std::string &srvID, const std::string &cltID,
                                  const boost::shared_ptr<IConnection> &conn)
    : ConnectionState<IConnection>::ConnectionState(conn), m_SrvID(srvID), m_CltID(cltID)
{
    memset(m_base_key,0,KEY_LENGTH);
    memset(m_srv_signature,0,KEY_LENGTH);

    m_opcode_table[CLIENT_HANDSHAKE] = boost::bind(&IStateHandshake::OnHandshake,this,_1);
    m_opcode_table[CLIENT_WHOIS] = boost::bind(&IStateHandshake::OnWhoIs,this,_1);
}

IStateHandshake::~IStateHandshake ()
{
}

void IStateHandshake::Close ()
{
}

int IStateHandshake::OnHandshake (const IPacket &packet)
{
    uint8_t mode = packet.Read<uint8_t>();

    if (mode & SECURITY_DH_AGREEMENT)
    {
        if (mode != SECURITY_DH_AGREEMENT)
            return MSG_ERROR_ARG;

        return OnHandshake_Signature(packet);
    }

    if (mode & SECURITY_NONE)
    {
        if (mode > 1)
            return MSG_ERROR_ARG;

        return OnHandshake_Clean(packet);
    }

    boost::shared_ptr<Authentication> auth = m_connection->getAuthentication();

    assert(auth);

    auth->setMode(mode);

    return OnHandshake_Security(packet);
}

int IStateHandshake::OnHandshake_Clean (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    SendID();

    return MSG_SUCCESS;
}

int IStateHandshake::OnHandshake_Security (const IPacket &packet)
{
    boost::shared_ptr<SROAuth> auth =
            boost::shared_static_cast<SROAuth,Authentication>(m_connection->getAuthentication());

    uint8_t mode = auth->getMode();

    if (mode & SECURITY_ENCRYPTION)
    {
        uint8_t public_key[KEY_LENGTH];

        for (int i = 0; i < KEY_LENGTH; ++i)
            public_key[i] = packet.Read<uint8_t>();

        auth->setKey(public_key,KEY_LENGTH);
    }

    if (mode & SECURITY_CRC)
    {
        uint32_t count = packet.Read<uint32_t>();

        uint32_t crc = packet.Read<uint32_t>();

        auth->initializeCount(count);

        auth->initializeCRC(crc);
    }

    if (mode & SECURITY_DH)
    {
        for (int i = 0; i < KEY_LENGTH; ++i)
            m_base_key[i] = packet.Read<uint8_t>();

        uint32_t G = packet.Read<uint32_t>();

        uint32_t P = packet.Read<uint32_t>();

        uint32_t A = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        uint32_t b = rand();

        uint8_t keyByte = 0;
        uint32_t B = ModExp(P,b,G);
        m_S = ModExp(P,b,A);

        uint8_t priv_key[KEY_LENGTH] = { 0 };
        memcpy(priv_key,&A,sizeof(A));
        memcpy(priv_key+4,&B,sizeof(B));

        keyByte = LOBYTE(LOWORD(m_S)) & 0x03;
        Func_X_2(priv_key, m_S, keyByte);

        auth->setKey(priv_key,KEY_LENGTH);

        /// Generate the private blowfish data
        uint8_t signature[KEY_LENGTH] = { 0 };
        memcpy(signature,&B,sizeof(B));
        memcpy(signature+4,&A,sizeof(A));

        keyByte = LOBYTE(LOWORD(B)) & 0x07;
        Func_X_2(signature, m_S, keyByte);

        auth->encrypt(signature,signature,KEY_LENGTH);

        OPacket pkt;
        clt_pkt::Signature(&pkt,B,signature,KEY_LENGTH);
        m_connection->send(&pkt);

        memcpy(m_srv_signature,&A,sizeof(A));
        memcpy(m_srv_signature+4,&B,sizeof(B));

        keyByte = LOBYTE(LOWORD(A)) & 0x07;
        Func_X_2(m_srv_signature,m_S, keyByte);

        auth->encrypt(m_srv_signature,m_srv_signature,KEY_LENGTH);
    }

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    if (!(mode & SECURITY_DH))
        SendID();

    return MSG_SUCCESS;
}

int IStateHandshake::OnHandshake_Signature (const IPacket &packet)
{
    uint8_t signature[KEY_LENGTH] = { 0 };

    for (int i=0; i < KEY_LENGTH; ++i)
        signature[i] = packet.Read<uint8_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    /// Simple check to see if the handshake is good
    if(memcmp(m_srv_signature,signature,KEY_LENGTH))
    {
#if defined DEBUG
        syslog(LOG_DEBUG,"clt::IStateHandshake::OnHandshake_Signature - Invalid Server Signature.");
        syslog(LOG_DEBUG,"Server Key = %i %i, Client Key = %i %i",m_srv_signature[0],m_srv_signature[1],
            signature[0],signature[1]);
#endif
        return MSG_ERROR_ARG;
    }

    Func_X_2(m_base_key, m_S, 0x3);

    boost::shared_ptr<SROAuth> auth =
            boost::shared_static_cast<SROAuth,Authentication>(m_connection->getAuthentication());

    auth->setKey(m_base_key,KEY_LENGTH);

    SendID();

    return MSG_SUCCESS;
}

int IStateHandshake::OnWhoIs (const IPacket &packet)
{
    std::string srvID = packet.Read();

    uint8_t resv = packet.Read<uint8_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    if (resv || srvID != m_SrvID)
        return MSG_ERROR_ARG;

    OnConfig();

    return MSG_SUCCESS;
}

void IStateHandshake::SendID ()
{
    OPacket pkt;
    clt_pkt::AuthValid(&pkt);
    m_connection->send(&pkt);

    pkt.Clear();

    pkt::Whois(&pkt,m_CltID);
    m_connection->send(&pkt,true);
}

}

