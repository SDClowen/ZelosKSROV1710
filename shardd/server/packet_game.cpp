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

#include "server/packet_game.h"

#include <net/opcode/shard_game_srv.h>
#include <net/opcode/shard_area_srv.h>
#include <packet/packet.h>

#include "area.h"

namespace srv_pkt
{
    void AreaConfig (OPacket *pkt, Area *area)
    {
        pkt->WriteOpcode(SRV_AREA_CONFIG);
        pkt->Write<uint16_t>(area->ID);
        pkt->Write<uint8_t>(area->Instanced);
        pkt->Write<uint16_t>(area->Zones.size());

        for (size_t i = 0; i < area->Zones.size(); ++i)
            pkt->Write<uint16_t>(area->Zones[i]);
    }

    void AreaAction (OPacket *pkt, bool action)
    {
        pkt->WriteOpcode(SRV_AREA_ACTION);
        pkt->Write<uint8_t>(action);
    }

    void AreaSrv (OPacket *pkt, const std::string &host, const std::string &port)
    {
        pkt->WriteOpcode(SRV_AREA_SERVER);
        pkt->Write(host);
        pkt->Write(port);
    }
}
