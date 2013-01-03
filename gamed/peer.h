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

#ifndef __ZELOS_GAMED_PEER_H__
#define __ZELOS_GAMED_PEER_H__

#include <stdint.h>
#include <string>

struct Peer
{
    Peer (const uint32_t _id, const std::string &_host, const std::string &_port)
        : ID(_id), host(_host), port(_port), state(0)
    {}

    uint32_t ID;
    std::string host;
    std::string port;
    uint8_t state;
};

#endif // __ZELOS_GAMED_PEER_H__
