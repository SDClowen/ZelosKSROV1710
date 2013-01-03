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

#ifndef __ZELOS_LIBSSO_SSO_EXCHANGE_H__
#define __ZELOS_LIBSSO_SSO_EXCHANGE_H__

#include <map>
#include <stdint.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#define MAX_EXCHANGE_WINDOW_CAPACITY 12

namespace sso
{
    enum EXCHANGE_HANDSHAKE_TYPE
    {
        EXCHANGE_HANDSHAKE_NONE,
        EXCHANGE_HANDSHAKE_CONFIRM,
        EXCHANGE_HANDSHAKE_APPROVE
    };

    class Item;
    class Storage;

    struct ExchangeItem
    {
        uint8_t pos;
        boost::shared_ptr<Item> InvItem;
    };

    class ExchangeWnd
    {
    public:

        typedef std::map<uint8_t,ExchangeItem>::iterator iterator;

        typedef std::map<uint8_t,ExchangeItem>::const_iterator const_iterator;

    public:

        ExchangeWnd();

        /**
         *
         *	@brief Set exchanging target GUID.
         *
         *	@param targetID: GUID of the player exchanging with.
         *
         **/

        void setTarget (const uint32_t targetID);

        /**
         *
         *	@brief Get exchanging target GUID.
         *
         *	@retval GUID of the player exchanging with.
         *
         **/

        uint32_t getTarget () const;

        /**
         *
         *	@brief Set handshake for the exchange window.
         *
         *	@param type: Handshake type to establish.
         *
         **/

        void setHandshake (const EXCHANGE_HANDSHAKE_TYPE type);

        /**
         *
         *	@brief Retrieve current handshake type.
         *
         *	@retval Exchange window current handshake.
         *
         **/

        EXCHANGE_HANDSHAKE_TYPE getHandshake () const;

        /**
         *
         *	@brief Find an empty position in the exchange window.
         *
         *	@retval 255 if exchange window is full, else free slot.
         *
         **/

        uint8_t findEmptyPos () const;

        /**
         *
         *	@brief Insert a new item in the exchange window.
         *
         *	@param item: Item description to insert.
         *
         *	@retval True if the item was inserted successfuly, false theres an item
         *	in that position.
         *
         **/

        bool InsertItem (const ExchangeItem &item);

        /**
         *
         *	@brief Remove an item from the exchange window.
         *
         *	@param pos: Item position to remove.
         *
         *	@retval True if the item was erased successfuly, false theres no item in that position.
         *
         **/

        bool RemoveItem (const uint8_t pos);

        /**
         *
         *	@brief Reset exchange window item,gold and state.
         *
         **/

        void Clear ();

        /**
         *
         *	@brief Return the number of items inside the exchange window.
         *
         *	@retval Number of current items.
         *
         **/

        size_t Size () const;

        /**
         *
         *	@brief Set gold amount to exchange.
         *
         *	@param amount: Gold amount.
         *
         **/

        void setGold (const uint64_t amount);

        /**
         *
         *	@brief Retrieve current gold amount to exchange.
         *
         *	@retval Gold amount.
         *
         **/

        uint64_t getGold () const;

        const_iterator begin () const;

        const_iterator end () const;

    public:

        mutable boost::mutex m_mutex;

    private:

        uint32_t m_TargetID;
        uint64_t m_GoldAmount;
        EXCHANGE_HANDSHAKE_TYPE m_Handshake;

        std::map<uint8_t,ExchangeItem> m_Items;
    };
}

#endif //__ZELOS_LIBSSO_SSO_EXCHANGE_H__
