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

#include "packet_manager_shard.h"

#include <packet/packet.h>
#include <net/opcode/global_shard_srv.h>

#include "shard_config.h"

namespace srv_pkt
{

void Config (OPacket *pkt, const ShardConfig *config)
{
    pkt->WriteOpcode(SRV_SHARD_CONFIG);
    pkt->Write<uint16_t>(config->ID);
    pkt->Write<uint8_t>(config->locale);
    pkt->Write<uint8_t>(config->races);
    pkt->Write<uint32_t>(config->version);
    pkt->Write(config->filter.first);
    pkt->Write(config->filter.second);

    pkt->Write<uint16_t>(config->world.size());

    for (std::vector<Area>::const_iterator iter = config->world.begin(); iter != config->world.end(); ++iter)
    {
        pkt->Write<uint16_t>(iter->ID);
        pkt->Write<uint8_t>(iter->Instance);
        pkt->Write<uint16_t>(iter->Zones.size());

        for (std::vector<int>::const_iterator ziter = iter->Zones.begin(); ziter != iter->Zones.end(); ++ziter)
            pkt->Write<uint16_t>(*ziter);
    }
}

void LoginQueue (OPacket *pkt, const uint32_t ID, const std::string &usr, const std::string &pw,
                 const std::string &IP,  const AccountProperty &property)
{
    pkt->WriteOpcode(SRV_TICKET_QUEUE);
    pkt->Write<uint32_t>(ID);
    pkt->Write(usr);
    pkt->Write(pw);
    pkt->Write(IP);
    pkt->Write<uint32_t>(property.ID);
    pkt->Write<uint8_t>(property.acc);
    pkt->Write<uint8_t>(property.sec);
    pkt->Write<uint32_t>(property.silk);
}

}
