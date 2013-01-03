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

#ifndef __ZELOS_GAMED_SERVER_PACKET_GAME_H__
#define __ZELOS_GAMED_SERVER_PACKET_GAME_H__

#include <stdint.h>

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "peer.h"

class OPacket;
class WorldArea;

namespace sso
{
    class CharacterInfo;
}

namespace srv_pkt
{
    void Config (OPacket *pkt, const WorldArea *area, const std::vector<Peer> &peers);

    void PeerState (OPacket *pkt, const uint8_t state);

    void SimulationAction (OPacket *pkt, const bool action);

    void CharacterInsert (OPacket *pkt, const uint32_t accountID,
                          const boost::shared_ptr<sso::CharacterInfo> &character);

    void CharacterAssign (OPacket *pkt, const uint32_t charID);

    void CharacterAssign (OPacket *pkt, const std::vector<uint32_t> &IDs);

    void CharacterExecute (OPacket *pkt, const uint32_t charID);

    /**
     *
     *	@brief Build single character remove notification packet.
     *	@param pkt: Message stream to write data.
     *	@param charID: Character reference ID to remove.
     *
     **/

    void CharacterRemove (OPacket *pkt, const uint32_t charID);

    /**
     *
     *	@brief Build multi character remove notification packet.
     *	@param pkt: Message stream to write data.
     *	@param IDs: Character's reference ID's to remove.
     *
     **/

    void CharacterRemove (OPacket *pkt, const std::vector<uint32_t> &IDs);
}

#endif // __ZELOS_GAMED_SERVER_PACKET_GAME_H__
