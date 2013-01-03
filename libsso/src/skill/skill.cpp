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

#include "skill/skill.h"

namespace sso
{
    Skill::Skill (const SkillProperty& property)
        : m_ID(property.refID),
            m_GroupID(property.GroupID),
            m_Type(static_cast<SKILL_TYPE>(property.Type)),
            m_Level(property.Level),
            m_NextSkill(property.NextSkill),
            m_SP(property.SP),
            m_Race(static_cast<RACE>(property.Race)),
            m_Mastery(property.MastID),
            m_MasteryLvl(property.MastLv),
            m_GroupSkill1(property.Skills[0].first),
            m_GroupSkillLv1(property.Skills[0].second),
            m_GroupSkill2(property.Skills[1].first),
            m_GroupSkillLv2(property.Skills[1].second),
            m_Weapon1(static_cast<REQ_WEAPON_TYPE>(property.Weapons[0])),
            m_Weapon2(static_cast<REQ_WEAPON_TYPE>(property.Weapons[1])),
            m_MP(property.MP),
            m_MinHPRatio(property.MinHPRatio),
            m_MaxHPRatio(0),
            m_InitTime(property.InitTime),
            m_DelayTime(property.DelayTime),
            m_NextTime(property.NextTime),
            m_Cooldown(property.Cooldown),
            m_StackID(property.StackID),
            m_SAD(static_cast<SKILL_ACTION_DETAIL_TYPE>(property.ActionType)),
            m_SADArg(property.ActionArg),
            m_Attack(0),
            m_Area(0)
    {
    }

    Skill::~Skill()
    {
        if (m_Attack)
            delete m_Attack;

        if (m_Area)
            delete m_Area;
    }

    uint32_t Skill::refID () const
    {
        return m_ID;
    }

    uint16_t Skill::refGroupID() const
    {
        return m_GroupID;
    }

    uint8_t Skill::Level () const
    {
        return m_Level;
    }

    uint32_t Skill::SP () const
    {
        return m_SP;
    }

    uint32_t Skill::nextSkill () const
    {
        return m_NextSkill;
    }

    SKILL_TYPE Skill::Type () const
    {
        return m_Type;
    }

    RACE Skill::Race () const
    {
        return m_Race;
    }

    uint16_t Skill::MasteryID() const
    {
        return m_Mastery;
    }

    uint8_t Skill::MasteryLv() const
    {
        return m_MasteryLvl;
    }

    uint16_t Skill::GroupID1 () const
    {
        return m_GroupSkill1;
    }

    uint8_t Skill::GroupLv1 () const
    {
        return m_GroupSkillLv1;
    }

    uint16_t Skill::GroupID2() const
    {
        return m_GroupSkill2;
    }

    uint8_t Skill::GroupLv2 () const
    {
        return m_GroupSkillLv2;
    }

    uint32_t Skill::MP () const
    {
        return m_MP;
    }

    uint8_t Skill::MinHPRatio () const
    {
        return m_MinHPRatio;
    }

    uint8_t Skill::MaxHPRatio () const
    {
        return m_MaxHPRatio;
    }

    uint16_t Skill::Init () const
    {
        return m_InitTime;
    }

    uint16_t Skill::Delay () const
    {
        return m_DelayTime;
    }

    uint16_t Skill::nextDelay () const
    {
        return m_NextTime;
    }

    uint16_t Skill::Cooldown () const
    {
        return m_Cooldown;
    }

    uint32_t Skill::StackID() const
    {
        return m_StackID;
    }

    SKILL_ACTION_TYPE Skill::ActionType() const
    {
        if ((m_Type == SKT_ATTACK && m_Attack && m_Attack->type != ATTACK_NONE) || (!m_statuses.empty() && m_Type != SKT_IMBUE))
            return SKILL_ACTION_ATTACK;

        if (!m_spawns.empty())
            return SKILL_ACTION_SUMMON;

        if (getEffectValue(SK_TELEPORT_DIST))
            return SKILL_ACTION_TELEPORT;

        if (!m_buffs.empty() || m_Type == SKT_IMBUE)
            return SKILL_ACTION_BUFF;

        return SKILL_ACTION_UNDEFINED;
    }

    bool Skill::IsStackable (const boost::shared_ptr<Skill> &skill) const
    {
        if (m_StackID && skill->m_StackID)
            return m_StackID != skill->m_StackID;

        return true;
    }

