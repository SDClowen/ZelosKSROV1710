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

#ifndef __ZELOS_LIBSSO_SSO_SKILL_REQUIREMENT_H__
#define __ZELOS_LIBSSO_SSO_SKILL_REQUIREMENT_H__

namespace sso
{
    enum REQ_WEAPON_TYPE
    {
        REQ_WEAPON_NONE = 1,
        REQ_WEAPON_CH_SWORD,
        REQ_WEAPON_CH_BLADE,
        REQ_WEAPON_CH_SPEAR,
        REQ_WEAPON_CH_GLAIVE,
        REQ_WEAPON_CH_BOW,
        REQ_WEAPON_EU_SWORD,
        REQ_WEAPON_EU_TSWORD,
        REQ_WEAPON_EU_AXE,
        REQ_WEAPON_EU_WARLOCK,
        REQ_WEAPON_EU_TSTAFF,
        REQ_WEAPON_EU_CROSSBOW,
        REQ_WEAPON_EU_DAGGER,
        REQ_WEAPON_EU_HARP,
        REQ_WEAPON_EU_CLERIC,
        REQ_WEAPON_CH_EU_FORTRESS,
        REQ_WEAPON_ANY = 255
    };

    enum REQ_ARMOR_TYPE
    {
        REQ_ARMOR_LA,
        REQ_ARMOR_NONE
    };
}

#endif //__ZELOS_LIBSSO_SSO_SKILL_REQUIREMENT_H__
