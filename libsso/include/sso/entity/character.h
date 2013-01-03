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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_H__

#include <set>
#include <utility>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

#include <khamaileon/property/character_type.h>
#include <sso/entity/character_state.h>
#include <sso/entity/entity.h>
#include <sso/skill/status.h>
#include <sso/skill/buff.h>
#include <sso/skill/imbue.h>

class EntityProperty;

namespace sso
{
    enum CHAR_STATS
    {
        STAT_STR,
        STAT_INT,
        STAT_WALL,
        STAT_WALL_HP,
        STAT_WALL_MAX,
        STAT_WALL_RATE,
        STAT_WALL_ID,
        STAT_HP_RGN,		/// HP REGENERATION
        STAT_MP_RGN,		/// MP REGENRATION
        STAT_IGD,			/// IGNORE NPC DEFENSE
        STAT_CPR,			/// CRITICAL PARRY RATIO
        STAT_MOL,			/// MAGICOPTION LUCK
        STAT_ETL,			/// ENCHANT LUCK (+)
        STAT_HP_HEAL,
        STAT_MP_HEAL,
        STAT_GOLD_DROP,
        STAT_MP_ABSORB,
        STAT_HP_PERCENT,
        STAT_MP_PERCENT,
        STAT_PR_PERCENT,
        STAT_HR_PERCENT
    };

    class ICharacter : public Entity
    {
    public:

        boost::signals2::signal<void ()> signal_stats;

        boost::signals2::signal<void (uint8_t,uint8_t)> signal_state;

        boost::signals2::signal<void (float,float)> signal_speed;

        boost::signals2::signal<void (uint32_t,const Coord&)> signal_dead;

    public:

        typedef std::pair<uint32_t,uint32_t> attack_type;

        typedef std::map<SKILL_BUFF,Buff>::iterator buff_iterator;

    public:

        ICharacter (const boost::shared_ptr<EntityProperty> &property);

        virtual ~ICharacter ();

        /**
         *
         *	@brief Get character base properties.
         *
         **/

        boost::shared_ptr<EntityProperty> getProperty () const;

        /**
         *
         *	@brief Get character refID.
         *
         **/

        uint32_t getRefID () const;

        /**
         *
         *	@brief Get character type.
         *
         **/

        ENTITY_TYPE getType () const;

        /**
         *
         *	@brief Get character current level.
         *
         **/

        virtual uint8_t getLevel () const = 0;

        /**
         *
         *	@brief Get character race.
         *
         **/

        RACE getRace () const;

        /**
         *
         *	@brief Get character gender.
         *
         **/

        GENDER getGender () const;

        /**
         *
         *  @brief: Retrive current hit points.
         *
         *	@retval Hit points.
         *
         **/

        uint32_t HP () const;

        /**
         *
         *	@brief Retrieve maximum hit points.
         *
         *	@retval Max hit points.
         *
         **/

        uint32_t maxHP () const;

        /**
         *
         *	@brief: Retrieve hit points in percentage (From 0 to 10 scale).
         *
         *	@retval Hit points (%).
         *
         **/

        uint8_t percentHP () const;

        /**
         *
         *	@brief Set current hit points.
         *
         *	@param value: Desired hit points (Must be lower or equal to maxHP)
         *
         **/

        void setHP (const uint32_t value);

        /**
         *
         *	@brief Heal character hit points by certain amount.
         *
         *	@param amount: Amount of hit points to be healed.
         *
         **/

        void healHP (const uint32_t amount);

        /**
         *
         *	@brief Consume character hit points by certain amount.
         *
         *	@param amount: Amount of hit points to consume.
         *
         **/

        void consumeHP (const uint32_t amount);

        void inc_aggro (const uint32_t aggro, const uint32_t attacker_id);

        void receive_damage (const uint32_t damage, const uint32_t attacker_id);

        std::multimap<uint32_t,uint32_t> getDamageList () const;

