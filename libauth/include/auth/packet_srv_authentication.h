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

#ifndef __ZELOS_LIBAUTH_PACKET_AUTHENTICATION_HPP__
#define __ZELOS_LIBAUTH_PACKET_AUTHENTICATION_HPP__

#include <stdint.h>
#include <boost/shared_ptr.hpp>

class OPacket;

namespace srv_pkt
{

enum HANDSHAKE_TYPE
{
    HANDSHAKE_CLEAN = 0x01,
    HANDSHAKE_EXCHANGE = 0x0E,
    HANDSHAKE_SIGNATURE = 0x10
};

/**
 *
 *  @brief: Construct the handshake packet with the desired security mode.
 *
 **/

void WriteHandshakeMode (OPacket *pkt, const uint8_t mode);


/**
 *
 *  @brief: Write Encryption key information into the packet.
 *
 **/

void WriteHandshakeEnc (OPacket *pkt, const uint8_t public_key[8]);

/**
 *
 *  @brief: Write CRC//Security seeds information into the packet.
 *
 **/

void WriteHandshakeSeeds (OPacket *pkt, const uint32_t count_seed,
    const uint32_t crc_seed);

/**
 *
 *  @brief: Write Diffie-Hellman exchange information.
 *
 **/

void WriteHandshakeDH (OPacket *pkt, const uint32_t base_key[2], const uint32_t G,
    const uint32_t P, const uint32_t A);

/**
 *
 *  @brief: Construct the handshake packet with server signature for later client validation.
 *
 **/

void WriteHandshakeSignature (OPacket *pkt, const uint32_t signature[2]);

void WriteVersionA (OPacket *pkt);

void WriteVersionB (OPacket *pkt);

void WriteVersionC (OPacket *pkt);

void WriteVersionD (OPacket *pkt);

void WriteVersionE (OPacket *pkt);

void WriteVersionF (OPacket *pkt);

void WriteVersionG (OPacket *pkt);

}

#endif // __ZELOS_LIBAUTH_PACKET_AUTHENTICATION_HPP__
