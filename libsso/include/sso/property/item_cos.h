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

#ifndef __ZELOS_LIBSSO_SSO_PROPERTY_COS_H__
#define __ZELOS_LIBSSO_SSO_PROPERTY_COS_H__

#include <stdint.h>
#include <string>

namespace sso
{
    namespace property
    {
        struct COS
        {
            COS ()
                : ID(0), state(0), HP(0), HGP(0), XP(0),
                Minutes(0), settings(0)
            {}

            uint32_t ID;
            uint8_t state;
            std::string name;
            uint32_t HP;
            uint16_t HGP;
            uint64_t XP;
            uint32_t Minutes;
            uint32_t settings;
        };
    }
}

#endif // __ZELOS_LIBSSO_SSO_PROPERTY_COS_H__
