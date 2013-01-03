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

#include "server/packet_login.h"

#include <net/opcode/agent_srv.h>
#include <packet/packet.h>

namespace srv_pkt
{
    void Login (OPacket *pkt)
    {
        pkt->WriteOpcode(SERVER_SHARD_LOGIN,ANSWER_ACCEPT);
    }

    void Login (OPacket *pkt, const int error)
    {
        pkt->WriteOpcode(SERVER_SHARD_LOGIN,ANSWER_ERROR);
        pkt->Write<uint8_t>(error);
    }
}
