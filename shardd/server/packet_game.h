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

#ifndef __ZELOS_SHARDD_SERVER_PACKET_GAME_H__
#define __ZELOS_SHARDD_SERVER_PACKET_GAME_H__

#include <string>

class OPacket;
class Area;

namespace srv_pkt
{
    void AreaConfig (OPacket *pkt, Area *area);

    void AreaAction (OPacket *pkt, bool action);

    void AreaSrv (OPacket *pkt, const std::string &host, const std::string &port);
}

#endif // __ZELOS_SHARDD_SERVER_PACKET_GAME_H__
