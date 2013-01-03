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

#include "item/item_factory.h"

#include <khamaileon/property/item.h>

#include "item/item.h"

namespace sso
{
    ItemFactory::ItemFactory ()
    {
    }

    ItemFactory::ItemFactory (const item_container &property_table)
        : m_item_list(property_table)
    {
    }

    void ItemFactory::InsertProperty (const boost::shared_ptr<ItemProperty> &property)
    {
        m_item_list.insert(std::make_pair(property->refID,property));
    }

    boost::shared_ptr<Item> ItemFactory::Create (const uint32_t ID) const
    {
        boost::shared_ptr<Item> item;

        const_iterator i = m_item_list.find(ID);

        if (i != m_item_list.end())
            item.reset(new Item(i->second));

        return item;
    }

    boost::shared_ptr<Item> ItemFactory::Create (const uint32_t ID,
                                                 const sso::property::Item &property) const
    {
        boost::shared_ptr<Item> item;

        const_iterator i = m_item_list.find(ID);

        if (i != m_item_list.end())
            item.reset(new Item(i->second,property));

        return item;
    }

    boost::shared_ptr<ItemProperty> ItemFactory::getProperty (const uint32_t refItemID) const
    {
        boost::shared_ptr<ItemProperty> property;

        const_iterator i = m_item_list.find(refItemID);

        if (i != m_item_list.end())
            property = i->second;

        return property;
    }

    size_t ItemFactory::Size() const
    {
        return m_item_list.size();
    }
}
