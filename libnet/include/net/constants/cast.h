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

#ifndef DEF_PKT_CAST_H
#define DEF_PKT_CAST_H

namespace pkt
{

enum SKCAST_ERROR_CODE
{
    ERROR_SKCAST_INVALID_SKILL = 0x3003,
    ERROR_SKCAST_INVALID_MP = 0x3004,
    ERROR_SKCAST_INVALID_TARGET = 0x3006,
    ERROR_SKCAST_INVALID_EQUIPMENT = 0x300D,
    ERROR_SKCAST_INVALID_AMMO = 0x300E,
    ERROR_SKCAST_WEAPON_BROKEN = 0x300F,
    ERROR_SKCAST_OBSTACLE = 0x3010,
    ERROR_SKCAST_INVALID_HP = 0x3013,
    ERROR_SKCAST_PK_INVALID_LEVEL = 0x3016,
    ERROR_SKCAST_SAFE_ZONE = 0x3018,
    ERROR_SKCAST_TRANSPORT = 0x3021,
    ERROR_SKCAST_PARTY_MEMBER = 0x3022,
    ERROR_SKCAST_PARTY_TRANSPORT = 0x3023,
    ERROR_SKCAST_PK_DISABLED = 0x3025,
    ERROR_SKCAST_ITEM_COOLDOWN = 0x3030
};

}

#endif // DEF_PKT_CAST_H
