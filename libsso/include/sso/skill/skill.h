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

#ifndef __ZELOS_LIBSSO_SSO_SKILL_SKILL_H__
#define __ZELOS_LIBSSO_SSO_SKILL_SKILL_H__

#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include <khamaileon/property/skill.h>
#include <sso/types.h>
#include <sso/skill/requirement.h>
#include <sso/skill/status.h>
#include <sso/skill/buff.h>
#include <sso/skill/attack.h>

namespace sso
{
    enum ESSENTIAL_ITEM_TYPE
    {
        ESS_SHIELD = 4,
        ESS_WEAPON = 6,
        ESS_ARMOR = 10,
        ESS_DEVIL = 14
    };

    enum SKILL_TYPE
    {
        SKT_PASSIVE,
        SKT_IMBUE,
        SKT_ATTACK,
        SKT_NONE
    };

    enum SKILL_ACTION_TYPE
    {
        SKILL_ACTION_ATTACK,
        SKILL_ACTION_BUFF,
        SKILL_ACTION_SUMMON,
        SKILL_ACTION_TELEPORT,
        SKILL_ACTION_HEAL,
        SKILL_ACTION_RESURRECT,
        SKILL_ACTION_UNDEFINED
    };

    enum SKILL_BASE_ATTACKS
    {
        SKILL_PUNCH_01 = 1,
        SKILL_CH_SWORD_BASE_01 = 2,
        SKILL_CH_SPEAR_BASE_01 = 40,
        SKILL_CH_BOW_BASE_01 = 70,
        SKILL_EU_SWORD_BASE_01 = 7125,
        SKILL_EU_TSWORD_BASE_01 = 7126,
        SKILL_EU_AXE_BASE_01 = 7127,
        SKILL_EU_CROSSBOW_BASE_01 = 7887,
        SKILL_EU_DAGGER_BASE_01 = 7888,
        SKILL_EU_STAFF_BASE_01 = 8368,
        SKILL_EU_WAND_WARLOCK_BASE_01 = 8899,
        SKILL_EU_HARP_BASE_01 = 9426,
        SKILL_EU_WAND_CLERIC_BASE_01 = 9784
    };

    enum SKILL_CAST_CONDITION
    {
        SKILL_CONDITION_NONE,
        SKILL_CONDITION_KNOCKDOWN = 1,
        SKILL_CONDITION_TAMBOUR = 32
    };

    enum SKILL_EFFECT
    {
        SK_NONE,
        SK_DURATION,
        SK_HIT_COUNT,
        SK_ABSOLUTE_DMG,
        SK_AGGRO_INC,
        SK_AGGRO_INC_RATIO,
        SK_AGGRO_DEC,
        SK_AGGRO_DEC_RATIO,
        SK_CAST_CONDITION,
        SK_KB,
        SK_KB_DIST,
        SK_KD,
        SK_KD_LEVEL,
        SK_KD_BONUS,
        SK_HP_HEAL,
        SK_HP_CONVERT_RATIO,
        SK_MP_HEAL,
        SK_MP_CONSUME,
        SK_MP_CONSUME_TIME,
        SK_MP_CONVERT_RATIO,
        SK_TELEPORT_DIST,
        SK_REFLECT_PHY,
        SK_REFLECT_MAG,
        SK_TARGET_LOCK
    };

    struct NPC_SPAWN_INFO
    {
        uint32_t refCharID;
        uint8_t GiantCount;
        uint8_t ChampionCount;
        uint8_t NormalCount;
    };

    class SkillBuilder;

    class Skill : public boost::noncopyable
    {
    public:

        typedef std::vector<BuffEffect>::const_iterator buff_const_iterator;
        typedef std::vector<BuffEnd>::const_iterator ebuff_const_iterator;
        typedef std::vector<Status>::const_iterator status_const_iterator;
        typedef std::vector<NPC_SPAWN_INFO>::const_iterator spawn_const_iterator;
        typedef std::map<SKILL_EFFECT,uint32_t>::const_iterator effect_const_iterator;

    public:

        friend class SkillBuilder;

        Skill (const SkillProperty& property);

        ~Skill ();

        /**
         *
         *	@brief Skill reference ID.
         *
         **/

        uint32_t refID () const;

        /**
         *
         *	@brief Skill group reference ID where this skill belongs.
         *
         **/

