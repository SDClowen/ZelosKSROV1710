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

#include "net/server/connection.h"

#include <auth/authentication_sro.h>
#include <auth/packet_srv_authentication.h>
#include <auth/security.h>
#include <auth/security_modes.h>

#include "net/server.h"
#include "net/server/state_handshake.h"

namespace srv
{
    Connection::Connection (const uint32_t connID, const uint32_t timeout,
                            boost::asio::io_service &io_service,
                            const boost::weak_ptr<IServer> &srv)
        :   IConnection(connID,timeout,2048,io_service), m_server(srv)
    {
    }

    Connection::~Connection ()
    {
    }

    void Connection::OnListen ()
    {
        boost::shared_ptr<SROAuth> auth = boost::shared_static_cast<SROAuth,Authentication>(m_Auth);

        assert(m_Auth);

        uint8_t mode = auth->getMode();

        OPacket pkt;
        srv_pkt::WriteHandshakeMode(&pkt,mode);

        if (mode & SECURITY_ENCRYPTION)
        {
            /// GENERATE A PUBLIC KEY
            uint8_t public_key[8] = { 0 };

            for (int i = 0; i < 8; ++i)
                public_key[i] = rand();

            srv_pkt::WriteHandshakeEnc(&pkt,public_key);
        }

        if (mode & SECURITY_CRC)
        {
            uint32_t count_seed = rand()%255;
            uint32_t crc_seed = rand()%255;

            auth->initializeCount(count_seed);
            auth->initializeCRC(crc_seed);

            srv_pkt::WriteHandshakeSeeds(&pkt,count_seed,crc_seed);
        }

        if (mode & SECURITY_DH)
        {
            /// GENERATE BASE KEY
            uint32_t base_key[2] = { 0 };
            base_key[0] = rand();
            base_key[1] = rand();

            /// GENERATE PUBLIC DATA
            uint32_t a = rand();
            uint32_t G = rand();
            uint32_t P = rand();

            uint32_t A = ModExp(P,a,G);

            boost::shared_ptr<IStateHandshake> st;
            st = boost::shared_static_cast<IStateHandshake,IConnection::State>(m_State);

            assert(st);

            st->SetBaseKey(base_key);
            st->SetSharedData(a,P,G,A);

            srv_pkt::WriteHandshakeDH(&pkt,base_key,G,P,A);
        }

        send(&pkt);
    }

    void Connection::OnCleanup ()
    {
        boost::shared_ptr<IServer> srv = m_server.lock();

        if (srv)
            srv->RemoveConnection(m_ID,m_current_state);
    }

    void Connection::OnError (const boost::system::error_code &error)
    {
    }

}

