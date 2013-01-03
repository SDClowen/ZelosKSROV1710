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

#ifndef __ZELOS_LIBSSO_SSO_ITEM_ITEM_FACTORY_H__
#define __ZELOS_LIBSSO_SSO_ITEM_ITEM_FACTORY_H__

#include <stdint.h>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include <sso/property/item.h>

class ItemProperty;

namespace sso
{
    class Item;

    class ItemFactory
    {
    public:

        typedef boost::unordered_map<uint32_t,boost::shared_ptr<ItemProperty> > item_container;

        typedef item_container::const_iterator const_iterator;

    public:

        ItemFactory ();

        ItemFactory (const item_container &property_table);

        void InsertProperty (const boost::shared_ptr<ItemProperty> &property);

        boost::shared_ptr<Item> Create (const uint32_t ID) const;

        boost::shared_ptr<Item> Create (const uint32_t ID, const sso::property::Item &property) const;

        boost::shared_ptr<ItemProperty> getProperty (const uint32_t refItemID) const;

        size_t Size () const;

    private:

        boost::unordered_map<uint32_t,boost::shared_ptr<ItemProperty> > m_item_list;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ITEM_ITEM_FACTORY_H__
