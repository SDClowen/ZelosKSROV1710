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

#ifndef __ZELOS_LIBNET_OPCODE_AREA_GAME_SRV_H__
#define __ZELOS_LIBNET_OPCODE_AREA_GAME_SRV_H__

enum
{
    SRV_CONFIG = 0xE001,
    SRV_PEER_STATE,
    SRV_SIMULATION_ACTION,
    SRV_CHARACTER_INSERT,
    SRV_CHARACTER_ASSIGN,
    SRV_CHARACTER_EXEC,
    SRV_CHARACTER_REMOVE
};

#endif // __ZELOS_LIBNET_OPCODE_AREA_GAME_SRV_H__
