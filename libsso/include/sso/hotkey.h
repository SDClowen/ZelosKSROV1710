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

#ifndef __ZELOS_LIBSSO_SSO_HOTKEY_H__
#define __ZELOS_LIBSSO_SSO_HOTKEY_H__

#include <stdint.h>

namespace sso
{
    enum HOTKEY_TYPE
    {
        HKT_INV_ITEM = 0x46,
        HKT_EQUIP_ITEM = 0x47,
        HKT_SKILL = 0x49,
        HKT_ACTION = 0x4A
    };

    struct Hotkey
    {
        uint8_t pos;
        uint8_t type;
        uint32_t id;
    };
}

#endif // __ZELOS_LIBSSO_SSO_HOTKEY_H__
