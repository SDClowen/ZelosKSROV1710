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

#include "shardd/packet_shard.h"

#include <net/opcode/shard_agent_clt.h>
#include <packet/packet.h>

#include "ticket.h"

namespace clt_pkt
{
    void Config (OPacket *pkt, const std::string &host, const std::string &port)
    {
        pkt->WriteOpcode(CLT_AGENT_CONFIG);
        pkt->Write(host);
        pkt->Write(port);
    }

    void State (OPacket *pkt, const bool state)
    {
        pkt->WriteOpcode(CLT_AGENT_STATE);
        pkt->Write<uint8_t>(state);
    }

    void LoginTicket (OPacket *pkt, const Ticket *ticket)
    {
        pkt->WriteOpcode(CLT_LOGIN_TICKET);
        pkt->Write<uint32_t>(ticket->ID);
        pkt->Write(ticket->usr);
        pkt->Write(ticket->pw);
        pkt->Write(ticket->host);
    }

    void AccountState (OPacket *pkt, const uint32_t accountID, const bool state)
    {
        pkt->WriteOpcode(CLT_ACCOUNT_STATE);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint8_t>(state);
    }
}
