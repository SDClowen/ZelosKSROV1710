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

#ifndef __ZELOS_GLOBALD_PACKET_MANAGER_SHARD_HPP__
#define __ZELOS_GLOBALD_PACKET_MANAGER_SHARD_HPP__

#include <stdint.h>

#include <string>
#include <boost/shared_ptr.hpp>

#include "account_property.h"

class OPacket;
class ShardConfig;

namespace srv_pkt
{

void Config (OPacket *pkt, const ShardConfig *config);

void LoginQueue (OPacket *pkt, const uint32_t ID, const std::string &usr, const std::string &pw,
                 const std::string &IP, const AccountProperty &property);

}

#endif //__ZELOS_GLOBALD_PACKET_MANAGER_SHARD_HPP__