        uint16_t refGroupID () const;

        /**
         *
         *	@brief Global skill type.
         *
         **/

        SKILL_TYPE Type () const;

        /**
         *
         *	@brief Skill level.
         *
         **/

        uint8_t Level () const;

        /**
         *
         *	@brief Next skill that need to be casted (Chained skills).
         *
         **/

        uint32_t nextSkill () const;

        /**
         *
         *	@brief Required skill points to activate the skill.
         *
         **/

        uint32_t SP () const;

        /**
         *
         *	@brief Race where this skills belongs to.
         *
         **/

        RACE Race () const;

        /**
         *
         *	@brief Required mastery to activate this skill.
         *
         **/

        uint16_t MasteryID () const;

        /**
         *
         *	@brief Minimum mastery level required to activate/use this skill.
         *
         **/

        uint8_t MasteryLv () const;

        /**
         *
         *	@brief First Skill group required to activate this skill.
         *
         **/

        uint16_t GroupID1 () const;

        /**
         *
         *	@brief First skill group required level to activate this skill.
         *
         **/

        uint8_t GroupLv1 () const;

        /**
         *
         *	@brief Second Skill group required to activate this skill.
         *
         **/

        uint16_t GroupID2 () const;

        /**
         *
         *	@brief Second skill group required level to activate this skill.
         *
         **/

        uint8_t GroupLv2 () const;

        /**
         *
         *	@brief Mana points required to cast the skill (Consumed by the caster).
         *
         *	@retval MP consumed.
         *
         **/

        uint32_t MP () const;

        /**
         *
         *	@brief Minimum HP(%) required to cast the skill (Consumed by the caster).
         *
         *	@retval HP(%)
         *
         **/

        uint8_t MinHPRatio () const;


        /**
         *
         *	@brief Maximum HP(%) allowed to cast the skill.
         *
         *	@retval HP(%)
         *
         **/

        uint8_t MaxHPRatio () const;

        /**
         *
         *	@brief Initial animation sequence duration [ms].
         *
         **/

        uint16_t Init () const;

        /**
         *
         *	@brief Delay time between animations [ms].
         *
         **/

        uint16_t Delay () const;

        /**
         *
         *	@brief Waiting time before performing another animation [ms].
         *
         **/

        uint16_t nextDelay () const;

        /**
         *
         *	@brief Cooldown time before being able to cast the skill again [ms].
         *
         **/

        uint16_t Cooldown () const;

        /**
         *
         *	@brief Skill stacking refID tag.
         *
         **/

        uint32_t StackID () const;

        SKILL_ACTION_TYPE ActionType () const;

        /**
         *
         *	@brief Check if a skill is stackable with another one.
         *
         *	@retval True if its stackable, false otherwise.
         *
         **/

        bool IsStackable (const boost::shared_ptr<Skill> &skill) const;

        /**
         *
         *	@brief Check if the desired weapon/armor/shield/suit its required to cast/use the skill.
         *
         *	@param reqType: Requirement group type.
         *	@param requirement: Requirement to test.
         *
         *	@retval True its usable, false otherwise.
         *
         **/

        bool IsRequired (const ESSENTIAL_ITEM_TYPE reqType, const uint8_t requirement) const;

        /**
         *
         *	@brief Check if the weapon is valid to cast the skill.
         *
         *	@param type: Weapon to test.
         *
         *	@retval True its valid, false otherwise.
         *
         **/

        bool IsWeaponUsable (const REQ_WEAPON_TYPE type) const;

        /**
         *
         *	@brief Check if a shield is required to cast/use the skill.
         *
         **/

        bool IsShieldRequired () const;

        /**
         *
         *	@brief Check if the devil suit is required cast/use the skill.
         *
         **/

        bool IsDevilSuitRequired () const;

        /**
         *
         *	@brief Get effect value from the skill.
         *
         *	@param effect: Desired effect to retrieve.
         *
         *	@retval 0 if effect dont exists, current value otherwise.
         *
         **/

        uint32_t getEffectValue (const SKILL_EFFECT effect) const;

        /**
         *
         *	@brief Add a new effect to the skill.
         *
         *	@param effect: Effect type to add.
         *	@param value: Effect value.
         *
         **/