        /**
         *
         *  @brief: Retrieve current Magic Points.
         *
         *	@retval Current character magic points.
         *
         **/

        uint32_t MP () const;

        /**
         *
         *	@brief Retrieve character maximum magic points.
         *
         *	@retval Max MP.
         *
         **/

        uint32_t maxMP () const;

        /**
         *
         *	@brief Retrieve character current magic points in percentage. (From 0 to 10 scale).
         *
         *	@retval MP (%)
         *
         **/

        uint8_t percentMP () const;

        /**
         *
         *	@brief Heal character magic points by certain amount.
         *
         *	@param amount: Amount of magic points to heal.
         *
         **/

        void healMP (const uint32_t amount);

        /**
         *
         *	@brief Consume character magic points by a certain amount.
         *
         *	@param amount: Magic points amount to consume.
         *
         **/

        void consumeMP (const uint32_t amount);

        /**
         *
         *	@brief Character current physical defense.
         *
         **/

        uint16_t physical_defense () const;

        /**
         *
         *	@brief Character current magical defense.
         *
         **/

        uint16_t magical_defense () const;

        /**
         *
         *	@brief Character current physical absorb.
         *
         **/

        float physical_absorb () const;

        /**
         *
         *	@brief Character current magical absorb.
         *
         **/

        float magical_absorb () const;

        float physical_rate () const;

        float magical_rate () const;

        /**
         *
         *	@brief Character current physical balance.
         *
         **/

        float physical_balance () const;

        /**
         *
         *	@brief Character current magical balance.
         *
         **/

        float magical_balance () const;

        /**
         *
         *	@brief Character current hit ratio.
         *
         **/

        uint16_t hit_ratio () const;

        /**
         *
         *	@brief Character current parry ratio.
         *
         **/

        uint16_t parry_ratio () const;

        /**
         *
         *	@brief Character attack distance (Mainly weapons+buffs).
         *
         **/

        float attack_distance () const;

        /**
         *
         *	@brief Character current physical attack [min,max].
         *
         **/

        attack_type physical_attack () const;

        /**
         *
         *	@brief Character current magical attack [min,max].
         *
         **/

        attack_type magical_attack () const;

        /**
         *
         *	@brief Character current critical ratio.
         *
         **/

        uint16_t critical_ratio () const;

        /**
         *
         *	@brief Character current block ratio.
         *
         **/

        uint16_t block_ratio () const;

        /**
         *
         *	@brief Character current walking speed.
         *
         **/

        float walking_speed () const;

        /**
         *
         *	@brief Character current running speed.
         *
         **/

        float running_speed () const;

        /**
         *
         *	@brief Character current berserk speed.
         *
         **/

        float berseker_speed () const;

        /**
         *
         *	@brief Character current speed, if its running (run speed), else walk speed.
         *
         **/

        float current_speed () const;

        /**
         *
         *  @brief Set current target. The object must be in the observer list.
         *
         *  @param objid Object ID to set as target.
         *
         **/

        bool selectTarget (uint32_t objid);

        /**
         *
         *	@brief Retrieve current selected target GUID.
         *
         **/

        uint32_t getTarget () const;

        /**
         *
         *	@brief Set character imbue skill.
         *
         *	@param imbue: Imbue skill to set.
         *
         **/

        void setImbue (const Imbue &imbue);

        /**
         *
         *	@brief Obtain current imbue skill.
         *
         **/

        const Imbue& getImbue () const;

        /**
         *
         *  @brief Remove a imbue from entity list.
         *
         *  @param SkillID: Originated skill.
         *  @param signal: Flag to update and signal stats.
         *
         *  @retval Pair associated taskID, associated castID.
         *
         **/

        std::pair<uint32_t,uint32_t> removeImbue (const uint32_t skillID, bool signal = true);

        /**
         *
         *	@brief Check if the character have an imbue skill activated.
         *
         *	@retval True if it has, false otherwise.
         *
         **/

        bool hasImbue () const;

