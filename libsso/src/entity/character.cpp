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

#include "entity/character.h"

#include <cmath>
#include <boost/thread/locks.hpp>

#include <khamaileon/property/character.h>
#include <sso/skill/skill.h>

template <typename T>
inline T linear_regression (const T &value, const short percent)
{
    return  ((100+percent)*value)/100;
}

namespace sso
{
    ICharacter::ICharacter (const boost::shared_ptr<EntityProperty> &property)
        :   m_property(property),
            m_WalkSpeed(property->WalkSpeed),
            m_RunSpeed(property->RunSpeed),
            m_BerserkSpeed(property->BersekSpeed),
            m_StatusState(STATUS_ACTION_ALIVE),
            m_AttributeState(ATTRIBUTE_ACTION_NORMAL),
            m_AttackState(ATTACK_ACTION_STOP),
            m_MoveState(MOVEMENT_ACTION_RUN),
            m_StanceState(MOVEMENT_ACTION_STAND),
            m_Knockdown(false)
    {
        m_QueueSkill = 0;
        m_QueueTarget = 0;

        m_EncStatus = 0;
    }

    ICharacter::~ICharacter ()
    {
    }

    boost::shared_ptr<EntityProperty> ICharacter::getProperty () const
    {
        return m_property;
    }

    uint32_t ICharacter::getRefID () const
    {
        boost::mutex::scoped_lock lock(m_property_mutex);

        return m_property->refID;
    }

    ENTITY_TYPE ICharacter::getType () const
    {
         boost::mutex::scoped_lock lock(m_property_mutex);

        return m_property->type();
    }

    RACE ICharacter::getRace () const
    {
        boost::mutex::scoped_lock lock(m_property_mutex);

        return static_cast<RACE>(m_property->Race);
    }

    GENDER ICharacter::getGender () const
    {
         boost::mutex::scoped_lock lock(m_property_mutex);

        return static_cast<GENDER>(m_property->Gender);
    }

