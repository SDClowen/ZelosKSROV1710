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

#include "auth/packet_srv_authentication.h"

#include <packet/packet.h>

#include "auth/opcodes.h"

namespace srv_pkt
{

void WriteHandshakeMode (OPacket *pkt, const uint8_t mode)
{
    pkt->WriteOpcode(SERVER_AUTHENTICATION);
    pkt->Write<uint8_t>(mode);
}

void WriteHandshakeEnc (OPacket *pkt, const uint8_t public_key[8])
{
    pkt->Write((const char*)public_key,8);
}

void WriteHandshakeSeeds (OPacket *pkt, const uint32_t count_seed,
    const uint32_t crc_seed)
{
    pkt->Write<uint32_t>(count_seed);
    pkt->Write<uint32_t>(crc_seed);
}

void WriteHandshakeDH (OPacket *pkt, const uint32_t base_key[2], const uint32_t G,
    const uint32_t P, const uint32_t A)
{
    for (int i = 0; i < 2; ++i)
        pkt->Write<uint32_t>(base_key[i]);

    pkt->Write<uint32_t>(G);
    pkt->Write<uint32_t>(P);
    pkt->Write<uint32_t>(A);
}

void WriteHandshakeSignature (OPacket *pkt, const uint32_t signature[2])
{
    pkt->WriteOpcode(SERVER_AUTHENTICATION);
    pkt->Write<uint8_t>(HANDSHAKE_SIGNATURE);
    pkt->Write<uint32_t>(signature[0]);
    pkt->Write<uint32_t>(signature[1]);
}

void WriteVersionA (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint16_t>(0x0101);
    pkt->Write<uint16_t>(0x0500);
    pkt->Write<uint8_t>(0x20);
}

void WriteVersionB (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint16_t>(0x0100);
    pkt->Write<uint16_t>(0x0100);
    pkt->Write<uint16_t>(0x0C69);   //Client Version ??? 1.189 = 0x095C, 1.190 = 0x02CF KSRO_VERSION=0x0C69
    pkt->Write<uint32_t>(0x05);
    pkt->Write<uint8_t>(0x02);
}

void WriteVersionC (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint16_t>(0x0101);
    pkt->Write<uint16_t>(0x0500);
    pkt->Write<uint8_t>(0x60);
}

void WriteVersionD (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint16_t>(0x0300);
    pkt->Write<uint16_t>(0x0200);
    pkt->Write<uint16_t>(0x0200);
}

void WriteVersionE (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint32_t>(0x0101);    ///ISRO AND KSRO <= 654 0x0101, KSRO VERSION 657 = 0x0B01
    pkt->Write<uint8_t>(0xA1);
}

void WriteVersionF (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint16_t>(0x0100);
}

void WriteVersionG (OPacket *pkt)
{
    pkt->WriteOpcode(SERVER_INFORMATION);
    pkt->Write<uint32_t>(0x04000101);
    pkt->Write<uint8_t>(0xA1);
}

}