        /**
         *
         *	@brief Add a new status to the character.
         *
         *	@param status: Status to add.
         *
         **/

        void addStatus (const Status &status);

        /**
         *
         *	@brief Remove a status already present in the character.
         *
         *	@param statusID: Status refID to remove.
         *
         **/

        std::pair<uint32_t,uint32_t> removeStatus (uint32_t statusID);

        /**
         *
         *	@brief Get character current status list.
         *
         *	@retval [status mask,list].
         *
         **/

        std::pair<uint32_t,std::map<uint32_t,Status> > getStatus () const;

        /**
         *
         *  @brief Check if the selected status are activated.
         *
         **/

        bool hasStatus (uint32_t mask) const;

        /**
         *
         *  @brief Remove all the status of the player.
         *
         **/

        void resetStatus ();

        /**
         *
         *	@brief Add a new buff to the character and recalculate stats.
         *
         *	@param buff: Buff to add.
         *
         **/

        void addBuff (const Buff &buff);

        /**
         *
         *  @brief Remove a buff from entity list.
         *
         *  @param SkillID: Originated skill.
         *  @param signal: Flag to update and signal stats.
         *
         *  @retval Pair associated taskID, associated castID.
         *
         **/

        std::pair<uint32_t,uint32_t> removeBuff (const uint32_t SkillID, bool signal = true);

        std::vector<std::pair<uint32_t,uint32_t> > removeMinorBuffs (const Buff &buff);

        /**
         *
         *	@brief Check if a buff is stackable with the character current active buffs.
         *
         *	@param buff: Buff to check.
         *
         *	@retval True if its stackable, false otherwise.
         *
         **/

        bool IsBuffStackable (const Buff &buff) const;

        /**
         *
         *	@brief Character current buff list.
         *
         **/

        const std::map<uint32_t,Buff>& getBuffs () const;

        /**
         *
         *	@brief Retrieve desired current stat value.
         *
         *	@param type: Stat type to retrieve.
         *
         *	@retval Stat current value.
         *
         **/

        int32_t getStatValue (const CHAR_STATS type) const;

        /**
         *
         *	@brief Calculate/Update character stats and buff effects.
         *
         **/

        void calculateStats ();

        /**
         *
         *  @brief Add a skill into the skill queue for later casting.
         *
         **/

        void SetQueueSkill (const uint32_t skillID, const uint32_t targetID);

        /**
         *
         *  @brief Get queue skill for casting.
         *
         **/

        uint32_t GetQueueSkill () const;

        /**
         *
         *	@brief Get queue target refID.
         *
         **/

        uint32_t GetQueueTarget () const;

        /**
         *
         *  @brief Indicates if a skill is available for use by the player or not.
         *
         *  @param skillID ID to lookup.
         *
         *  @retval active status.
         *
         **/

        bool IsSkillAvailable (const uint32_t skillID) const;

        /**
         *
         *  @brief Insert a skill into cooldown list.
         *
         *  @param taskID Associated task ID in the world queue.
         *  @param skillID skill to add.
         *
         **/

        void AddSkillCooldown (const uint32_t taskID, const uint32_t skillID);

        /**
         *
         *  @brief Remove a skill from the cooldown list.
         *
         *  @param skillID skill to remove.
         *
         **/

        void RemoveSkillCooldown (const uint32_t skillID);

        /**
         *
         *	@brief Set character current task.
         *
         **/

        void SetCurrentTask (const uint8_t taskID);

        /**
         *
         *	@brief Get character current task.
         *
         **/

        uint8_t GetCurrentTask () const;

        /**
         *
         *	@brief Check if the current task is interruptable or not.
         *
         **/

        virtual bool IsTaskInterruptable () const;

        /**
         *
         *	@brief Set object destination position.
         *
         **/

        void setDestination (const Coord &pos);

        /**
         *
         *	@brief Get character current destination position.
         *
         **/

        const Coord& getDestination () const;

