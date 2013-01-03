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

#include <net/opcode/common_srv.h>
#include <net/opcode/gateway_srv.h>
#include <packet/packet.h>

namespace srv_pkt
{
    void WriteNews (OPacket *pkt, const std::map<uint32_t,Notice> &news)
    {
        pkt->WriteOpcode(SERVER_INFORMATION);
        pkt->Write<uint8_t>(0x00);
        pkt->Write<uint8_t>(news.size());

        for (std::map<uint32_t,Notice>::const_iterator i = news.begin(); i != news.end(); ++i)
        {
            pkt->Write(i->second.heading);
            pkt->Write(i->second.message);
            pkt->Write<uint16_t>(0x07D8);
            pkt->Write<uint16_t>(i->second.month);	//Month
            pkt->Write<uint16_t>(i->second.day);	//Day
            pkt->Write<uint32_t>(0x0025000B);
            pkt->Write<uint32_t>(0xAFC00012);
            pkt->Write<uint16_t>(0x35D2);
        }
    }

    void WriteShards (OPacket *pkt, const std::string &name, const uint8_t locale,
        const std::map<uint16_t,Shard> &servers, const bool state)
    {
        pkt->WriteOpcode(SERVER_GAME_SERVER_LIST,ANSWER_ACCEPT);
        pkt->Write<uint8_t>(locale);
        pkt->Write(name);
        pkt->Write<uint8_t>(0x00);

        for (std::map<uint16_t,Shard>::const_iterator i = servers.begin(); i != servers.end(); ++i)
        {
            pkt->Write<uint8_t>(i->second.is_new);
            pkt->Write<uint16_t>(i->second.id);
            pkt->Write(i->second.name);
            pkt->Write<uint16_t>(i->second.usr_cnt);
            pkt->Write<uint16_t>(i->second.usr_max);
            pkt->Write<uint8_t>(state ? i->second.state : false);
        }

        pkt->Write<uint8_t>(0x00);
    }

    void C5 (OPacket *pkt)
    {
        enum { LOGIN_ERROR_C5 = 4 };

        pkt->WriteOpcode(SERVER_LOGIN,ANSWER_ERROR);
        pkt->Write<uint8_t>(LOGIN_ERROR_C5);
    }
}