        void addEffect (const SKILL_EFFECT effect, const uint32_t value);

        /**
         *
         *	@brief Set skill attack information.
         *
         **/

        void setAttack (const AttackInfo &atk);

        /**
         *
         *	@brief Get skill attack description.
         *
         **/

        AttackInfo const* getAttack () const;

        /**
         *
         *	@brief Set attack area information.
         *
         **/

        void setAttackArea (const AttackArea &area);

        /**
         *
         *	@brief Get skill area of attack description.
         *
         **/

        AttackArea const* getAttackArea () const;

        /**
         *
         *	@brief Add a new buff effect to this skill.
         *
         *	@param buff: Effect to add.
         *
         **/

        void addBuff (const BuffEffect &buff);

        /**
         *
         *	@brief Check if the skill has any effects.
         *
         **/

        bool hasBuffs () const;

        /**
         *
         *	@brief Buff begin iterator.
         *
         **/

        buff_const_iterator buff_begin () const;

        /**
         *
         *	@brief Buff end iterator.
         *
         **/

        buff_const_iterator buff_end () const;

        /**
         *
         *	@brief Add a new buff effect that triggers when the skill is terminated.
         *
         *	@param buff: Effect to add.
         *
         **/

        void addEndBuff (const BuffEnd &buff);

        /**
         *
         *	@brief End buff begin iterator.
         *
         **/

        ebuff_const_iterator ebuff_begin () const;

        /**
         *
         *	@brief End buff end iterator.
         *
         **/

        ebuff_const_iterator ebuff_end () const;

        /**
         *
         *	@brief Summon list begin iterator.
         *
         **/

        spawn_const_iterator spawn_begin () const;

        /**
         *
         *	@brief Summon list end iterator.
         *
         **/

        spawn_const_iterator spawn_end () const;

        /**
         *
         *	@brief Add a new requirement to cast/use the skill.
         *
         *	@param type: Requirement type to add.
         *	@param requirement: Requirement needed (depends on the type).
         *
         **/

        void addRequirement (const ESSENTIAL_ITEM_TYPE type, const uint8_t requirement);

    private:

        uint32_t m_ID;				/// Skill ID.
        uint16_t m_GroupID;			/// Skill Group ID.
        SKILL_TYPE m_Type;			/// Type of skill.
        uint8_t m_Level;			/// Skill Level.
        uint32_t m_NextSkill;		/// Next skill to cast.
        uint32_t m_SP;				/// Required Skill Points.
        RACE m_Race;				/// Required Race.
        uint16_t m_Mastery;			/// Required Mastery.
        uint8_t m_MasteryLvl;		/// Minimum Mastery Level.
        int16_t m_GroupSkill1;		/// Required Primary Group Skill.
        uint8_t m_GroupSkillLv1;	/// Minimum Primary Group Skill level.
        int16_t m_GroupSkill2;		/// Required Secundary Group Skill.
        uint8_t m_GroupSkillLv2;	/// Minimum Secundary Group Skill level.
        REQ_WEAPON_TYPE m_Weapon1;  /// Required Primary Weapon.
        REQ_WEAPON_TYPE m_Weapon2;  /// Required Secundary Weapon.
        uint32_t m_MP;				/// Required Mana Points to cast skill.
        uint8_t m_MinHPRatio;		/// Required Hit Points Percent to cast skill.
        uint8_t m_MaxHPRatio;
        uint16_t m_InitTime;		/// Skill initiation time.
        uint16_t m_DelayTime;		/// Skill delay time.
        uint16_t m_NextTime;		/// Skill next cast time.
        uint16_t m_Cooldown;
        uint32_t m_StackID;
        SKILL_ACTION_DETAIL_TYPE m_SAD;
        uint16_t m_SADArg;

        AttackInfo *m_Attack;
        AttackArea *m_Area;

        std::vector<BuffEffect> m_buffs;
        std::vector<BuffEnd> m_end_buffs;
        std::vector<Status> m_statuses;
        std::vector<NPC_SPAWN_INFO> m_spawns;
        std::vector<std::pair<ESSENTIAL_ITEM_TYPE,uint8_t> > m_Requirements;

        std::map<SKILL_EFFECT,uint32_t> m_Effects;
    };
}

#endif //__ZELOS_LIBSSO_SSO_SKILL_SKILL_H__
