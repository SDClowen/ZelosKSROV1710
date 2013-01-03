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

#ifndef __ZELOS_LIBSSO_SSO_PROPERTY_ITEM_H__
#define __ZELOS_LIBSSO_SSO_PROPERTY_ITEM_H__

#include <stdint.h>
#include <utility>
#include <vector>

#include <sso/property/item_cos.h>

namespace sso
{
    namespace property
    {
        struct Item
        {
            Item ()
                : ID(0), refID(0), position(0), location(0),
                enchantLv(0), quantity(0), mods(0), arg(0)
            {}

            uint64_t ID;
            uint32_t refID;
            uint8_t position;
            uint8_t location;
            uint8_t enchantLv;
            uint16_t quantity;
            uint64_t mods;
            uint32_t arg;	/// EQUIPMENT (DURABILITY), STONES (ASSIMILATION %) AND PETS (PET REFID)
                            /// DEVIL SPIRIT (DURATION)

            sso::property::COS cos;

            std::vector<std::pair<uint32_t,uint32_t> > magicoptions;
        };
    }
}

#endif // __ZELOS_LIBSSO_SSO_PROPERTY_ITEM_H__