        /**
         *
         *	@brief Check if the character is moving or not.
         *
         *	@retval True if its moving, false otherwise.
         *
         **/

        bool IsMoving () const;

        /**
         *
         *	@brief Set character speed.
         *
         *	@param wlk: Walking speed.
         *	@param run: Running speed.
         *	@param bsrk: Berserk speed.
         *	@param signal: Flag to signal for update.
         *
         **/

        void setSpeed (float wlk, float run, float bsrk, bool signal = true);

        /**
         *
         *  @brief: Indicates object current state.
         *
         *  @retval: if the object is alive or dead.
         *
         **/

        STATE_STATUS_ACTION getStatusState () const;

        /**
         *
         *  @brief: Indicate the object current stance state.
         *
         *  @retval: if the object is standing or sitting.
         *
         **/

        STATE_MOVEMENT_ACTION getStanceState () const;

        /**
         *
         *	@brief Change character current stance, from sitting to standing and viceversa.
         *
         **/

        void changeStance ();

        /**
         *
         *	@brief Set character movement state (walking or running).
         *
         **/

        void setMovementState (const STATE_MOVEMENT_ACTION action);

        /**
         *
         *  @brief: Indicate the object current move state.
         *
         *  @return: if the object is running or walking.
         *
         **/

        STATE_MOVEMENT_ACTION getMovementState () const;

        /**
         *
         *	@brief Set character attribute state (berserk,invunerable or normal).
         *
         *	@param action: Desired state.
         *
         **/

        void setAttributeState (const STATE_ATTRIBUTE_ACTION action);

        /**
         *
         *  @brief: Indicate the object current attribute state.
         *
         *  @retval: if the object is berserking, invunerable or normal.
         *
         **/

        STATE_ATTRIBUTE_ACTION getAttributeState () const;

        /**
         *
         *	@brief Set character attack state (none,attacking).
         *
         **/

        void setAttackState (STATE_ATTACK_ACTION state);

        /**
         *
         *  @brief: Indicate the object current attack state.
         *
         *  @retval: if the object is attacking or not.
         *
         **/

        STATE_ATTACK_ACTION getAttackState () const;

        /**
         *
         *	@brief Set character knockdown state.
         *
         *	@param state: True for knockdown, false standing.
         *
         **/

        void setKnockdown (bool state);

        /**
         *
         *	@brief Check if the character is knockdown or not.
         *
         *	@retval: True its currently knockdown, false otherwise.
         *
         **/

        bool IsKnockdown () const;

        /**
         *
         *  @brief: Check the death state.
         *
         *  @retval: true if is dead else returns false.
         *
         **/

        bool IsDead () const;

        /**
         *
         *	@brief check if the entity is on berserk.
         *
         *	@retval current berserker state.
         *
         **/

        bool IsBerserk () const;

        /**
         *
         *	@brief: Morph entity into another changing all its base attributes.
         *
         *	@param property: Selected property to update.
         *
         **/

        void Morph (const boost::shared_ptr<EntityProperty> &property);

        /**
         *
         *	@brief Add a new background task for the character.
         *
         *	@param taskType: Task type to add.
         *	@param taskID: Task refID assigned.
         *
         *	@retval True if success on adding the task, false theres already a task with that type.
         *
         **/

        bool addBackgroundTask (const uint16_t taskType, const uint32_t taskID);

        /**
         *
         *	@brief Remove an active background task.
         *
         *	@param taskType: Task type to remove.
         *
         *	@retval Assigned taskID, 0 if the task dont exists.
         *
         **/

        uint32_t removeBackgroundTask (const uint16_t taskType);

        /**
         *
         *	@brief Check if a task is currently active.
         *
         *	@param taskType: Task type to check for.
         *
         *	@retval True if the task is active, false otherwise.
         *
         **/

        bool hasBackgroundTask (const uint16_t taskType) const;

    protected:

        /**
         *
         *	@brief Calculate character base stats based on other attributes besides buffs.
         *	(Override if needed).
         *
         *	@param bonus Processed bonus list for appending new bonus (ID,Value).
         *
         **/

