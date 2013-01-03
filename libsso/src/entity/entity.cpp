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

#include "entity/entity.h"

#include <cassert>

namespace sso
{
    Entity::Entity (float x,float y, float z, int zone)
        : m_ChannelID(0), m_CurrentPos(x,y,z, zone),m_Angle(0)
    {
        m_UniqueID = 10 + m_Count;
        ++m_Count;
    }

    Entity::Entity (const Coord &position)
        : m_ChannelID(0), m_CurrentPos(position), m_Angle(0)
    {
        m_UniqueID = 10 + m_Count;
        ++m_Count;
    }

    void Entity::setChannelID (const uint32_t channelID)
    {
        boost::lock_guard<boost::shared_mutex> lock(m_channel_mutex);

        m_ChannelID = channelID;
    }

    uint32_t Entity::getChannelID () const
    {
        boost::shared_lock<boost::shared_mutex> lock(m_channel_mutex);

        return m_ChannelID;
    }

    uint32_t Entity::GUID() const
    {
        return m_UniqueID;
    }

    void Entity::setPosition (const Coord& position)
    {
        assert(position.get_x() >= 0 && position.get_x() <= 1920);
        assert(position.get_y() >= 0 && position.get_y() <= 1920);

        boost::lock_guard<boost::mutex> lock(m_mov_mutex);

        m_CurrentPos = position;
    }

    Coord Entity::getPosition () const
    {
        boost::lock_guard<boost::mutex> lock(m_mov_mutex);

        return m_CurrentPos;
    }

    void Entity::setAngle (const uint16_t angle)
    {
        boost::lock_guard<boost::mutex> lock(m_mov_mutex);

        m_Angle = angle;
    }

    uint16_t Entity::getAngle () const
    {
        boost::lock_guard<boost::mutex> lock(m_mov_mutex);

        return m_Angle;
    }

    uint32_t Entity::m_Count = 0;
}
