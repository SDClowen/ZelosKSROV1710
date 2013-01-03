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

#ifndef __ZELOS_SHARDD_SERVER_PACKET_AGENT_H__
#define __ZELOS_SHARDD_SERVER_PACKET_AGENT_H__

#include <stdint.h>
#include <string>
#include <vector>

class Area;
class OPacket;
class Ticket;

namespace srv_pkt
{
    enum LOGIN_ERROR
    {
        LOGIN_ERROR_C9 = 1,
        LOGIN_ERROR_C10,
        LOGIN_ERROR_C11,
        LOGIN_ERROR_USER_LIMIT,
        LOGIN_ERROR_IP_LIMIT,
        LOGIN_ERROR_BAD_CREDENTIALS
    };

    void AgentConfig (OPacket *pkt, const uint8_t locale, const uint8_t race, const uint32_t version,
                      const std::string &chFilter, const std::vector<std::string> &strFilter, const uint8_t maxAreas);

    void AgentAction (OPacket *pkt, const uint8_t action);

    void WorldArea (OPacket *pkt, const Area *area);

    void LoginTicket (OPacket *pkt, Ticket *ticket);

    void LoginTicket (OPacket *pkt, const LOGIN_ERROR error, const uint32_t ticketID);
}

#endif // __ZELOS_SHARDD_SERVER_PACKET_AGENT_H__
