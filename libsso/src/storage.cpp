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

#include "storage.h"

#include <boost/thread/locks.hpp>

#include <sso/item/item.h>

namespace sso
{
    Storage::Storage (const uint8_t capacity, const LOCATION_TYPE type, const uint32_t ownerID)
        : m_Type(type), m_Capacity(capacity), m_OwnerID(ownerID)
    {
    }

    Storage::Storage (const uint8_t capacity, const LOCATION_TYPE type, const uint32_t ownerID,
                     const std::map<uint8_t,boost::shared_ptr<Item> >& list)
        : m_Type(type), m_Capacity(capacity), m_OwnerID(ownerID), m_Slots(list)
    {
    }

    void Storage::SetOwner (const uint32_t refID)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_OwnerID = refID;
    }

    void Storage::SetCapacity (uint8_t capacity)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_Capacity = capacity;
    }

    uint8_t Storage::GetCapacity () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Capacity;
    }

    bool Storage::IsPositionValid (const uint8_t pos) const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return pos < m_Capacity;
    }

    void Storage::MaxSlotsInc (const uint8_t slot)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_Capacity += slot;
    }

    bool Storage::InsertItem (const boost::shared_ptr<Item> &item)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        if (!item || !item->getRefID() || item->getPosition() > m_Capacity)
            return false;

        m_Slots.insert(std::make_pair(item->getPosition(),item));

        uint64_t itemID = item->ID();

        lock.unlock();

        if (!signal_insert.empty())
            itemID = *(signal_insert(item,m_OwnerID));

        item->ID(itemID);

        return true;
    }

    void Storage::MoveItem (const uint8_t src, const uint8_t dst, const uint16_t quantity)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        boost::shared_ptr<Item> isrc = m_Slots[src];
        boost::shared_ptr<Item> idest = m_Slots[dst];

        if (quantity && isrc->getMaxStack() > 1)
        {
            if (isrc->getQuantity() < quantity)
                return;

            /// UNSTACK PART OF ITEM AMOUNT TO AN EMPTY POSITION
            if (!idest && isrc->getQuantity() > quantity)
            {
                boost::shared_ptr<ItemProperty> property = isrc->property();

                idest.reset(new Item(property));

                if (idest)
                {
                    idest->setPosition(dst);
                    idest->setQuantity(quantity);

                    isrc->setQuantity(isrc->getQuantity() - quantity);

                    if (!signal_insert.empty())
                        idest->ID(*(signal_insert(idest,m_OwnerID)));

                    if (!signal_quantity.empty())
                        signal_quantity(isrc->ID(),isrc->getQuantity());

                    m_Slots[dst] = idest;
                    m_Slots[src] = isrc;
                }

                return;
            }

            /// STACK OR UNSTACK ONLY IF THEY ARE THE SAME ITEM
            if (idest && idest->getRefID() == isrc->getRefID())
            {
                /// MOVE ONLY THE MAX POSSIBLE AMOUNT OF ITEMS
                uint16_t maxQuantity = idest->getMaxStack() - idest->getQuantity();

                if (maxQuantity > quantity)
                    maxQuantity = quantity;

                uint16_t amount = isrc->getQuantity() - maxQuantity;

                idest->setQuantity(idest->getQuantity()+maxQuantity);
                isrc->setQuantity(amount);

                if (!signal_quantity.empty())
                {
                    if (isrc->getQuantity())
                        signal_quantity(isrc->ID(),isrc->getQuantity());

                    signal_quantity(idest->ID(),idest->getQuantity());
                }

                if (!amount)
                {
                    m_Slots.erase(src);

                    if (!signal_remove.empty())
                        signal_remove(isrc->ID());
                }

                return;
            }
        }

        m_Slots.erase(src);

        if (idest && idest->getQuantity())
        {
            idest->setPosition(src);
            idest->setLocation(m_Type);

            m_Slots[src] = idest;

            if (!signal_position.empty())
                signal_position(idest->ID(),src);
        }

        if (isrc->getQuantity())
        {
            isrc->setPosition(dst);

            m_Slots[dst] = isrc;

            if (!signal_position.empty())
                signal_position(isrc->ID(),dst);
        }
    }

    bool Storage::Swap (const uint8_t src, const uint8_t dst, const boost::shared_ptr<Storage> &srcStorage)
    {
        if (srcStorage.get() == this)
            return false;

        boost::lock(m_mutex,srcStorage->m_mutex);

        boost::lock_guard<boost::mutex> dstLock(m_mutex,boost::adopt_lock);

        boost::lock_guard<boost::mutex> srcLock(srcStorage->m_mutex,boost::adopt_lock);

        iterator iter = srcStorage->m_Slots.find(src);

        if (iter == srcStorage->m_Slots.end() || m_Slots.find(dst) != m_Slots.end())
            return false;

        boost::shared_ptr<Item> item = iter->second;
        item->setPosition(dst);
        item->setLocation(m_Type);

        srcStorage->m_Slots.erase(iter);

        m_Slots.insert(std::make_pair(dst,item));

        if (!signal_location.empty())
            signal_location(item->ID(),m_OwnerID,m_Type,dst);

        return true;
    }

    boost::shared_ptr<Item> Storage::Find (const uint8_t pos) const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        boost::shared_ptr<Item> item;

        const_iterator it = m_Slots.find(pos);

        if (it != m_Slots.end())
            item = it->second;

        return item;
    }

    void Storage::RemoveItem (uint8_t position)
    {
        boost::mutex::scoped_lock lock(m_mutex);

        iterator iter = m_Slots.find(position);

        if (iter != m_Slots.end())
        {
            uint64_t itemID = iter->second->ID();

            m_Slots.erase(iter);

            lock.unlock();

            if (!signal_remove.empty())
                signal_remove(itemID);
        }
    }

    uint8_t Storage::get_empty_position (const uint8_t startPos, bool reserve)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        uint8_t pos = startPos;
        uint8_t empty_pos = MAX_STORAGE_CAPACITY;

        for ( const_iterator i = m_Slots.lower_bound(startPos); i != m_Slots.end(); ++i )
        {
            if (i->first < startPos)
                continue;

            if ( i->first == pos )
                ++pos;
            else if ( empty_pos == MAX_STORAGE_CAPACITY)
            {
                if (std::find(m_ReservedPos.begin(),m_ReservedPos.end(),pos) != m_ReservedPos.end())
                    ++pos;
                else
                    empty_pos = pos;
            }
        }

        if ( empty_pos == MAX_STORAGE_CAPACITY && pos < m_Capacity)
        {
            empty_pos = pos;

            if (reserve)
                m_ReservedPos.push_back(empty_pos);
        }

        return empty_pos;
    }

    bool Storage::ReservePosition (const uint8_t pos)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        if (std::find(m_ReservedPos.begin(),m_ReservedPos.end(),pos) != m_ReservedPos.end())
            return false;

        m_ReservedPos.push_back(pos);
        return true;
    }

    bool Storage::FreePosition (const uint8_t pos)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        for (std::vector<uint8_t>::iterator it = m_ReservedPos.begin(); it != m_ReservedPos.end(); ++it)
        {
            if (*it == pos)
            {
                m_ReservedPos.erase(it);
                return true;
            }
        }

        return false;
    }

    bool Storage::Empty () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Slots.empty();
    }

    size_t Storage::Size () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Slots.size();
    }

    void Storage::Clear ()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_Slots.clear();
    }

    Storage::iterator Storage::begin ()
    {
        return m_Slots.begin();
    }

    Storage::iterator Storage::end ()
    {
        return m_Slots.end();
    }

    Storage::const_iterator Storage::begin () const
    {
        return m_Slots.begin();
    }

    Storage::const_iterator Storage::end () const
    {
        return m_Slots.end();
    }
}
