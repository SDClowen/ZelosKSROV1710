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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_DROP_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_DROP_H__

#include <boost/shared_ptr.hpp>

#include <sso/entity/entity.h>

namespace sso
{
    enum DROP_SOURCE
    {
        DROP_SOURCE_MOB = 5,
        DROP_SOURCE_PLAYER = 6
    };

    class Item;

    struct Drop : public Entity
    {
        Drop()
            : m_OwnerID(0), m_OwnerAuth(-1), m_GoldAmount(0),
            m_Source(0), m_SourceID(0), m_Blocked(false)
        {}

        std::string m_OwnerAlias;
        uint32_t m_OwnerID;
        uint32_t m_OwnerAuth;
        uint32_t m_GoldAmount;
        uint8_t m_Source;
        uint32_t m_SourceID;
        bool m_Blocked;

        boost::shared_ptr<Item> m_Item;
    };
}

#endif //__ZELOS_LIBSSO_SSO_ENTITY_DROP_H__
