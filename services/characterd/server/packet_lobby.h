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

#ifndef __ZELOS_SERVICES_CHARACTERD_SERVER_PACKET_LOBBY_H__
#define __ZELOS_SERVICES_CHARACTERD_SERVER_PACKET_LOBBY_H__

#include <vector>

#include <boost/shared_ptr.hpp>

#include <net/constants/lobby.h>
#include <sso/property/character.h>

class OPacket;
class Player;

namespace srv_pkt
{
    void Lobby (OPacket *pkt, const uint32_t accountID);

    /**
     *
     *	@brief Build lobby action success packet.
     *	@param pkt: Message stream to write data.
     *	@param accountID: Associated account.
     *	@param action: Lobby action performed.
     *
     **/

    void Lobby (OPacket *pkt, const uint32_t accountID, const pkt::LOBBY_ACTION action);

    /**
     *
     *	@brief Build lobby list characters packet.
     *	@param pkt: Message stream to write data.
     *	@param accountID: Associated account.
     *	@param chars: Available characters list.
     *
     **/

    void Lobby (OPacket *pkt, const uint32_t accountID, const std::vector<sso::CharacterView> &chars);

    /**
     *
     *	@brief Build lobby action failed packet.
     *	@param pkt: Message stream to write data.
     *	@param accountID: Associated account.
     *	@param action: Lobby action performed.
     *	@param error: Error return code.
     *
     **/

    void Lobby (OPacket *pkt, const uint32_t accountID, const pkt::LOBBY_ACTION action,
                const pkt::LOBBY_ERROR error);

    /**
     *
     *	@brief Build character select reply packet.
     *	@param pkt: Message stream to write data.
     *	@param accountID: Associated account.
     *	@param zoneID: Character current zone refID.
     *
     **/

    void Select (OPacket *pkt, const uint32_t accountID, const uint16_t zoneID);
}

#endif // __ZELOS_SERVICES_CHARACTERD_SERVER_PACKET_LOBBY_H__
