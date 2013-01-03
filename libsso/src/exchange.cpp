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

#include "exchange.h"

#include <boost/thread/locks.hpp>

#include <sso/item/item.h>

namespace sso
{
    ExchangeWnd::ExchangeWnd ()
        : m_TargetID(0),
        m_GoldAmount(0),
        m_Handshake(EXCHANGE_HANDSHAKE_NONE)
    {
    }

    void ExchangeWnd::setTarget (const uint32_t targetID)
    {
        m_TargetID = targetID;
    }

    uint32_t ExchangeWnd::getTarget () const
    {
        return m_TargetID;
    }

    EXCHANGE_HANDSHAKE_TYPE ExchangeWnd::getHandshake () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Handshake;
    }

    void ExchangeWnd::setHandshake (const EXCHANGE_HANDSHAKE_TYPE type)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_Handshake = type;
    }

    uint8_t ExchangeWnd::findEmptyPos () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        uint8_t pos = 0;
        uint8_t empty_pos = 255;

        for ( const_iterator i = m_Items.begin(); i != m_Items.end(); ++i )
        {
            if ( i->first == pos )
                ++pos;
            else if ( empty_pos == 255)
                empty_pos = pos;
        }

        if ( empty_pos == 255 && pos < MAX_EXCHANGE_WINDOW_CAPACITY)
            empty_pos = pos;

        return empty_pos;
    }

    bool ExchangeWnd::InsertItem (const ExchangeItem &item)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        const_iterator it = m_Items.find(item.pos);

        if (it != m_Items.end())
            return false;

        item.InvItem->Lock();
        m_Items.insert(std::make_pair(item.pos,item));
        return true;
    }

    bool ExchangeWnd::RemoveItem (const uint8_t pos)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        iterator it = m_Items.find(pos);

        if (it == m_Items.end())
            return false;

        it->second.InvItem->Unlock();

        return m_Items.erase(pos);
    }

    void ExchangeWnd::Clear ()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_TargetID = 0;
        m_GoldAmount = 0;
        m_Handshake = EXCHANGE_HANDSHAKE_NONE;

        for (const_iterator it = m_Items.begin(); it != m_Items.end(); ++it)
            it->second.InvItem->Unlock();

        m_Items.clear();
    }

    size_t ExchangeWnd::Size () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Items.size();
    }

    void ExchangeWnd::setGold (const uint64_t amount)
    {
        m_GoldAmount = amount;
    }

    uint64_t ExchangeWnd::getGold () const
    {
        return m_GoldAmount;
    }

    ExchangeWnd::const_iterator ExchangeWnd::begin () const
    {
        return m_Items.begin();
    }

    ExchangeWnd::const_iterator ExchangeWnd::end () const
    {
        return m_Items.end();
    }
}
