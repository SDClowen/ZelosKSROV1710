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

#include "mastery.h"

namespace sso
{
    MasteryTree::MasteryTree ()
    {
    }

    MasteryTree::MasteryTree (const std::map<uint32_t, uint8_t> &tree)
        : m_tree(tree)
    {
    }

    int MasteryTree::getLimit (const RACE race, const uint8_t lv) const
    {
        int limit = 0;

        switch(race)
        {
        case RACE_EU:
            limit = 2*lv;
            break;
        case RACE_CH:
            limit = 330;
            break;
        default:
            break;
        }

        return limit;
    }

    int MasteryTree::getTotal () const
    {
        int total = 0;

        for (const_iterator i = m_tree.begin(); i != m_tree.end(); ++i)
            total += i->second;

        return total;
    }

    int MasteryTree::getBonus (const uint32_t masteryID) const
    {
        int bonus = 0;

        const_iterator iter = m_tree.find(masteryID);

        if (iter != m_tree.end())
        {
            switch(masteryID)
            {
            case MASTERY_CH_SWORD:
            case MASTERY_CH_SPEAR:
            case MASTERY_CH_BOW:
            case MASTERY_CH_FIRE:
            case MASTERY_CH_COLD:
            case MASTERY_CH_LIGHT:
            case MASTERY_EU_WARLOCK:
                bonus = iter->second;
                break;
            default:
                break;
            }
        }

        return bonus;
    }

    const std::map<uint32_t,uint8_t>& MasteryTree::getMasteries() const
    {
        return m_tree;
    }

    bool MasteryTree::IsMax (const RACE race, const uint8_t lv) const
    {
        return getTotal() == getLimit(race,lv);
    }

    std::pair<MasteryTree::Mastery,MasteryTree::Mastery> MasteryTree::getHighests () const
    {
        std::pair<MasteryTree::Mastery,MasteryTree::Mastery> t;

        for (const_iterator i = m_tree.begin(); i != m_tree.end(); ++i)
        {
            if (i->second > t.first.second)
            {
                if (t.first.second > t.second.second)
                    t.second = t.first;

                t.first = *i;
            }
            else if (i->second > t.second.second)
                t.second = *i;
        }

        return t;
    };

    MasteryTree MasteryTree::Create (const RACE race)
    {
        MasteryTree tr;

        if (race == RACE_EU)
        {
            tr.m_tree.insert(std::make_pair(513,0));
            tr.m_tree.insert(std::make_pair(514,0));
            tr.m_tree.insert(std::make_pair(515,0));
            tr.m_tree.insert(std::make_pair(516,0));
            tr.m_tree.insert(std::make_pair(517,0));
            tr.m_tree.insert(std::make_pair(518,0));

        }
        else if ( race == RACE_CH )
        {
            tr.m_tree.insert(std::make_pair(257,0));
            tr.m_tree.insert(std::make_pair(258,0));
            tr.m_tree.insert(std::make_pair(259,0));
            tr.m_tree.insert(std::make_pair(273,0));
            tr.m_tree.insert(std::make_pair(274,0));
            tr.m_tree.insert(std::make_pair(275,0));
            tr.m_tree.insert(std::make_pair(276,0));
        }

        return tr;
    }
}
