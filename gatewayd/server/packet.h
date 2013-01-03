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

#ifndef __ZELOS_GATEWAYD_SERVER_PACKET_H__
#define __ZELOS_GATEWAYD_SERVER_PACKET_H__

#include <map>

#include <boost/shared_ptr.hpp>

#include "news.h"
#include "shard.h"

class OPacket;

namespace srv_pkt
{
    void WriteNews (OPacket *pkt, const std::map<uint32_t,Notice> &news);

    void WriteShards (OPacket *pkt, const std::string &name, const uint8_t global_version,
                      const std::map<uint16_t,Shard> &servers, const bool state);

    void C5 (OPacket *pkt);
}

#endif // __ZELOS_GATEWAYD_SERVER_PACKET_H__
