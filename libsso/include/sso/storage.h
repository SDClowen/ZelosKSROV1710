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

#ifndef __ZELOS_LIBSSO_SSO_STORAGE_H__
#define __ZELOS_LIBSSO_SSO_STORAGE_H__

#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <map>

#include <sso/location.h>

#define MAX_STORAGE_CAPACITY 255

namespace sso
{
    class Item;

    class Storage
    {
    public:

        boost::signals2::signal<uint64_t (const boost::shared_ptr<Item>&, uint32_t)> signal_insert;

        boost::signals2::signal<void (uint64_t,uint32_t,uint8_t,uint8_t)> signal_location;

        boost::signals2::signal<void (const uint64_t, const uint8_t)> signal_position;

        boost::signals2::signal<void (uint64_t, uint16_t)> signal_quantity;

        boost::signals2::signal<void (const uint64_t)> signal_remove;

    public:

        typedef std::map<uint8_t,boost::shared_ptr<Item> >::iterator iterator;
        typedef std::map<uint8_t,boost::shared_ptr<Item> >::const_iterator const_iterator;

    public:

        Storage (const uint8_t capacity, const LOCATION_TYPE type, const uint32_t ownerID);

        Storage (const uint8_t capacity, const LOCATION_TYPE type, const uint32_t ownerID,
                 const std::map<uint8_t,boost::shared_ptr<Item > >& list);

        /**
         *
         *	@brief Set storage owner reference ID.
         *
         *	@param refID: Owner refID.
         *
         **/

        void SetOwner (const uint32_t refID);

        /**
         *
         *	@brief Set storage max capacity.
         *
         *	@param capacity: Storage max available slots.
         *
         **/

        void SetCapacity (uint8_t capacity);

        /**
         *
         *	@brief Retrieve storage max available slots.
         *
         *	@retval Max capacity.
         *
         **/

        uint8_t GetCapacity () const;

        /**
         *
         *
         *	@brief Check if a position is valid.
         *
         *	@retval True if the position is inside the storage range [0,Capacity), false otherwise.
         *
         **/

        bool IsPositionValid (const uint8_t pos) const;

        /**
         *
         *	@brief Increase storage max capacity.
         *
         *	@param slots: Number of slots to add.
         *
         **/

        void MaxSlotsInc (const uint8_t slot);

        /**
         *
         *	@brief Insert an item to the storage.
         *
         *	@param item: Item object to insert.
         *
         *	@retval True if the operation was successful, false otherwise.
         *
         **/

        bool InsertItem (const boost::shared_ptr<Item> &item);

        /**
         *
         *	@brief Move an item to a different position inside the storage.
         *
         *	@param src: Source position of the item.
         *	@param dst: Destination position of the item.
         *	@param quantity: Amount of items to move.
         *
         **/

        void MoveItem (const uint8_t src, const uint8_t dst, const uint16_t quantity);

        /**
         *
         *	@brief Swap an item from one storage to another.
         *
         *	@param src: Item source position to retrive the item.
         *	@param dst: Item destination position to insert the item.
         *	@param srcStorage: Storage where to retrieve the item.
         *
         *	@retval True if the operation was successful, false otherwise.
         *
         **/

        bool Swap (const uint8_t src, const uint8_t dst, const boost::shared_ptr<Storage> &srcStorage);

        /**
         *
         *	@brief Retrieve an item in the desired position.
         *
         *	@param pos: Item position to search for.
         *
         *	@retval Item in the respective position.
         *
         **/

        boost::shared_ptr<Item> Find (const uint8_t pos) const;

        /**
         *
         *	@brief Remove the item in the desired position.
         *
         *	@param pos: Storage position to remove the desired item.
         *
         **/

        void RemoveItem (const uint8_t pos);

        uint8_t get_empty_position (const uint8_t start_pos, bool reserve = false);

        /**
         *
         *	@brief Reserve a position for further use.
         *
         *	@param pos: Position to reserve.
         *
         *	@retval True if the position was reserved, false the position is already reserved.
         *
         **/

        bool ReservePosition (const uint8_t pos);

        /**
         *
         *	@brief Free a reserved position.
         *
         *	@param pos Position to free.
         *
         *	@retval True if operation was successful, false otherwise.
         *
         **/

        bool FreePosition (const uint8_t pos);

        /**
         *
         *	@brief Check if there no items stored.
         *
         *	@retval True if theres no items stored, false otherwise.
         *
         **/

        bool Empty() const;

        /**
         *
         *	@brief Retrieve current stored items amount.
         *
         *	@retval Number of current items stored.
         *
         **/

        size_t Size() const;

        /**
         *
         *	@brief Remove all items stored.
         *
         **/

        void Clear();

        iterator begin ();

        iterator end ();

        const_iterator begin () const;

        const_iterator end () const;

    public:

        mutable boost::mutex m_mutex;

    protected:

        LOCATION_TYPE m_Type;

        uint8_t m_Capacity;
        uint32_t m_OwnerID;

        std::vector<uint8_t> m_ReservedPos;
        std::map<uint8_t,boost::shared_ptr<Item> > m_Slots;
    };
}

#endif //__ZELOS_LIBSSO_SSO_STORAGE_H__
