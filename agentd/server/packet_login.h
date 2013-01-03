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

#ifndef __ZELOS_SERVER_PACKET_LOGIN_H__
#define __ZELOS_SERVER_PACKET_LOGIN_H__

class OPacket;

namespace srv_pkt
{
    /**
     *
     *	@brief Build a login successful packet.
     *	@param pkt: Message stream to write data.
     *
     **/

    void Login (OPacket *pkt);

    /**
     *
     *	@brief Build a login failed packet with the desired error code.
     *	@param pkt: Message stream to write data.
     *	@param error: Reason why login failed.
     *
     **/

    void Login (OPacket *pkt, const int error);
}

#endif // __ZELOS_SERVER_PACKET_LOGIN_H__
