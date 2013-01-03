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

#include "server/packet_gateway.h"

#include <net/opcode/global_gateway_srv.h>
#include <packet/packet.h>

namespace srv_pkt
{
    void WriteConfig (OPacket *pkt, const uint8_t locale, const uint32_t client_version,
        const std::map<uint16_t,Shard> &servers, const std::map<uint32_t,Notice> &news)
    {
        pkt->WriteOpcode(SRV_CONFIG);
        pkt->Write<uint8_t>(locale);
        pkt->Write<uint32_t>(client_version);

        pkt->Write<uint8_t>(servers.size());
        for (std::map<uint16_t,Shard>::const_iterator i = servers.begin(); i != servers.end(); ++i)
        {
            pkt->Write<uint8_t>(i->second.is_new);
            pkt->Write(i->second.name);
            pkt->Write<uint16_t>(i->second.id);
            pkt->Write<uint16_t>(i->second.usr_cnt);
            pkt->Write<uint16_t>(i->second.usr_max);
            pkt->Write<uint8_t>(i->second.state);
        }

        pkt->Write<uint8_t>(news.size());
        for (std::map<uint32_t,Notice>::const_iterator i = news.begin(); i != news.end(); ++i)
        {
            pkt->Write<uint32_t>(i->second.id);
            pkt->Write(i->second.heading);
            pkt->Write(i->second.message);
            pkt->Write<uint16_t>(i->second.month);
            pkt->Write<uint16_t>(i->second.day);
        }
    }

    void WriteShard (OPacket *pkt, const uint16_t ID)
    {
        pkt->WriteOpcode(SRV_SHARD_REMOVE);
        pkt->Write<uint16_t>(ID);
    }

    void WriteShard (OPacket *pkt, const SHARD_NOTIFY type, const uint16_t ID, const uint16_t arg)
    {
        pkt->WriteOpcode(SRV_SHARD_NOTIFY);
        pkt->Write<uint8_t>(type);
        pkt->Write<uint16_t>(ID);
        pkt->Write<uint16_t>(arg);
    }

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const uint32_t queueID,
        const Agent &agent)
    {
        pkt->WriteOpcode(SRV_ACCOUNT_PROCESS);
        pkt->Write<uint32_t>(clientID);
        pkt->Write<uint8_t>(ANSWER_ACCEPT);
        pkt->Write<uint32_t>(queueID);
        pkt->Write(agent.host);
        pkt->Write<uint16_t>(agent.port);
        pkt->Write<uint8_t>(0x03);	//NOT SURE I ONLY SEE IT ON KSRO
    }

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error)
    {
        pkt->WriteOpcode(SRV_ACCOUNT_PROCESS);
        pkt->Write<uint32_t>(clientID);
        pkt->Write<uint8_t>(ANSWER_ERROR);
        pkt->Write<uint8_t>(error);
    }

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error, const uint32_t attempts,
                     const uint32_t max)
    {
        pkt->WriteOpcode(SRV_ACCOUNT_PROCESS);
        pkt->Write<uint32_t>(clientID);
        pkt->Write<uint8_t>(ANSWER_ERROR);
        pkt->Write<uint8_t>(error);
        pkt->Write<uint32_t>(attempts);
        pkt->Write<uint32_t>(max);
    }

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error,
        const std::string &msg, const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour,
        const uint8_t min)
    {
        pkt->WriteOpcode(SRV_ACCOUNT_PROCESS);
        pkt->Write<uint32_t>(clientID);
        pkt->Write<uint8_t>(ANSWER_ERROR);
        pkt->Write<uint8_t>(error);
        pkt->Write<uint8_t>(1);
        pkt->Write(msg);
        pkt->Write<uint16_t>(year);
        pkt->Write<uint16_t>(month);
        pkt->Write<uint16_t>(day);
        pkt->Write<uint16_t>(hour);
        pkt->Write<uint16_t>(min);
        pkt->Write<uint16_t>(0);
        pkt->Write<uint16_t>(0);
        pkt->Write<uint16_t>(0);
    }
}
