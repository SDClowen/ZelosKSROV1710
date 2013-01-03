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

#include "server/packet_agent.h"

#include <packet/packet.h>
#include <net/opcode/shard_agent_srv.h>

#include "area.h"
#include "ticket.h"

namespace srv_pkt
{
    void AgentConfig (OPacket *pkt, const uint8_t locale, const uint8_t race, const uint32_t version,
                      const std::string &chFilter, const std::vector<std::string> &strFilter, const uint8_t maxAreas)
    {
        pkt->WriteOpcode(SRV_AGENT_CONFIG);
        pkt->Write<uint8_t>(locale);
        pkt->Write<uint8_t>(race);
        pkt->Write<uint32_t>(version);
        pkt->Write<uint8_t>(maxAreas);
        pkt->Write(chFilter);
        pkt->Write<uint32_t>(strFilter.size());

        for (size_t i = 0; i < strFilter.size(); ++i)
            pkt->Write(strFilter[i]);
    }

    void AgentAction (OPacket *pkt, const uint8_t action)
    {
        pkt->WriteOpcode(SRV_AGENT_ACTION);
        pkt->Write<uint8_t>(action);
    }

    void WorldArea (OPacket *pkt, const Area *area)
    {
        pkt->WriteOpcode(SRV_AGENT_AREA);
        pkt->Write<uint16_t>(area->ID);
        pkt->Write(area->host);
        pkt->Write(area->port);
        pkt->Write<uint16_t>(area->Zones.size());

        for (size_t i = 0; i < area->Zones.size(); ++i)
            pkt->Write<uint16_t>(area->Zones[i]);
    }

    void LoginTicket (OPacket *pkt, Ticket *ticket)
    {
        pkt->WriteOpcode(SRV_LOGIN_TICKET,ANSWER_ACCEPT);
        pkt->Write<uint32_t>(ticket->ID);
        pkt->Write<uint32_t>(ticket->AccountID);
        pkt->Write<uint8_t>(ticket->sec_lv);
    }

    void LoginTicket (OPacket *pkt, const LOGIN_ERROR error, const uint32_t ticketID)
    {
        pkt->WriteOpcode(SRV_LOGIN_TICKET,ANSWER_ERROR,error);
        pkt->Write<uint32_t>(ticketID);
    }
}