    bool Skill::IsRequired (const ESSENTIAL_ITEM_TYPE reqType, const uint8_t requirement) const
    {
        if (reqType == ESS_WEAPON)
        {
            bool hasWeaponReq = false;

            for (size_t i=0; i < m_Requirements.size(); ++i)
            {
                if (m_Requirements[i].first == ESS_WEAPON)
                {
                    hasWeaponReq = true;

                    if (m_Requirements[i].second == requirement)
                        return true;
                }
            }

            if (!hasWeaponReq)
                return true;
        }
        else if (reqType == ESS_SHIELD)
        {
        }
        else if (reqType == ESS_ARMOR)
        {
            for (size_t i = 0; i < m_Requirements.size(); ++i)
            {
                if (m_Requirements[i].first == ESS_ARMOR && m_Requirements[i].second == requirement)
                    return true;
            }
        }
        else if (reqType == ESS_DEVIL)
        {
        }
        else
        {
        }

        return false;
    }

    bool Skill::IsWeaponUsable (const REQ_WEAPON_TYPE weapon) const
    {
        if (m_Weapon1 == weapon || m_Weapon2 == weapon)
            return true;

        if (m_Weapon1 == REQ_WEAPON_ANY && m_Weapon2 == REQ_WEAPON_ANY)
        {
            bool hasWeaponReq = false;

            for (size_t i=0; i < m_Requirements.size(); ++i)
            {
                if (m_Requirements[i].first == ESS_WEAPON)
                {
                    hasWeaponReq = true;

                    if (m_Requirements[i].second == weapon)
                        return true;
                }
            }

            if (!hasWeaponReq)
                return true;
        }

        return false;
    }

    bool Skill::IsShieldRequired() const
    {
        for (size_t i = 0; i < m_Requirements.size(); ++i)
        {
            if (m_Requirements[i].first == ESS_SHIELD && m_Requirements[i].second)
                return true;
        }

        return false;
    }

    bool Skill::IsDevilSuitRequired() const
    {
        for (size_t i = 0; i < m_Requirements.size(); ++i)
        {
            if (m_Requirements[i].first == ESS_DEVIL && m_Requirements[i].second)
                return true;
        }

        return false;
    }

    uint32_t Skill::getEffectValue (const SKILL_EFFECT effect) const
    {
        uint32_t val = 0;

        std::map<SKILL_EFFECT,uint32_t>::const_iterator it = m_Effects.find(effect);

        if (it != m_Effects.end())
            val = it->second;

        return val;
    }

    void Skill::addEffect (const SKILL_EFFECT effect, const uint32_t value)
    {
        m_Effects.insert(std::make_pair(effect,value));
    }

    void Skill::setAttack (const AttackInfo &atk)
    {
        if (!m_Attack)
            m_Attack = new AttackInfo(atk);
    }


    const AttackInfo* Skill::getAttack () const
    {
        return m_Attack;
    }

    void Skill::setAttackArea (const AttackArea &area)
    {
        if (!m_Area)
            m_Area = new AttackArea(area);
    }

    const AttackArea* Skill::getAttackArea () const
    {
        return m_Area;
    }

    void Skill::addBuff (const BuffEffect &buff)
    {
        m_buffs.push_back(buff);
    }

    bool Skill::hasBuffs() const
    {
        return !m_buffs.empty();
    }

    Skill::buff_const_iterator Skill::buff_begin() const
    {
        return m_buffs.begin();
    }

    Skill::buff_const_iterator Skill::buff_end() const
    {
        return m_buffs.end();
    }

    void Skill::addEndBuff (const BuffEnd &buff)
    {
        m_end_buffs.push_back(buff);
    }

    Skill::ebuff_const_iterator Skill::ebuff_begin() const
    {
        return m_end_buffs.begin();
    }

    Skill::ebuff_const_iterator Skill::ebuff_end() const
    {
        return m_end_buffs.end();
    }

    Skill::spawn_const_iterator Skill::spawn_begin () const
    {
        return m_spawns.begin();
    }

    Skill::spawn_const_iterator Skill::spawn_end () const
    {
        return m_spawns.end();
    }

    void Skill::addRequirement (const ESSENTIAL_ITEM_TYPE type, const uint8_t requirement)
    {
        m_Requirements.push_back(std::make_pair(type,requirement));
    }
}