        virtual void calculatePreStats (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus);

        /**
         *
         *	@brief Calculate/Update character stats that depends on the other stats.
         *	(Override if needed).
         *
         **/

        virtual void calculatePostStats ();

        /**
         *
         *	@brief Calculate/Update character stats that required other information not available here.
         *
         *	@param bonus Processed bonus list for appending new bonus (ID,Value).
         *	@param effect Buff effect to process.
         *
         **/

        virtual void calculateBuffEffects (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus,
                                           const BuffEffect &effect);

        /**
         *
         *	@brief Calculate/Update character bad status effects.
         *
         **/

        void calculateStatus ();

    protected:

        mutable boost::mutex m_property_mutex;
        boost::shared_ptr<EntityProperty> m_property;

        mutable boost::mutex m_stats_mutex;     /// MUTEX FOR ATTACK,STATS,SKILLS,DAMAGE AND RELATED

        uint32_t m_sel_object;

        float m_AD;					/// ATTACK DISTANCE
        attack_type m_PA;			/// PHYSICAL ATTACK
        attack_type m_MA;			/// MAGICAL ATTACK
        uint32_t m_HP;				/// HIT POINTS
        uint32_t m_MaxHP;			/// MAX HIT POINTS
        uint32_t m_MP;				/// MANA POINTS
        uint32_t m_MaxMP;			/// MAX MANA POINTS
        uint16_t m_PD;				/// PHYSICAL DEFENSE
        uint16_t m_MD;				/// MAGICAL DEFENSE
        uint16_t m_HR;				/// HIT RATIO
        uint16_t m_PR;				/// PARRY RATIO
        uint16_t m_CR;				/// CRITICAL RATIO
        uint16_t m_BR;				/// BLOCK RATIO
        float m_PAbs;               /// PHYSICAL ABSORB (VALUES FROM 0 TO 100)
        float m_MAbs;               /// MAGICAL ABSORB (VALUES FROM 0 TO 100)
        float m_PRate;				/// PHYSICAL INCREASE RATE
        float m_MRate;				/// MAGICAL INCREASE RATE
        float m_PBalance;			/// PHYSICAL BALANCE
        float m_MBalance;			/// MAGICAL BALANCE
        float m_WalkSpeed;
        float m_RunSpeed;
        float m_BerserkSpeed;

        std::map<CHAR_STATS,int32_t> m_Effects;	/// BUFF BONUS STATS

        uint32_t m_AttackTaskID;            /// taskID where attack state timeout is being tracked

        mutable boost::mutex m_state_mutex;

        STATE_STATUS_ACTION m_StatusState;
        STATE_ATTRIBUTE_ACTION m_AttributeState;
        STATE_ATTACK_ACTION m_AttackState;
        STATE_MOVEMENT_ACTION m_MoveState;      ///WALK OR RUN
        STATE_MOVEMENT_ACTION m_StanceState;    ///STAND OR SIT

        bool m_Knockdown;

        std::map<uint32_t,uint32_t> m_aggro_list;
        std::map<uint32_t,uint32_t> m_damage_list;

        uint8_t m_Level;

        mutable boost::mutex m_status_mutex;

        uint32_t m_EncStatus;
        std::map<uint32_t,Status> m_status_list;

        mutable boost::mutex m_buff_mutex;

        Imbue m_Imbue;
        std::map<uint32_t,Buff> m_buff_list;

        mutable boost::mutex m_queue_mutex;

        uint32_t m_QueueSkill;
        uint32_t m_QueueTarget;
        std::map<uint32_t,uint32_t> m_cooldown_list;

        mutable boost::mutex m_bg_mutex;
        std::vector<std::pair<uint16_t,uint32_t> > m_bg_tasks;

        /// MOVEABLE OBJECT MEMBER
        Coord m_NextPos;

        uint8_t m_CurrentTask;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_H__
