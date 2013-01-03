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

#include "entity/cos_attack.h"

namespace sso
{
    AttackCOS::AttackCOS (const boost::shared_ptr<EntityProperty> &property)
        : COS(property), m_XP(0), m_HGP(MAX_HGP)
    {
    }

    AttackCOS::~AttackCOS ()
    {
    }

    void AttackCOS::setXP (const uint64_t XP)
    {
        m_XP = XP;
    }

    uint64_t AttackCOS::getXP () const
    {
        return m_XP;
    }

    void AttackCOS::setHGP (const uint16_t HGP)
    {
        m_HGP = HGP;
    }

    void AttackCOS::healHGP (uint16_t value)
    {
        m_HGP += value;

        if (m_HGP > MAX_HGP)
            m_HGP = MAX_HGP;
    }

    void AttackCOS::consumeHGP (const uint16_t value)
    {
        if (m_HGP)
        {
            if (value <= m_HGP)
                m_HGP -= value;
            else
                m_HGP = 0;
        }
    }

    uint16_t AttackCOS::getHGP () const
    {
        return m_HGP;
    }

    bool AttackCOS::IsDefensive () const
    {
        return m_Settings & ATTACK_COS_DEFENSIVE;
    }
}
