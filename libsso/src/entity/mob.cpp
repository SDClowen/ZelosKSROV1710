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

#include "entity/mob.h"

#include <khamaileon/property/character.h>

namespace sso
{
    Mob::Mob (const boost::shared_ptr<EntityProperty> &property, const uint8_t quality)
        : ICharacter(property), m_quality(quality), m_skill_idx(0), m_spawn_idx(0)
    {
        m_CurrentTask = MOB_TASK_IDLE;
        m_MoveState = MOVEMENT_ACTION_WALK;

        calculateStats();

        m_HP = m_MaxHP;
    }

    Mob::~Mob ()
    {
    }

    uint8_t Mob::Quality () const
    {
        return m_quality;
    }

    uint8_t Mob::getLevel () const
    {
        boost::mutex::scoped_lock lock(m_property_mutex);

        return m_property->Level;
    }

    bool Mob::IsTaskInterruptable () const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        return m_CurrentTask < MOB_TASK_MOVE_TO_TARGET;
    }

    uint32_t Mob::get_next_skill ()
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        for (int i = m_skill_idx; i < MAX_CHARACTER_BASE_SKILL; ++i)
        {
            if (m_property->skill_list[i])
            {
                if (m_cooldown_list.find(m_property->skill_list[i]) == m_cooldown_list.end())
                {
                    m_QueueSkill = m_property->skill_list[i];
                    m_skill_idx = ++i;
                    break;
                }
            }
            else
            {
                m_skill_idx = 0;
                m_QueueSkill = m_property->skill_list[0];
                break;
            }
        }

        if (m_skill_idx == MAX_CHARACTER_BASE_SKILL)
            m_skill_idx = 0;

        return m_QueueSkill;
    }

    void Mob::SetSpawnPoint (const size_t spawnIndex)
    {
        m_spawn_idx = spawnIndex;
    }

    size_t Mob::GetSpawnPoint () const
    {
        return m_spawn_idx;
    }

    bool Mob::IsRespawnable () const
    {
        return m_spawn_idx;
    }

    void Mob::calculatePreStats (std::vector<std::pair<CHAR_STATS, int32_t> > &bonus)
    {
        m_CR = 10;

        m_PBalance = 1;
        m_MBalance = 1;

        switch(m_quality)
        {
        case CH_QUALITY_CHAMPION:
            m_MaxHP = m_property->HP*2;
            break;
        case CH_QUALITY_GIANT:
            m_MaxHP = m_property->HP*20;
            break;
        case CH_QUALITY_ELITE:
        case CH_QUALITY_EVENT_ELITE:
            m_MaxHP = m_property->HP*30;
            break;
        default:
            m_MaxHP = m_property->HP;
            break;
        }
    }
}