    uint32_t ICharacter::HP () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_HP;
    }

    uint32_t ICharacter::maxHP () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MaxHP;
    }

    uint8_t ICharacter::percentHP () const
    {
        boost::lock_guard<boost::mutex> lock(m_stats_mutex);

        return m_MaxHP ? (10*m_HP)/m_MaxHP : 0;
    }

    void ICharacter::setHP (const uint32_t value)
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        m_HP = (value > m_MaxHP) ? m_MaxHP : value;
    }

    void ICharacter::healHP (const uint32_t amount)
    {
        if (amount)
        {
            boost::mutex::scoped_lock lock(m_stats_mutex);

            m_HP += amount;

            if (m_HP > m_MaxHP)
                m_HP = m_MaxHP;
        }
    }

    void ICharacter::consumeHP (const uint32_t amount)
    {
        if (amount)
        {
            boost::mutex::scoped_lock stats_lock(m_stats_mutex);

            if (m_HP)
            {
                if ( amount > m_HP)
                    m_HP = 0;
                else
                    m_HP -= amount;

                if (!m_HP)
                {
                    stats_lock.unlock();

                    if (!signal_dead.empty())
                        signal_dead(m_UniqueID,m_CurrentPos);

                    boost::lock(m_state_mutex,m_queue_mutex,m_mov_mutex);

                    boost::unique_lock<boost::mutex> stateLock(m_state_mutex,boost::adopt_lock);
                    boost::unique_lock<boost::mutex> queueLock(m_queue_mutex,boost::adopt_lock);
                    boost::unique_lock<boost::mutex> moveLock(m_mov_mutex,boost::adopt_lock);

                    if (m_AttackState != ATTACK_ACTION_STOP)
                    {
                        m_AttackState = ATTACK_ACTION_STOP;

                        if (!signal_state.empty())
                            signal_state(STATE_ATTACK,m_AttackState);
                    }

                    m_StatusState = STATUS_ACTION_DEAD;

                    if (!signal_state.empty())
                        signal_state(STATE_STATUS,m_StatusState);

                    m_QueueSkill = 0;
                    m_QueueTarget = 0;
                    m_CurrentTask = 0;

                    m_NextPos = Coord();
                }
            }
        }
    }

    void ICharacter::inc_aggro (const uint32_t aggro, const uint32_t attacker_id)
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        m_aggro_list[attacker_id] += aggro;
    }

    void ICharacter::receive_damage (const uint32_t damage, const uint32_t attacker_id)
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        if (m_HP)
        {
            if (damage > m_HP)
                m_damage_list[attacker_id] += m_HP;
            else
                m_damage_list[attacker_id] += damage;

            lock.unlock();

            consumeHP(damage);
        }
    }

    std::multimap<uint32_t,uint32_t> ICharacter::getDamageList () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        std::multimap<uint32_t,uint32_t> dmgs;

        for (std::map<uint32_t,uint32_t>::const_iterator it = m_damage_list.begin(); it != m_damage_list.end(); ++it)
            dmgs.insert(std::make_pair(it->second,it->first));

        return dmgs;
    }


    uint32_t ICharacter::MP () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MP;
    }

    uint32_t ICharacter::maxMP () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MaxMP;
    }

    uint8_t ICharacter::percentMP () const
    {
        boost::lock_guard<boost::mutex> lock(m_stats_mutex);

        return m_MaxMP ? (10*m_MP)/m_MaxMP : 0;
    }

    void ICharacter::healMP (const uint32_t amount)
    {
        if (amount)
        {
            boost::mutex::scoped_lock lock(m_stats_mutex);

            m_MP += amount;

            if (m_MP > m_MaxMP)
                m_MP = m_MaxMP;
        }
    }

    void ICharacter::consumeMP (const uint32_t amount)
    {
        if (amount)
        {
            boost::mutex::scoped_lock lock(m_stats_mutex);

            if ( amount > m_MP)
                m_MP = 0;
            else
                m_MP -= amount;
        }
    }

    uint16_t ICharacter::physical_defense () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PD;
    }

    uint16_t ICharacter::magical_defense () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MD;
    }

    float ICharacter::physical_absorb () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PAbs;
    }

    float ICharacter::magical_absorb () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MAbs;
    }

    float ICharacter::physical_rate() const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PRate;
    }

    float ICharacter::magical_rate() const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MRate;
    }

    float ICharacter::physical_balance() const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PBalance;
    }

    float ICharacter::magical_balance() const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MBalance;
    }

    uint16_t ICharacter::hit_ratio () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_HR;
    }

    uint16_t ICharacter::parry_ratio () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PR;
    }

    float ICharacter::attack_distance () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_AD;
    }

    ICharacter::attack_type ICharacter::physical_attack () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_PA;
    }

    ICharacter::attack_type ICharacter::magical_attack () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_MA;
    }

    uint16_t ICharacter::critical_ratio () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_CR;
    }

    uint16_t ICharacter::block_ratio () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_BR;
    }

    float ICharacter::walking_speed () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_WalkSpeed;
    }

    float ICharacter::running_speed () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_RunSpeed;
    }

    float ICharacter::berseker_speed () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_BerserkSpeed;
    }

    float ICharacter::current_speed () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return (getMovementState() == MOVEMENT_ACTION_RUN) ? m_RunSpeed : m_WalkSpeed;
    }

    bool ICharacter::selectTarget (uint32_t id)
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        m_sel_object = id;
        return true;
    }

    uint32_t ICharacter::getTarget () const
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        return m_sel_object;
    }

    void ICharacter::setImbue (const Imbue &imbue)
    {
        boost::mutex::scoped_lock lock(m_buff_mutex);

        m_Imbue = imbue;

        lock.unlock();

        calculateStats();
    }

    const Imbue& ICharacter::getImbue () const
    {
        boost::mutex::scoped_lock lock(m_buff_mutex);

        return m_Imbue;
    }

    std::pair<uint32_t,uint32_t> ICharacter::removeImbue (const uint32_t skillID, bool signal)
    {
        boost::mutex::scoped_lock lock(m_buff_mutex);

        std::pair<uint32_t,uint32_t> ret = std::make_pair(0,0);

        if (m_Imbue.skill && m_Imbue.skill->refID() == skillID)
        {
            ret = std::make_pair(m_Imbue.taskID,m_Imbue.castID);

            m_Imbue = Imbue();

            lock.unlock();

            if (signal)
                calculateStats();
        }

        return ret;
    }

    bool ICharacter::hasImbue () const
    {
        boost::mutex::scoped_lock lock(m_buff_mutex);

        return m_Imbue.skill;
    }

    void ICharacter::addStatus (const Status &status)
    {
        boost::mutex::scoped_lock lock(m_status_mutex);

        m_status_list.insert(std::make_pair(status.ID,status));

        m_EncStatus |= status.ID;
    }

    std::pair<uint32_t,uint32_t> ICharacter::removeStatus (uint32_t statusID)
    {
        boost::mutex::scoped_lock lock(m_status_mutex);

        std::pair<uint32_t,uint32_t> ret = std::make_pair(0,0);
        std::map<uint32_t,Status>::iterator it = m_status_list.find(statusID);

        if (it != m_status_list.end())
        {
            ret = std::make_pair(it->second.taskID,it->second.payloadTaskID);
            m_status_list.erase(it);
            m_EncStatus ^= statusID;
        }

        return ret;
    }

    std::pair<uint32_t,std::map<uint32_t,Status> > ICharacter::getStatus () const
    {
        boost::mutex::scoped_lock lock(m_status_mutex);

        return std::make_pair(m_EncStatus,m_status_list);
    }

    bool ICharacter::hasStatus (uint32_t mask) const
    {
        boost::mutex::scoped_lock lock(m_status_mutex);

        return m_EncStatus & mask;
    }

    void ICharacter::resetStatus ()
    {
        boost::lock_guard<boost::mutex> lock(m_status_mutex);

        m_EncStatus = 0;
        m_status_list.clear();
    }

    void ICharacter::addBuff (const Buff &buff)
    {
        assert(buff.skill->refID());

        if (buff.skill->hasBuffs())
        {
            boost::mutex::scoped_lock lock(m_buff_mutex);

            m_buff_list.insert(std::make_pair(buff.skill->refID(),buff));

            lock.unlock();

            calculateStats();
        }
    }

    std::pair<uint32_t,uint32_t> ICharacter::removeBuff (const uint32_t SkillID, bool signal)
    {
        boost::mutex::scoped_lock lock(m_buff_mutex);

        std::pair<uint32_t,uint32_t> ret = std::make_pair(0,0);
        std::map<uint32_t,Buff>::iterator i = m_buff_list.find(SkillID);

        if (i != m_buff_list.end())
        {
            ret = std::make_pair(i->second.taskID,i->second.CastID);

            m_buff_list.erase(i);

            lock.unlock();

            if (signal)
                calculateStats();
        }

        return ret;
    }

    std::vector<std::pair<uint32_t,uint32_t> > ICharacter::removeMinorBuffs (const Buff &buff)
    {
        boost::lock_guard<boost::mutex> lock(m_buff_mutex);

        std::vector<std::pair<uint32_t,uint32_t> > ret;

        /// REMOVE ALL BUFF THAT THIS BUFF CANCEL. !!ONLY FROM THE SAME GROUP!!
        for (std::map<uint32_t,Buff>::iterator i = m_buff_list.begin(); i != m_buff_list.end();)
        {
            if (!buff.persistent && !buff.skill->IsStackable(i->second.skill))
            {
                ret.push_back(std::make_pair(i->second.taskID,i->second.CastID));
                m_buff_list.erase(i++);
            }
            else
                ++i;
        }

        return ret;
    }

    bool ICharacter::IsBuffStackable (const Buff &buff) const
    {
        boost::lock_guard<boost::mutex> lock(m_buff_mutex);

        for (std::map<uint32_t,Buff>::const_iterator i = m_buff_list.begin(); i != m_buff_list.end(); ++i)
        {
            if (!i->second.skill->IsStackable(buff.skill))
                return false;
        }

        return true;
    }

    const std::map<uint32_t,Buff>& ICharacter::getBuffs () const
    {
        boost::lock_guard<boost::mutex> lock(m_buff_mutex);

        return m_buff_list;
    }

    int32_t ICharacter::getStatValue (const CHAR_STATS type) const
    {
        boost::lock_guard<boost::mutex> lock(m_stats_mutex);

        int32_t val = 0;
        std::map<CHAR_STATS,int32_t>::const_iterator it = m_Effects.find(type);

        if (it != m_Effects.end())
            val = it->second;

        return val;
    }

    void ICharacter::SetQueueSkill (const uint32_t skillID, const uint32_t targetID)
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        m_QueueSkill = skillID;
        m_QueueTarget = targetID;
    }

    uint32_t ICharacter::GetQueueSkill () const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        return m_QueueSkill;
    }

    uint32_t ICharacter::GetQueueTarget () const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        return m_QueueTarget;
    }

    bool ICharacter::IsSkillAvailable (const uint32_t skillID) const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        return m_cooldown_list.find(skillID) == m_cooldown_list.end();
    }

    void ICharacter::AddSkillCooldown (const uint32_t taskID, const uint32_t skillID)
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        m_cooldown_list.insert(std::make_pair(skillID,taskID));
    }

    void ICharacter::RemoveSkillCooldown (const uint32_t skillID)
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        m_cooldown_list.erase(skillID);
    }

    void ICharacter::SetCurrentTask (const uint8_t taskID)
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        m_CurrentTask = taskID;
    }

    uint8_t ICharacter::GetCurrentTask () const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        return m_CurrentTask;
    }

    bool ICharacter::IsTaskInterruptable () const
    {
        return true;
    }

    const Coord& ICharacter::getDestination () const
    {
        boost::mutex::scoped_lock lock(m_mov_mutex);

        return m_NextPos;
    }

    void ICharacter::setDestination (const Coord &pos)
    {
        boost::mutex::scoped_lock lock(m_mov_mutex);

        if (pos.get_zone())
        {
            float dx = pos.global_x() - m_CurrentPos.global_x();
            float dy = pos.global_y() - m_CurrentPos.global_y();

            m_Angle = atan2(dy,dx)*10430.38208;
        }

        m_NextPos = pos;
    }

    bool ICharacter::IsMoving () const
    {
        boost::mutex::scoped_lock lock(m_mov_mutex);

        return m_NextPos.get_zone();
    }

    void ICharacter::setSpeed (float wlk, float run, float bsk, bool signal)
    {
        boost::mutex::scoped_lock lock(m_stats_mutex);

        m_WalkSpeed = wlk;
        m_RunSpeed = run;
        m_BerserkSpeed = bsk;

        lock.unlock();

        if (signal && !signal_speed.empty())
            signal_speed(wlk,run);
    }

    STATE_STATUS_ACTION ICharacter::getStatusState () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_StatusState;
    }

    STATE_MOVEMENT_ACTION ICharacter::getStanceState () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_StanceState;
    }

    void ICharacter::changeStance ()
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        if (m_StanceState == MOVEMENT_ACTION_SIT)
            m_StanceState = MOVEMENT_ACTION_STAND;
        else
            m_StanceState = MOVEMENT_ACTION_SIT;

        if (!signal_state.empty())
            signal_state(STATE_MOVEMENT,m_StanceState);
    }

    void ICharacter::setMovementState (STATE_MOVEMENT_ACTION action)
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        m_MoveState = action;

        if (!signal_state.empty())
            signal_state(STATE_MOVEMENT,m_MoveState);

        float wlkSpeed = m_WalkSpeed;
        float runSpeed = m_RunSpeed;

        lock.unlock();

        if (!signal_speed.empty())
            signal_speed(wlkSpeed,runSpeed);
    }

    STATE_MOVEMENT_ACTION ICharacter::getMovementState () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_MoveState;
    }

    void ICharacter::setAttributeState (const STATE_ATTRIBUTE_ACTION action)
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        m_AttributeState = action;

        if (!signal_state.empty())
            signal_state(STATE_ATTRIBUTE,m_AttributeState);
    }

    STATE_ATTRIBUTE_ACTION ICharacter::getAttributeState () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_AttributeState;
    }

    void ICharacter::setAttackState (STATE_ATTACK_ACTION state)
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        m_AttackState = state;
    }

    STATE_ATTACK_ACTION ICharacter::getAttackState () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_AttackState;
    }

    void ICharacter::setKnockdown (bool state)
    {
        boost::mutex::scoped_lock lock(m_state_mutex);
        m_Knockdown = state;
    }

    bool ICharacter::IsKnockdown () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);
        return m_Knockdown;
    }

    bool ICharacter::IsDead () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_StatusState == STATUS_ACTION_DEAD;
    }

    bool ICharacter::IsBerserk () const
    {
        boost::mutex::scoped_lock lock(m_state_mutex);

        return m_AttributeState == ATTRIBUTE_ACTION_BERSERK;
    }

    void ICharacter::Morph (const boost::shared_ptr<EntityProperty> &property)
    {
        boost::mutex::scoped_lock lock(m_property_mutex);

        m_property = property;
    }

    bool ICharacter::addBackgroundTask (const uint16_t taskType, const uint32_t taskID)
    {
        boost::lock_guard<boost::mutex> lock(m_bg_mutex);

        for (size_t i = 0; i < m_bg_tasks.size(); ++i)
        {
            if (m_bg_tasks[i].first == taskType)
                return false;
        }

        m_bg_tasks.push_back(std::make_pair(taskType,taskID));

        return true;
    }

    uint32_t ICharacter::removeBackgroundTask (const uint16_t taskType)
    {
        typedef std::vector<std::pair<uint16_t,uint32_t> >::iterator bgiterator;

        uint32_t taskID = 0;

        boost::lock_guard<boost::mutex> lock(m_bg_mutex);

        for (bgiterator it = m_bg_tasks.begin(); it != m_bg_tasks.end(); ++it)
        {
            if (it->first == taskType)
            {
                taskID = it->second;
                m_bg_tasks.erase(it);
                break;
            }
        }

        return taskID;
    }

    bool ICharacter::hasBackgroundTask (const uint16_t taskType) const
    {
        boost::lock_guard<boost::mutex> lock(m_bg_mutex);

        for (size_t i = 0; i < m_bg_tasks.size(); ++i)
        {
            if (m_bg_tasks[i].first == taskType)
                return true;
        }

        return false;
    }

    void ICharacter::calculateStats ()
    {
        boost::unique_lock<boost::mutex> statsLock(m_stats_mutex);

        boost::mutex::scoped_lock property_lock(m_property_mutex);

        /// SET BASE STATS AND RECALCULATE
        m_MA = std::make_pair(0,0);
        m_PA = std::make_pair(0,0);
        m_PAbs = 0;
        m_MAbs = 0;
        m_PD = m_property->PhysicalDefense;
        m_MD = m_property->MagicalDefense;
        m_BR = m_property->BlockRatio;
        m_CR = 0;
        m_PR = m_property->ParryRatio;
        m_HR = m_property->HitRatio;
        m_PBalance = 0;
        m_MBalance = 0;
        m_PRate = 0;
        m_MRate = 0;
        m_AD = 0;

        float wlkspeed = m_property->WalkSpeed;
        float runspeed = m_property->RunSpeed;
        float bskspeed = m_property->BersekSpeed;

        property_lock.unlock();

        bool hasWall = false;
        uint32_t curWallHP = m_Effects[STAT_WALL_HP];

        m_Effects.clear();

        short speed_percent = 0;
        short hp_percent = 0;
        short mp_percent = 0;
        short pr_percent = 0;
        short hr_percent = 0;
        short pd_percent = 0;
        short md_percent = 0;

        std::vector<std::pair<CHAR_STATS,int32_t> > bonus;

        calculatePreStats(bonus);

        boost::mutex::scoped_lock buff_lock(m_buff_mutex);

        for ( std::map<uint32_t,Buff>::const_iterator k = m_buff_list.begin(); k != m_buff_list.end(); ++k)
        {
            for (Skill::buff_const_iterator j = k->second.skill->buff_begin(); j != k->second.skill->buff_end(); ++j)
            {
                switch(j->ID)
                {
                case BUFF_CRITICAL_INC:
                    m_CR += j->Arg[0];
                    break;
                case BUFF_SPEED_INC:
                case BUFF_SPEED_INC2:
                case BUFF_SPEED_INC3:
                    speed_percent += j->Arg[0];
                    break;
                case BUFF_PARRY_INC:
                    m_PR += j->Arg[0];
                    pr_percent += j->Arg[1];
                    break;
                case BUFF_PARRY_DEC:
                    m_PR -= j->Arg[0];
                    break;
                case BUFF_HIT_INC:
                    m_HR += j->Arg[0];
                    hr_percent += j->Arg[1];
                    break;
                case BUFF_HIT_DEC:
                    m_HR -= j->Arg[0];
                    break;
                case BUFF_HP_INC:
                    m_MaxHP += j->Arg[0];
                    hp_percent += j->Arg[1];
                    break;
                case BUFF_MP_INC:
                    m_MaxMP += j->Arg[0];
                    mp_percent += j->Arg[1];
                    break;
                case BUFF_RANGE_INC:
                    m_AD += j->Arg[0];
                    break;
                case BUFF_DAMAGE_INC:
                    m_PRate += j->Arg[0];
                    m_MRate += j->Arg[0];
                    break;
                case BUFF_INT_INC:
                case BUFF_STR_INC:
                case BUFF_SHIELD_PWR_4_DAMAGE:
                    calculateBuffEffects(bonus,*j);
                    break;
                case BUFF_REINCARNATION:
                    m_Effects[STAT_HP_RGN] += j->Arg[0];
                    m_Effects[STAT_MP_RGN] += j->Arg[1];
                    break;
                case BUFF_IGNORE_DEFENSE:
                    m_Effects[STAT_IGD] += j->Arg[0];
                    break;
                case BUFF_CRIT_PARRY_INC:
                    m_Effects[STAT_CPR] += j->Arg[0];
                    break;
                case BUFF_GOLD_DROP_INC:
                    m_Effects[STAT_GOLD_DROP] += j->Arg[0];
                    break;
                case BUFF_MAGICOPTION_LUCK_INC:
                    m_Effects[STAT_MOL] += j->Arg[0];
                    break;
                case BUFF_ENCHANT_LUCK_INC:
                    m_Effects[STAT_ETL] += j->Arg[0];
                    break;
                case BUFF_MP_ABSORB:
                    m_Effects[STAT_MP_ABSORB] += j->Arg[0];
                    break;
                case BUFF_DAMAGE_PWR_INC:
                    m_PA.first += j->Arg[0];
                    m_PA.second += j->Arg[0];

                    m_MA.first += j->Arg[1];
                    m_MA.second += j->Arg[1];
                    break;
                case BUFF_DETECT:
                case BUFF_DETECT_V2:
                    //(5 Stealth) (6 Invisible) (7 All)
                    //Max level
                    break;
                case BUFF_HEAL_INC:
                    m_Effects[STAT_HP_HEAL] += j->Arg[0];
                    m_Effects[STAT_MP_HEAL] += j->Arg[1];
                    break;
                case BUFF_ABSORB_WALL:
                    hasWall = true;
                    m_Effects[STAT_WALL] = j->Arg[0] == WALL_TYPE_PHYSICAL;
                    m_Effects[STAT_WALL_MAX] = j->Arg[1];
                    m_Effects[STAT_WALL_RATE] = j->Arg[3];
                    m_Effects[STAT_WALL_ID] = k->first;
                    break;
                case BUFF_BLOCK_INC:
                    //BLOCK TYPE +0 (15 ALL) (11 Magic %) (7 Physical %)
                    //BLOCK RATE +1
                    m_BR += j->Arg[1];
                    break;
                case BUFF_DAMAGE_REFLECT:
                    //PROBABILITY [%]
                    //PHY DAMAGE RETURNED [%]
                    //MAG DAMAGE RETURNED [%]
                    //RETURN RANGE
                    break;
                case BUFF_ABSORB:
                    //TYPE (3 - Physical Absorb) (12 & 15 - Absorb all) (4 Physical) (8 Magical) (6 Physical ranged)
                    //Amount (%)
                    //0
                    break;
                case BUFF_DEFENSE_INC:
                    m_PD += j->Arg[0];
                    m_MD += j->Arg[1];
                    //Caster Range
                    break;
                case BUFF_DEFENSE_DEC:
                    pd_percent -= j->Arg[0];
                    md_percent -= j->Arg[1];
                    break;
                case BUFF_DAMAGE_DEC:
                    m_PRate -= j->Arg[0];
                    m_MRate -= j->Arg[1];
                    break;
                case BUFF_HP_DEC:
                    //Duration
                    //0
                    hp_percent -= j->Arg[2];
                    //2
                    break;
                default:
                    break;
                }
            }
        }

        if (hasWall)
        {
            uint32_t wallMax = m_Effects[STAT_WALL_MAX];

            if (curWallHP > wallMax)
                m_Effects[STAT_WALL_HP] = wallMax;
        }

        buff_lock.unlock();

        for (std::vector<std::pair<CHAR_STATS,int32_t> >::const_iterator it = bonus.begin(); it != bonus.end(); ++it)
        {
            switch(it->first)
            {
            case STAT_HP_PERCENT:
                hp_percent += it->second;
                break;
            case STAT_HR_PERCENT:
                hr_percent += it->second;
                break;
            case STAT_MP_PERCENT:
                mp_percent += it->second;
                break;
            case STAT_PR_PERCENT:
                pr_percent += it->second;
                break;
            default:
                break;
            }
        }

        calculatePostStats();

        if (hp_percent)
            m_MaxHP = linear_regression(m_MaxHP,hp_percent);

        if (m_HP > m_MaxHP)
            m_HP = m_MaxHP;

        if (mp_percent)
            m_MaxMP = linear_regression(m_MaxMP,mp_percent);

        if (m_MP > m_MaxMP)
            m_MP = m_MaxMP;

        if (pr_percent)
            m_PR = linear_regression(m_PR,pr_percent);

        if (hr_percent)
            m_HR = linear_regression(m_HR,hr_percent);

        if (pd_percent)
            m_PD = linear_regression(m_PD,pd_percent);

        if (md_percent)
            m_MD = linear_regression(m_MD,md_percent);

        if (speed_percent)
        {
            wlkspeed = linear_regression(wlkspeed,speed_percent);
            runspeed = linear_regression(runspeed,speed_percent);
            bskspeed = linear_regression(bskspeed,speed_percent);
        }

        if (IsBerserk())
        {
            wlkspeed *=2;
            runspeed *=2;
            bskspeed *=2;

            m_PA.first *= 2;
            m_PA.second *= 2;
            m_MA.first *= 2;
            m_MA.second *= 2;
        }

        m_WalkSpeed = wlkspeed;
        m_RunSpeed = runspeed;
        m_BerserkSpeed = bskspeed;

        calculateStatus();

        statsLock.unlock();

        if (!signal_stats.empty())
            signal_stats();

        if (!signal_speed.empty())
            signal_speed(m_WalkSpeed,m_RunSpeed);
    }

    void ICharacter::calculatePreStats (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus)
    {
    }

    void ICharacter::calculateBuffEffects (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus,
                                    const BuffEffect &effect)
    {
    }

    void ICharacter::calculatePostStats ()
    {
    }

    void ICharacter::calculateStatus ()
    {
        uint8_t ratio = 0;

        boost::lock_guard<boost::mutex> lock(m_status_mutex);

        std::map<uint32_t,Status>::const_iterator it;

        for (it = m_status_list.begin(); it != m_status_list.end(); ++it)
        {
            switch(it->first)
            {
            case STATUS_SHOCK:
                if (m_PR < it->second.Payload)
                    m_PR = 0;
                else
                    m_PR -= it->second.Payload;
                break;
            case STATUS_FROST:
            case STATUS_DULL:
                m_WalkSpeed *=0.5;
                m_RunSpeed *=0.5;
                m_BerserkSpeed *=0.5;
                break;
            case STATUS_WEAKEN:
                if (m_MD < it->second.Payload)
                    m_MD = 0;
                else
                    m_MD -= it->second.Payload;
                break;
            case STATUS_DECAY:
                if (m_PD < it->second.Payload)
                    m_PD = 0;
                else
                    m_PD -= it->second.Payload;
                break;
            case STATUS_DARKNESS:
                if (m_HR < it->second.Payload)
                    m_HR = 0;
                else
                    m_HR -= it->second.Payload;
                break;
            case STATUS_IMPOTENT:
                ratio = (1-it->second.Payload/100);
                m_PA.first *= ratio;
                m_PA.second *= ratio;
                m_MA.first *= ratio;
                m_MA.second *= ratio;
                break;
            default:
                break;
            }
        }
    }
}
