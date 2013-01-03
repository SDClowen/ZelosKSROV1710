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

#include "packet_lobby.h"

#include <net/opcode/chard_srv.h>
#include <packet/packet.h>

namespace srv_pkt
{
    void Lobby (OPacket *pkt, const uint32_t accountID, const pkt::LOBBY_ACTION action)
    {
        pkt->WriteOpcode(SRV_CHARD_LOBBY);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint8_t>(action);
        pkt->Write<uint8_t>(ANSWER_ACCEPT);
    }

    void Lobby (OPacket *pkt, const uint32_t accountID, const std::vector<sso::CharacterView> &chars)
    {
        Lobby(pkt,accountID,pkt::LOBBY_ACTION_LIST);

        pkt->Write<uint8_t>(chars.size());

        for (std::vector<sso::CharacterView>::const_iterator it = chars.begin(); it < chars.end(); ++it)
        {
            pkt->Write<uint32_t>(it->refID);
            pkt->Write(it->name);
            pkt->Write<uint16_t>(0);     ///KSRO ONLY I THINK
            pkt->Write<uint8_t>(it->scale);
            pkt->Write<uint8_t>(it->level);
            pkt->Write<uint64_t>(it->exp);
            pkt->Write<uint16_t>(it->statStr);
            pkt->Write<uint16_t>(it->statInt);
            pkt->Write<uint16_t>(it->stats);
            pkt->Write<uint32_t>(it->HP);
            pkt->Write<uint32_t>(it->MP);

            if (it->timeMin)
            {
                pkt->Write<uint8_t>(1);
                pkt->Write<uint16_t>(it->timeMin);
                pkt->Write<uint32_t>(0);
                pkt->Write<uint8_t>(0);
            }
            else
                pkt->Write<uint32_t>(0);

            pkt->Write<uint8_t>(it->equipItems.size());

            for (size_t j=0; j < it->equipItems.size(); ++j)
            {
                pkt->Write<uint32_t>(it->equipItems[j].first);
                pkt->Write<uint8_t>(it->equipItems[j].second);
            }

            pkt->Write<uint8_t>(it->avatarItems.size());

            for (size_t j=0; j < it->avatarItems.size(); ++j)
            {
                pkt->Write<uint32_t>(it->avatarItems[j].first);
                pkt->Write<uint8_t>(it->avatarItems[j].second);
            }
        }
    }

    void Lobby (OPacket *pkt, const uint32_t accountID, const pkt::LOBBY_ACTION action,
                const pkt::LOBBY_ERROR error)
    {
        pkt->WriteOpcode(SRV_CHARD_LOBBY);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint8_t>(action);
        pkt->Write<uint8_t>(ANSWER_ERROR);
        pkt->Write<uint16_t>(error);
    }

    void Select (OPacket *pkt, const uint32_t accountID, const uint16_t zoneID)
    {
        pkt->WriteOpcode(SRV_CHARD_SELECT);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint16_t>(zoneID);
        pkt->Write<uint8_t>(ANSWER_ACCEPT);
    }
}
