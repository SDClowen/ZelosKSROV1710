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

#include "stall.h"

#include <boost/thread/locks.hpp>

#include <sso/item/item.h>

namespace sso
{
    Stall::Stall (const std::string& name, const uint32_t owner)
        : m_Name(name),m_Owner(owner),m_State(false)
    {
    }

    uint32_t Stall::getOwner () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Owner;
    }

    bool Stall::IsOwner (const uint32_t playerID) const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Owner == playerID;
    }

    void Stall::setTitle (const std::string& name)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_Name = name;
    }

    const std::string& Stall::getTitle () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Name;
    }

    void Stall::setGreeting (const std::string& greeting)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        m_Greeting = greeting;

        lock.unlock();

        if (!signal_greeting.empty())
            signal_greeting(greeting);
    }

    const std::string& Stall::getGreeting () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Greeting;
    }

    void Stall::setState (const uint8_t state, const uint8_t reg)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        m_State = state;

        m_Register = reg;

        lock.unlock();

        if (!signal_state.empty())
            signal_state(state,reg);
    }

    uint8_t Stall::getState () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_State;
    }

    uint8_t Stall::getRegister () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_Register;
    }

    bool Stall::IsOpen () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_State;
    }

    void Stall::Enter (const uint32_t playerID, const std::string &name)
    {
        if (!signal_enter.empty())
            signal_enter(playerID,name);

        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_IDs.push_back(playerID);
    }

    void Stall::Leave (const uint32_t playerID)
    {
        if (!signal_leave.empty())
            signal_leave(playerID);

        boost::lock_guard<boost::mutex> lock(m_mutex);

        m_IDs.erase(std::remove(m_IDs.begin(),m_IDs.end(),playerID));
    }

    void Stall::Close ()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        for (const_iterator it = m_Items.begin(); it != m_Items.end(); ++it)
            it->second.InvItem->Unlock();

        lock.unlock();

        if (!signal_close.empty())
            signal_close(m_Owner);
    }

    void Stall::InsertItem (const StallItem &item)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        if (item.pos < MAX_STALL_ITEMS)
        {
            const_iterator it = m_Items.find(item.pos);

            if (it == m_Items.end())
            {
                item.InvItem->Lock();

                m_Items.insert(std::make_pair(item.pos,item));

                lock.unlock();

                if (!signal_insert.empty())
                    signal_insert();
            }
        }
    }

    void Stall::ModifyItem (const uint8_t pos, const uint16_t quantity, const uint64_t price)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        iterator i = m_Items.find(pos);

        if (i != m_Items.end())
        {
            i->second.Quantity = quantity;

            i->second.Price = price;

            lock.unlock();

            if (!signal_price.empty())
                signal_price(pos,quantity,price);
        }
    }

    StallItem Stall::Find (const uint8_t pos) const
    {
        StallItem item;
        item.pos = 255;
        item.Price = 0;
        item.Quantity = 0;

        boost::lock_guard<boost::mutex> lock(m_mutex);

        const_iterator it = m_Items.find(pos);

        if (it != m_Items.end())
            item = it->second;

        return item;
    }

    void Stall::RemoveItem (const uint8_t pos)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        iterator it = m_Items.find(pos);

        if (it != m_Items.end())
        {
            it->second.InvItem->Unlock();

            m_Items.erase(it);

            lock.unlock();

            if (!signal_remove.empty())
                signal_remove();
        }
    }

    void Stall::BuyItem (const std::string &name,const uint8_t pos)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);

        iterator it = m_Items.find(pos);

        if (it != m_Items.end())
        {
            it->second.InvItem->Unlock();

            m_Items.erase(it);
        }

        bool hasItems = !m_Items.empty();

        lock.unlock();

        if (!signal_buy.empty())
            signal_buy(pos,name);

        if (!hasItems && !signal_close.empty())
            signal_close(m_Owner);
    }

    Stall::const_iterator Stall::begin () const
    {
        return m_Items.begin();
    }

    Stall::const_iterator Stall::end() const
    {
        return m_Items.end();
    }

    std::vector<uint32_t> Stall::getClients () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_IDs;
    }

    bool Stall::IsFull () const
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);

        return m_IDs.size() == MAX_STALL_CLIENTS;
    }

    void Stall::notifyMsg (const std::string &name, const std::string &msg)
    {
        if (!signal_msg.empty())
            signal_msg(name,msg);
    }
}
