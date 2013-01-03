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

#ifndef AUTHENTICATION_ZELOS_H
#define AUTHENTICATION_ZELOS_H

#include "authentication.h"

class Blowfish;

class SROAuth : public Authentication
{
public:

    SROAuth (AUTH_TYPE authType, const uint8_t mode);

    virtual ~SROAuth ();

    void initializeCRC (const uint32_t seed);

    void initializeCount (unsigned long seed);

    uint8_t generateCountByte ();

    void encode (OPacket *packet, bool encrypt, std::vector<uint8_t> &stream);

    DECODE_ERROR readHeader (const uint8_t *buffer, const size_t pendingLen,
                             const size_t maxLen, size_t *buffer_size,
                             size_t *real_size, bool *hasSecurity);

    DECODE_ERROR decode (IPacket *packet, const size_t buffer_size, const size_t real_size,
                         const bool hasSecurity);

    /**
     *
     *  @brief: Initialize blowfish key for decoding and encoding packets.
     *  @param key: Key to initialize with.
     *  @param keybytes: Length of the key.
     *
     **/

    void setKey (uint8_t *key, int keybytes);

    void encrypt (uint8_t *pInput, uint8_t *pOutput, uint32_t lSize);

    void decrypt (uint8_t *pInput, uint8_t *pOutput, uint32_t lSize);

private:

    Blowfish *m_blowfish;

    uint32_t m_crc_seed;
    uint8_t m_count_seeds[3];
};

#endif // AUTHENTICATION_ZELOS_H
