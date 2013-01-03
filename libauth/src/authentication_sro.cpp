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

#include "auth/authentication_sro.h"

#include <blowfish.h>
#include <packet/packet.h>
#include <packet/packet_group.h>

#include "auth/security.h"
#include "auth/security_modes.h"

SROAuth::SROAuth (AUTH_TYPE authType, const uint8_t mode)
    : Authentication(authType,mode), m_blowfish(0), m_crc_seed(0)
{
    m_count_seeds[0] = 0;
    m_count_seeds[1] = 0;
    m_count_seeds[2] = 0;
}

SROAuth::~SROAuth ()
{
    if (m_blowfish)
        delete m_blowfish;
}

void SROAuth::initializeCRC (uint32_t seed)
{
    m_crc_seed = seed;
}

void SROAuth::initializeCount (unsigned long seed)
{
    if( seed == 0 )
        seed = 0x9ABFB3B6;

    unsigned long mut = seed, mut1 = 0, mut2 = 0, mut3 = 0;

    mut1 = GenerateValue(mut);
    mut2 = GenerateValue(mut1);
    mut3 = GenerateValue(mut2);
    mut = GenerateValue(mut3);

    unsigned char byte1 = (unsigned char)((mut&0xFF)^(mut3&0xFF));
    unsigned char byte2 = (unsigned char)((mut1&0xFF)^(mut2&0xFF));

    if( !byte1 )
        byte1 = 1;

    if( !byte2 )
        byte2 = 1;

    m_count_seeds[0] = byte1^byte2;
    m_count_seeds[1] = byte2;
    m_count_seeds[2] = byte1;
}

uint8_t SROAuth::generateCountByte ()
{
    unsigned char result = (m_count_seeds[2]*(~m_count_seeds[0]+m_count_seeds[1]));

    result = result ^ ( result >> 4 );
    m_count_seeds[0] = result;

    return result;
}

void SROAuth::encode(OPacket *packet, bool encrypt, std::vector<uint8_t> &stream)
{
    size_t size = packet->size();

    if (m_Type == AUTH_CLIENT && m_Mode & SECURITY_CRC)
    {
        generateCountByte();

        packet->WriteCountSecurity(m_count_seeds[0]);

        packet->WriteCrcSecurity(0);

        packet->WriteCrcSecurity(GenerateCheckByte((const char*)packet->buffer(),size,m_crc_seed));
    }

    if ((m_Mode & SECURITY_ENCRYPTION) && encrypt)
        size = m_blowfish->GetOutputLength(size-2)+2;

    stream.resize(size);

    if ((m_Mode & SECURITY_ENCRYPTION) && encrypt)
    {
        memcpy(stream.data(),packet->buffer(),2);
        m_blowfish->Encode((packet->buffer()+2),stream.data()+2,packet->size()-2);
    }
    else
        memcpy(stream.data(),packet->buffer(),size);
}

DECODE_ERROR SROAuth::readHeader (const uint8_t *buffer, const size_t pendingLen,
                                       const size_t maxLen, size_t *buffer_size,
                                       size_t *real_size, bool *hasSecurity)
{
    PacketHeader *header = (PacketHeader*)(buffer);

    *hasSecurity = header->size & 0x8000;

    if (*hasSecurity)
    {
        *real_size = header->size & 0x7FFF;

        if (m_Mode & SECURITY_ENCRYPTION)
            *buffer_size = m_blowfish->GetOutputLength(*real_size+4)+2;
        else
            *buffer_size = *real_size + 6;
    }
    else
    {
        *real_size = header->size;
        *buffer_size = *real_size + 6;
    }

    if (*buffer_size > maxLen)
        return ERR_DECODE_INVALID;

    /// CHECK IF WE GOT COMPLETE PACKET
    if (pendingLen < *buffer_size)
        return ERR_DECODE_MISSING_DATA;

    /// COPY THE REAL SIZE TO THE BUFFER
    header->size = *real_size;

    return ERR_DECODE_SUCCESS;
}

DECODE_ERROR SROAuth::decode (IPacket *packet, const size_t buffer_size, const size_t real_size,
                                  const bool hasSecurity)
{
    if (hasSecurity && (m_Mode & SECURITY_ENCRYPTION))
    {
        if (m_Mode & SECURITY_NONE)
            return ERR_DECODE_INVALID;

        /// DECODE AND COPY THE PACKET TO THE PARSING BUFFER
        m_blowfish->Decode(packet->buffer()+2,packet->buffer()+2,buffer_size-2);

        packet->resize();
    }

    if (!packet->opcode())
        return ERR_DECODE_INVALID;

    PacketHeader *header = reinterpret_cast<PacketHeader*>(packet->buffer());

    /// CHECK SECURITY BYTES
    if (m_Mode & SECURITY_CRC)
    {
        if (m_Type == AUTH_SERVER)
        {
            generateCountByte();

            if (m_count_seeds[0] != header->securityCount)
                return ERR_DECODE_INVALID;

            uint8_t clt_crc = header->securityCRC;

            /// SET THE ENCRYPTED FLAG SO WE CAN CALCULATE THE PACKET CRC.
            if (hasSecurity)
                header->size = real_size | 0x8000;

            /// SET CRC BYTE 0 TO CALCULATE THE PACKET CRC.
            header->securityCRC = 0;

            uint8_t srv_crc = GenerateCheckByte((const char*)(packet->buffer()),
                                                header->size+6,m_crc_seed);

            /// RESTORE PAYLOAD SIZE
            header->size = real_size;

            if (clt_crc != srv_crc)
                return ERR_DECODE_INVALID;

            header->securityCRC = clt_crc;
        }
        else
        {
            if (header->securityCount || header->securityCRC)
                return ERR_DECODE_INVALID;
        }
    }
    else
    {
        if (header->securityCount || header->securityCRC)
            return ERR_DECODE_INVALID;
    }

    return ERR_DECODE_SUCCESS;
}

void SROAuth::setKey (uint8_t *key, int keybytes)
{
    if (!m_blowfish)
        m_blowfish = new Blowfish();

    m_blowfish->Initialize(key,keybytes);
}

void SROAuth::encrypt (uint8_t *pInput, uint8_t *pOutput, uint32_t lSize)
{
    m_blowfish->Encode(pInput,pOutput,lSize);
}

void SROAuth::decrypt (uint8_t *pInput, uint8_t *pOutput, uint32_t lSize)
{
    m_blowfish->Decode(pInput,pOutput,lSize);
}
