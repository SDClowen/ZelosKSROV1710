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

#include "entity/cos_transport.h"

#include <khamaileon/property/character.h>
#include <sso/item/item.h>
#include <sso/storage.h>

namespace sso
{
    Transport::Transport (const boost::shared_ptr<EntityProperty> &property)
        : COS(property), m_Mounted(true), m_TradeScale(0)
    {
        if (property->type() == ET_COS_TRANSPORT)
            m_Inventory.reset(new Storage(0x41,LOCATION_TRANSPORT,0));
    }

    Transport::~Transport ()
    {
    }

    bool Transport::has_goods () const
    {
        if (!m_Inventory)
            return false;

        return m_Inventory->Size();
    }

    bool Transport::IsMounted () const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_Mounted;
    }

    void Transport::Mount ()
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        m_Mounted = true;
    }

    void Transport::Dismount ()
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        m_Mounted = false;
    }

    uint8_t Transport::TradeScale() const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_TradeScale;
    }

    void Transport::CalculateTradeScale()
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        uint16_t max_goods = 0;

        boost::mutex::scoped_lock store_lock(m_Inventory->m_mutex);

        for (Storage::const_iterator it = m_Inventory->begin(); it != m_Inventory->end(); ++it)
            max_goods += it->second->getQuantity();

        store_lock.unlock();

        m_TradeScale = 0;

        if (max_goods)
        {
            if (max_goods > 0 && max_goods <= 719)
            {
                m_TradeScale = 1;
            }
            else if (max_goods > 719 && max_goods <= 1293)
            {
                m_TradeScale = 2;
            }
            else if (max_goods > 1293 && max_goods <= 2012)
            {
                m_TradeScale = 3;
            }
            else if (max_goods > 2012 && max_goods <= 2600)
            {
                m_TradeScale = 4;
            }
            else
            {
                m_TradeScale = 5;
            }
        }
    }
}
