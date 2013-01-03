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

#include "packet.h"

#include <net/opcode/global_gateway_clt.h>
#include <packet/packet.h>

namespace clt_pkt
{
    void WriteConfig (OPacket *pkt)
    {
        pkt->WriteOpcode(CLT_CONFIG);
    }

    void WriteQueue (OPacket *pkt, const uint32_t clientID, const std::string &usr,
        const std::string &pw, const uint32_t shardID, const std::string &IP)
    {
        pkt->WriteOpcode(CLT_LOGIN_QUEUE);
        pkt->Write<uint32_t>(clientID);
        pkt->Write<uint32_t>(shardID);
        pkt->Write(usr);
        pkt->Write(pw);
        pkt->Write(IP);
    }
}
