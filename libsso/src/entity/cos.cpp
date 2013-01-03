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

#include "entity/cos.h"

#include <khamaileon/property/character.h>

namespace sso
{
    COS::COS (const boost::shared_ptr<EntityProperty> &property)
        : ICharacter(property), m_DBID(0), m_OwnerSlot(255), m_Settings(0)
    {
        m_HP = m_property->HP;
        m_MP = 0;

        m_MaxHP = m_HP;
        m_MaxMP = 0;
    }

    COS::~COS()
    {
    }

    uint8_t COS::getLevel () const
    {
        return m_property->Level;
    }

    void COS::setDatabaseID (const uint64_t ID)
    {
        m_DBID = ID;
    }

    uint64_t COS::getDatabaseID () const
    {
        return m_DBID;
    }

    void COS::setOwner (const boost::shared_ptr<Player> &player)
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        m_Owner = player;
    }

    boost::shared_ptr<Player> COS::getOwner () const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_Owner;
    }

    void COS::setOwnerSlot (const uint8_t slot)
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        m_OwnerSlot = slot;
    }

    uint8_t COS::getOwnerSlot () const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_OwnerSlot;
    }

    boost::shared_ptr<Storage> COS::getInventory () const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_Inventory;
    }

    void COS::setName (const std::string &name)
    {
        m_Name = name;
    }

    const std::string& COS::getName () const
    {
        return m_Name;
    }

    void COS::setSettings (const uint32_t settings)
    {
        m_Settings = settings;
    }

    uint32_t COS::getSettings () const
    {
        return m_Settings;
    }
}
