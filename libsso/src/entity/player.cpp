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

#include "entity/player.h"

#include <cmath>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>

#include <khamaileon/property/character.h>
#include <sso/entity/cos_transport.h>
#include <sso/item/item.h>
#include <sso/skill/skill.h>
#include <sso/exchange.h>
#include <sso/storage.h>

#define MAX_PLAYER_SP 50000000

namespace sso
{
    Player::Player (const boost::shared_ptr<EntityProperty> &reference)
        : 	ICharacter(reference),
            m_AccountID(0),
            m_ConnectionID(0),
            m_EquipTime(0),
            m_ExchangeWnd(new ExchangeWnd()),
            m_TaskNPC(PLAYER_NPC_TASK_NONE),
            m_IconMask(0),
            m_InvType(INVITATION_NONE),
            m_InvTarget(0)
    {
        m_BaseChar.RefCharID = reference->refID;

        m_mastery_tree = MasteryTree::Create(static_cast<RACE>(reference->Race));

        m_HP = m_BaseChar.HP;
        m_MP = m_BaseChar.MP;

        calculateStats();

        if (IsGM())
            m_BaseChar.Name = "[GM]" + m_BaseChar.Name;

        m_CurrentTask = PLAYER_TASK_IDLE;

        if (m_BaseChar.MaxLevel <= PLAYER_MIN_JOB_LEVEL)
            m_IconMask = PLAYER_MIN_LV_DISPLAY;
    }

    Player::Player (const boost::shared_ptr<EntityProperty> &reference,
                    const std::string& name, uint8_t volume)
        : 	ICharacter(reference),
            m_AccountID(0),
            m_ConnectionID(0),
            m_EquipTime(0),
            m_ExchangeWnd(new ExchangeWnd()),
            m_TaskNPC(PLAYER_NPC_TASK_NONE),
            m_IconMask(0),
            m_InvType(INVITATION_NONE),
            m_InvTarget(0)
    {
        m_BaseChar.RefCharID = reference->refID;
        m_BaseChar.Name = name;
        m_BaseChar.Volume = volume;

        m_mastery_tree = MasteryTree::Create(static_cast<RACE>(reference->Race));

        m_HP = m_BaseChar.HP;
        m_MP = m_BaseChar.MP;

        calculateStats();

        if (IsGM())
            m_BaseChar.Name = "[GM]" + m_BaseChar.Name;

        m_CurrentTask = PLAYER_TASK_IDLE;

        if (m_BaseChar.MaxLevel <= PLAYER_MIN_JOB_LEVEL)
            m_IconMask = PLAYER_MIN_LV_DISPLAY;
    }

    Player::Player (const boost::shared_ptr<EntityProperty> &reference,
                    const sso::property::Player &property)
        : 	ICharacter(reference),
            m_AccountID(0),
            m_ConnectionID(0),
            m_BaseChar(property),
            m_EquipTime(0),
            m_ExchangeWnd(new ExchangeWnd()),
            m_TaskNPC(PLAYER_NPC_TASK_NONE),
            m_IconMask(0),
            m_InvType(INVITATION_NONE),
            m_InvTarget(0)
    {
        m_mastery_tree = MasteryTree::Create(static_cast<RACE>(reference->Race));

        m_HP = m_BaseChar.HP;
        m_MP = m_BaseChar.MP;
        m_Angle = m_BaseChar.Angle;

        m_CurrentPos = Coord(m_BaseChar.X, m_BaseChar.Y, m_BaseChar.Z,m_BaseChar.Zone);

        calculateStats();

        if (IsGM())
            m_BaseChar.Name = "[GM]" + m_BaseChar.Name;

        if (!m_HP)
            m_StatusState = STATUS_ACTION_DEAD;

        m_CurrentTask = PLAYER_TASK_IDLE;

        if (m_BaseChar.MaxLevel <= PLAYER_MIN_JOB_LEVEL)
            m_IconMask = PLAYER_MIN_LV_DISPLAY;
    }

    Player::~Player ()
    {
    }

    void Player::ID (const uint64_t newID)
    {
        m_BaseChar.ID = newID;

        if (m_InvStore)
            m_InvStore->SetOwner(newID);

        if (m_AvaStore)
            m_AvaStore->SetOwner(newID);
    }

    uint64_t Player::ID () const
    {
        return m_BaseChar.ID;
    }

    void Player::AccountID (const uint32_t accountID)
    {
        m_AccountID = accountID;
    }

    uint32_t Player::AccountID () const
    {
        return m_AccountID;
    }

    void Player::ConnectionID(const uint32_t connID)
    {
        m_ConnectionID = connID;
    }

    uint32_t Player::ConnectionID() const
    {
        return m_ConnectionID;
    }

    void Player::setName (const std::string& name)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.Name = name;
    }

    std::string Player::getName () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.Name;
    }

    void Player::setFakeName (const std::string &name)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_FakeName = name;
    }

    void Player::resetFakeName ()
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_FakeName.clear();
    }

    const std::string& Player::getFakeName () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_FakeName;
    }

    bool Player::hasFakeName () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return !m_FakeName.empty();
    }

    uint8_t Player::getScale () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.Volume;
    }

    void Player::setGM (bool flag)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.GM = flag;
    }

    bool Player::IsGM () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.GM;
    }

    uint8_t Player::getLevel () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.Level;
    }

    void Player::Levelup ()
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        ++m_BaseChar.Level;

        if (m_BaseChar.MaxLevel < m_BaseChar.Level)
        {
            m_BaseChar.MaxLevel = m_BaseChar.Level;

            if (m_BaseChar.MaxLevel > PLAYER_MIN_JOB_LEVEL)
                m_IconMask &= ~PLAYER_MIN_LV_DISPLAY;

            m_BaseChar.Stats += 3;

            m_BaseChar.Str++;
            m_BaseChar.Int++;
        }

        m_BaseChar.Exp = 0;
    }

    uint8_t Player::getMaxLv () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.MaxLevel;
    }

    uint64_t Player::getGold () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.Gold;
    }

    void Player::addGold (const uint64_t amount)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.Gold += amount;

        uint64_t curGold = m_BaseChar.Gold;

        lock.unlock();

        if (!signal_gold.empty())
            signal_gold(curGold);
    }

    bool Player::subGold (const uint64_t amount)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if ( amount > m_BaseChar.Gold)
            return false;

        m_BaseChar.Gold -= amount;

        uint64_t curGold = m_BaseChar.Gold;

        lock.unlock();

        if (!signal_gold.empty())
            signal_gold(curGold);

        return true;
    }

    bool Player::depositGold (const uint64_t amount)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if ( amount > m_BaseChar.Gold)
            return false;

        m_BaseChar.Gold -= amount;

        m_BaseChar.GoldStored += amount;

        uint64_t curGold = m_BaseChar.Gold;

        lock.unlock();

        if (!signal_gold.empty())
            signal_gold(curGold);

        return true;
    }

    bool Player::withdrawGold (const uint64_t amount)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if ( amount > m_BaseChar.GoldStored)
            return false;

        m_BaseChar.Gold += amount;

        m_BaseChar.GoldStored -= amount;

        uint64_t curGold = m_BaseChar.Gold;

        lock.unlock();

        if (!signal_gold.empty())
            signal_gold(curGold);

        return true;
    }

    uint64_t Player::getStoredGold () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.GoldStored;
    }

    uint16_t Player::getStatPoints () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.Stats;
    }

    bool Player::useStatPoint (const PLAYER_STAT_POINT type)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if (!m_BaseChar.Stats)
            return false;

        if (type == PLAYER_STAT_INT)
            ++m_BaseChar.Int;
        else
            ++m_BaseChar.Str;

        --m_BaseChar.Stats;

        lock.unlock();

        calculateStats();

        return true;
    }

    uint64_t Player::getExp () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.Exp;
    }

    void Player::addExp (const uint64_t amount)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.Exp += amount;
    }

    void Player::subExp (const uint64_t exp, const uint64_t prevExp)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        if (m_BaseChar.Exp >= exp)
            m_BaseChar.Exp -= exp;
        else
            m_BaseChar.Exp = prevExp + m_BaseChar.Exp - exp;
    }

    uint16_t Player::getSExp () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.SPExp;
    }

    void Player::addSExp (const uint32_t amount)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.SPExp += amount ;

        uint32_t sp = m_BaseChar.SPExp / 400;

        m_BaseChar.SPExp %= 400;

        if (sp)
        {
            m_BaseChar.SP += sp;

            if (m_BaseChar.SP > MAX_PLAYER_SP)
                m_BaseChar.SP = MAX_PLAYER_SP;

            sp = m_BaseChar.SP;

            lock.unlock();

            if (!signal_sp.empty())
                signal_sp(sp);
        }
    }

    uint32_t Player::getSP () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.SP;
    }

    bool Player::subSP (const uint32_t sp)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if ( sp > m_BaseChar.SP )
            return false;

        m_BaseChar.SP -= sp;

        uint32_t curSP = m_BaseChar.SP;

        lock.unlock();

        if (!signal_sp.empty())
            signal_sp(curSP);

        return true;
    }

    void Player::setRank (const PLAYER_RANK rank)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.Rank = rank;
    }

    PLAYER_RANK Player::getRank() const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return static_cast<PLAYER_RANK>(m_BaseChar.Rank);
    }

    uint8_t Player::getBerserkCounter () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.BskCount;
    }

    void Player::incBerserkCounter ()
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        ++m_BaseChar.BskCount;
    }

    void Player::fillBerserkCounter ()
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.BskCount = MAX_PLAYER_BERSERK_COUNT;
    }

    void Player::Berserk (bool activate)
    {
        boost::lock(m_player_mutex,m_state_mutex);

        boost::mutex::scoped_lock lock(m_player_mutex,boost::adopt_lock);
        boost::mutex::scoped_lock slock(m_state_mutex,boost::adopt_lock);

        if (activate)
        {
            if (m_BaseChar.BskCount < MAX_PLAYER_BERSERK_COUNT)
                return;

            m_BaseChar.BskCount = 0;
            m_AttributeState = ATTRIBUTE_ACTION_BERSERK;
        }
        else
        {
            m_AttributeState = ATTRIBUTE_ACTION_NORMAL;
        }

        if (!signal_state.empty())
            signal_state(STATE_ATTRIBUTE,m_AttributeState);

        slock.unlock();

        lock.unlock();

        calculateStats();
    }

    void Player::setReturnID (const uint32_t ID)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_BaseChar.ReturnID = ID;
    }

    uint32_t Player::getReturnID() const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.ReturnID;
    }

    uint64_t Player::getTutorials () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.Tutorials;
    }

    void Player::addTutorial (uint64_t tutorial)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.Tutorials |= tutorial;
    }

    bool Player::hasTutorial (uint64_t tutorial)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.Tutorials & tutorial;
    }

    void Player::Morph (const boost::shared_ptr<EntityProperty> &property, const uint8_t scale)
    {
        boost::lock(m_player_mutex,m_property_mutex);

        boost::mutex::scoped_lock eLock(m_property_mutex,boost::adopt_lock);
        boost::mutex::scoped_lock pLock(m_player_mutex,boost::adopt_lock);

        m_BaseChar.RefCharID = property->refID;
        m_BaseChar.Volume = scale;

        m_property = property;
    }

    void Player::addIcon (const uint8_t IconMask)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_IconMask |= IconMask;
    }

    void Player::removeIcon (const uint8_t IconMask)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        m_IconMask &= ~IconMask;
    }

    uint8_t Player::getIconMask () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_IconMask;
    }

    void Player::setPKDaily (uint8_t pk)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.PKDaily = pk;
    }

    uint8_t Player::getPKDaily () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.PKDaily;
    }

    uint16_t Player::getPKLevel () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.PKLevel;
    }

    void Player::addPKPoints (const uint32_t points)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.PKPoints += points;
    }

    void Player::subPKPoints (const uint32_t points)
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        if (m_BaseChar.PKPoints > points)
            m_BaseChar.PKPoints -= points;
        else
            m_BaseChar.PKPoints = 0;
    }

    uint32_t Player::getPKPoints () const
    {
        boost::lock_guard<boost::mutex> lock(m_player_mutex);

        return m_BaseChar.PKPoints;
    }

    bool Player::IsStorageExpanded () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.StorageSlotsIncreased;
    }

    uint8_t Player::getStorageSlots () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.StoreSlots;
    }

    void Player::setStorage (const boost::shared_ptr<Storage> &storage, const LOCATION_TYPE type)
    {
        switch(type)
        {
        case LOCATION_INVENTORY:
            m_InvStore = storage;
            break;
        case LOCATION_AVATAR:
            m_AvaStore = storage;
            break;
        case LOCATION_STORAGE:
            m_MainStore = storage;
            break;
        default:
            break;
        }
    }

    boost::shared_ptr<Storage> Player::getStorage(const LOCATION_TYPE type)
    {
        boost::shared_ptr<Storage> st;

        switch(type)
        {
        case LOCATION_INVENTORY:
            st = m_InvStore;
            break;
        case LOCATION_AVATAR:
            st = m_AvaStore;
            break;
        case LOCATION_STORAGE:
            st = m_MainStore;
            break;
        default:
            break;
        }

        return st;
    }

    bool Player::IsInvExpanded () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.InvSlotsIncreased;
    }

    void Player::IncInventorySlots (const uint8_t nslots)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if (!m_BaseChar.InvSlotsIncreased)
        {
            m_InvStore->MaxSlotsInc(nslots);

            m_BaseChar.InvSlotsIncreased = true;
        }
    }

    void Player::IncStorageSlots (const uint8_t nslots)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        if (!m_BaseChar.StorageSlotsIncreased)
        {
            m_MainStore->MaxSlotsInc (nslots);

            m_BaseChar.StorageSlotsIncreased = true;
        }
    }

    void Player::setSkills (const std::set<uint32_t> &list)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_SkillList = list;
    }

    const std::set<uint32_t>& Player::getSkills () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_SkillList;
    }

    void Player::addSkill (const uint32_t refID)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_SkillList.insert(refID);
    }

    void Player::removeSkill (const uint32_t refID)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_SkillList.erase(refID);
    }

    bool Player::hasSkill (const uint32_t refID) const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_SkillList.find(refID) != m_SkillList.end();
    }

    uint32_t Player::getBaseAttack () const
    {
        boost::shared_ptr<Item> item = m_InvStore->Find(Item::WEAPON_SLOT);

        uint32_t skillid = SKILL_PUNCH_01;

        if (item)
        {
            switch(item->getType())
            {
            case ITEM_CH_SWORD:
            case ITEM_CH_BLADE:
                skillid = SKILL_CH_SWORD_BASE_01;
                break;
            case ITEM_CH_BOW:
                skillid = SKILL_CH_BOW_BASE_01;
                break;
            case ITEM_CH_SPEAR:
            case ITEM_CH_TBLADE:
                skillid = SKILL_CH_SPEAR_BASE_01;
                break;
            case ITEM_EU_SWORD:
                skillid = SKILL_EU_SWORD_BASE_01;
                break;
            case ITEM_EU_TSWORD:
                skillid = SKILL_EU_TSWORD_BASE_01;
                break;
            case ITEM_EU_AXE:
                skillid = SKILL_EU_AXE_BASE_01;
                break;
            case ITEM_EU_DAGGER:
                skillid = SKILL_EU_DAGGER_BASE_01;
                break;
            case ITEM_EU_CROSSBOW:
                skillid = SKILL_EU_CROSSBOW_BASE_01;
                break;
            case ITEM_EU_HARP:
                skillid = SKILL_EU_HARP_BASE_01;
                break;
            case ITEM_EU_STAFF:
                skillid = SKILL_EU_WAND_CLERIC_BASE_01;
                break;
            case ITEM_EU_TSTAFF:
                skillid = SKILL_EU_STAFF_BASE_01;
                break;
            case ITEM_EU_DARKSTAFF:
                skillid = SKILL_EU_WAND_WARLOCK_BASE_01;
                break;
            default:
                skillid = SKILL_PUNCH_01;
                break;
            }
        }

        return skillid;
    }

    void Player::addPassive (const Buff &passive)
    {
        boost::mutex::scoped_lock lock(m_passive_mutex);

        /// DEACTIVATE CURRENT HIGHEST PASSIVE OF THE SAME GROUP

        if (passive.skill->Level() > 1)
        {
            typedef std::multimap<uint32_t,Buff>::iterator passive_iterator;

            std::pair<passive_iterator,passive_iterator> range = m_Passives.equal_range(passive.skill->refGroupID());

            for (passive_iterator it = range.first; it != range.second; ++it)
                it->second.active = false;
        }

        m_Passives.insert(std::make_pair(passive.skill->refGroupID(),passive));

        lock.unlock();

        calculateStats();
    }

    void Player::removePassive (const uint32_t groupID, const uint32_t skillID)
    {
        boost::mutex::scoped_lock lock(m_passive_mutex);

        /// ACTIVATE CURRENT 2ND HIGHEST PASSIVE BEFORE OF THIS GROUP

        typedef std::multimap<uint32_t,Buff>::iterator passive_iterator;

        std::pair<passive_iterator,passive_iterator> range = m_Passives.equal_range(groupID);

        passive_iterator highestIt = range.first;

        for (passive_iterator it = range.first; it != range.second;)
        {
            if (it->second.skill->Level() > highestIt->second.skill->Level())
                highestIt = it;

            if (it->second.skill->refID() == skillID)
                m_Passives.erase(it++);
            else
                ++it;
        }

        if (highestIt != range.second)
            highestIt->second.active = true;

        lock.unlock();

        calculateStats();
    }

    const MasteryTree& Player::get_mastery_tree () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_mastery_tree;
    }

    MasteryTree& Player::get_mastery_tree ()
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_mastery_tree;
    }

    int Player::getMasteryBonus (const uint32_t masteryID) const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_mastery_tree.getBonus(masteryID);
    }

    void Player::set_mastery_tree (const MasteryTree& tree)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_mastery_tree = tree;
    }

    void Player::setStall (const boost::shared_ptr<Stall> &stall)
    {
        boost::unique_lock<boost::mutex> lock(m_stall_mutex);

        m_Stall = stall;
    }

    boost::shared_ptr<Stall> Player::getStall () const
    {
        boost::mutex::scoped_lock lock(m_stall_mutex);

        return m_Stall;
    }

    void Player::setStallAvatar (const uint32_t avatarID)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.StallAvatarID = avatarID;
    }

    uint32_t Player::getStallAvatar () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.StallAvatarID;
    }

    void Player::resetStall ()
    {
        boost::unique_lock<boost::mutex> lock(m_stall_mutex);

        m_Stall.reset();
    }

    void Player::setInvitation (const INVITATION_TYPE type)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_InvType = type;
    }

    INVITATION_TYPE Player::getInvitation () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_InvType;
    }

    void Player::setInvitationPlayer (const uint32_t ID)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_InvTarget = ID;
    }

    uint32_t Player::getInvitationPlayer () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_InvTarget;
    }

    boost::shared_ptr<ExchangeWnd> Player::getExchangeWindow () const
    {
        return m_ExchangeWnd;
    }

    void Player::setParty (const boost::shared_ptr<Party> &party)
    {
        boost::lock_guard<boost::mutex> lock(m_party_mutex);

        m_Party = party;
    }

    boost::shared_ptr<Party> Player::getParty () const
    {
        boost::lock_guard<boost::mutex> lock(m_party_mutex);

        return m_Party;
    }

    void Player::resetParty ()
    {
        boost::lock_guard<boost::mutex> lock(m_party_mutex);

        m_Party.reset();
    }

    uint32_t Player::getGuildID () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.GuildID;
    }

    void Player::setGuild (const boost::shared_ptr<Guild> &guild, const uint32_t guildID)
    {
        if (guild)
        {
            boost::mutex::scoped_lock lock(m_player_mutex);

            m_BaseChar.GuildID = guildID;

            m_Guild = guild;
        }
    }

    boost::shared_ptr<Guild> Player::getGuild () const
    {
        boost::mutex::scoped_lock lock(m_guild_mutex);

        return m_Guild;
    }

    void Player::resetGuild ()
    {
        boost::mutex::scoped_lock lock(m_guild_mutex);

        m_BaseChar.GuildID = 0;
        m_Guild.reset();
    }

    void Player::addCOS (const boost::shared_ptr<COS> &cos)
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        m_cos.push_back(cos);
    }

    bool Player::hasCOS (const PLAYER_COS type) const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        for (size_t i = 0; i < m_cos.size(); ++i)
        {
            switch(type)
            {
            case PLAYER_COS_TRANSPORT:
                if (m_cos[i]->getType() == ET_COS_TRANSPORT || m_cos[i]->getType() == ET_COS_VEHICLE)
                    return true;
                break;
            case PLAYER_COS_ATTACK:
            case PLAYER_COS_ABILITY:
                if (static_cast<PLAYER_COS>(m_cos[i]->getType()) == type)
                    return true;
                break;
            default:
                break;
            }
        }

        return false;
    }

    bool Player::hasCOS (const uint32_t cosID) const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        for (size_t i = 0; i < m_cos.size(); ++i)
        {
            if (m_cos[i]->GUID() == cosID)
                return true;
        }

        return false;
    }

    boost::shared_ptr<COS> Player::getCOS (const uint32_t cosID) const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        boost::shared_ptr<COS> cos;
        for (size_t i = 0; i < m_cos.size(); ++i)
        {
            if (m_cos[i]->GUID() == cosID)
            {
                cos = m_cos[i];
                break;
            }
        }

        return cos;
    }

    boost::shared_ptr<COS> Player::getCOS (const PLAYER_COS type) const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        boost::shared_ptr<COS> cos;
        for (size_t i = 0; i < m_cos.size(); ++i)
        {
            switch(type)
            {
            case PLAYER_COS_TRANSPORT:
                if (m_cos[i]->getType() == ET_COS_TRANSPORT || m_cos[i]->getType() == ET_COS_VEHICLE)
                    return m_cos[i];
                break;
            case PLAYER_COS_ATTACK:
            case PLAYER_COS_ABILITY:
                if (static_cast<PLAYER_COS>(m_cos[i]->getType()) == type)
                    return m_cos[i];
                break;
            default:
                break;
            }
        }

        return cos;
    }

    std::vector<boost::shared_ptr<COS> > Player::getCOSs () const
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        return m_cos;
    }

    void Player::removeCOS (const uint32_t cosID)
    {
        boost::mutex::scoped_lock lock(m_cos_mutex);

        for (size_t i = 0; i < m_cos.size(); ++i)
        {
            if (m_cos[i]->GUID() == cosID)
            {
                m_cos.erase(m_cos.begin()+i);
                break;
            }
        }
    }

    void Player::Resurrect (bool setInvulnerable, bool maxRecovery)
    {
        boost::lock(m_state_mutex,m_stats_mutex);

        boost::mutex::scoped_lock state_lock(m_state_mutex,boost::adopt_lock);
        boost::mutex::scoped_lock stat_lock(m_stats_mutex,boost::adopt_lock);

        m_StatusState = STATUS_ACTION_ALIVE;

        if (!signal_state.empty())
            signal_state(STATE_STATUS,m_StatusState);

        if (setInvulnerable)
        {
            m_AttributeState = ATTRIBUTE_ACTION_INVUNERABLE;

            if (!signal_state.empty())
                signal_state(STATE_ATTRIBUTE,m_AttributeState);
        }

        if (maxRecovery)
        {
            m_HP = m_MaxHP;
            m_MP = m_MaxMP;
        }
        else
        {
            if (!m_HP)
                m_HP = (m_MaxHP*0.2);

            if (!m_MP)
                m_MP = (m_MaxMP*0.2);
        }
    }

    void Player::setJob (const Job &job)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_Job = job;
    }

    Job Player::getJob () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_Job;
    }

    bool Player::IsJobbing () const
    {
        boost::shared_ptr<Item> item = m_InvStore->Find(Item::JOB_CAPE_SLOT);

        if (!item)
            return false;

        ITEM_TYPE type = item->getType();

        if (type == ITEM_JOB_TRADER || type == ITEM_JOB_THIEF || type == ITEM_JOB_HUNTER)
            return true;

        return false;
    }

    uint8_t Player::PvPCape () const
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        return m_BaseChar.pvpCape;
    }

    void Player::setPvPCape (uint8_t cape)
    {
        boost::mutex::scoped_lock lock(m_player_mutex);

        m_BaseChar.pvpCape = cape;
    }

    void Player::setEquipWaitTime (uint8_t time)
    {
        m_EquipTime = time;
    }

    uint8_t Player::getEquipWaitTime () const
    {
        return m_EquipTime;
    }

    void Player::EstablishRecallPoint ()
    {
        m_RecallPoint = m_CurrentPos;
    }

    const Coord& Player::GetRecallPoint () const
    {
        return m_RecallPoint;
    }

    void Player::EstablishDeathPoint ()
    {
        m_DeathPoint = m_CurrentPos;
    }

    const Coord& Player::GetDeathPoint () const
    {
        return m_DeathPoint;
    }

    bool Player::IsTaskInterruptable () const
    {
        boost::mutex::scoped_lock lock(m_queue_mutex);

        if (m_CurrentTask > PLAYER_TASK_ITEM_MOVE)
            return false;

        return true;
    }

    void Player::setTaskNPC (const PLAYER_NPC_TASK task)
    {
        boost::lock_guard<boost::mutex> lock(m_queue_mutex);

        m_TaskNPC = task;
    }

    PLAYER_NPC_TASK Player::getTaskNPC () const
    {
        boost::lock_guard<boost::mutex> lock(m_queue_mutex);

        return m_TaskNPC;
    }

    bool Player::IsBusyNPC () const
    {
        bool ret = false;

        switch(getTaskNPC())
        {
        case PLAYER_NPC_TASK_TALK:
        case PLAYER_NPC_TASK_BUY_SELL:
        case PLAYER_NPC_TASK_BUY_SELL_SPECIAL_GOODS:
        case PLAYER_NPC_TASK_STORAGE:
        case PLAYER_NPC_TASK_GUILD_MANAGEMENT:
            ret = true;
        default:
            break;
        }

        return ret;
    }

    bool Player::IsInviteable () const
    {
        boost::lock_guard<boost::mutex> lock(m_queue_mutex);

        if (m_TaskNPC != PLAYER_NPC_TASK_NONE)
            return false;

        if (m_CurrentTask == PLAYER_TASK_EXCHANGE || m_CurrentTask == PLAYER_TASK_STALL)
            return false;

        return getInvitationPlayer();
    }

    bool Player::IsItemAvailable (const uint32_t itemType) const
    {
        boost::mutex::scoped_lock lock(m_item_mutex);

        std::map<uint32_t,uint32_t>::const_iterator iter = m_cooldown_itemgroup.find(itemType);

        return iter == m_cooldown_itemgroup.end();
    }

    void Player::addItemCooldown (const uint32_t taskID, const uint32_t itemType)
    {
        boost::mutex::scoped_lock lock(m_item_mutex);

        m_cooldown_itemgroup.insert(std::make_pair(itemType,taskID));
    }

    void Player::removeItemCooldown (const uint32_t itemType)
    {
        boost::mutex::scoped_lock lock(m_item_mutex);

        m_cooldown_itemgroup.erase(itemType);
    }

    const std::map<uint32_t,uint32_t>& Player::getItemCooldownList() const
    {
        boost::mutex::scoped_lock lock(m_item_mutex);

        return m_cooldown_itemgroup;
    }

    std::vector<std::pair<uint32_t,uint32_t> > Player::removeInvalidatedBuffs ()
    {
        std::vector<std::pair<uint32_t,uint32_t> > ret;

        boost::shared_ptr<Item> weapon = m_InvStore->Find(Item::WEAPON_SLOT);
        boost::shared_ptr<Item> shield = m_InvStore->Find(Item::SHIELD_SLOT);

        bool remove = false;
        REQ_WEAPON_TYPE weapon_type = weapon ? (REQ_WEAPON_TYPE)weapon->getType() : REQ_WEAPON_NONE;

        boost::mutex::scoped_lock lock(m_buff_mutex);

        for (std::map<uint32_t,Buff>::iterator i = m_buff_list.begin(); i != m_buff_list.end();)
        {
            if (!i->second.skill->IsRequired(ESS_WEAPON,weapon_type))
                remove = true;

            if (i->second.skill->IsShieldRequired() && !shield)
               remove = true;

            if (remove)
            {
                ret.push_back(std::make_pair(i->second.taskID,i->second.CastID));
                m_buff_list.erase(i++);

                remove = false;
            }
            else
                ++i;
        }

        return ret;
    }

    void Player::calculatePreStats (std::vector<std::pair<CHAR_STATS, int32_t> > &bonus)
    {
        boost::mutex::scoped_lock player_lock(m_player_mutex);

        /// SET BASE STATS AND RECALCULATE
        m_Effects[STAT_STR] = m_BaseChar.Str;
        m_Effects[STAT_INT] = m_BaseChar.Int;

        /// CALCULATE BASE DEFENSE POWER
        m_PD = 6;
        m_MD = 6;
        m_HR = 10 + m_BaseChar.MaxLevel;
        m_PR = 10 + m_BaseChar.MaxLevel;

        m_MaxHP = 0;
        m_MaxMP = 0;

        if (m_InvStore)
        {
            boost::lock_guard<boost::mutex> invLock(m_InvStore->m_mutex);
            for (Storage::const_iterator iter = m_InvStore->begin(); iter != m_InvStore->end(); ++iter)
            {
                if (iter->first < Item::MAX_EQUIPMENT_SLOT)
                {
                    if (iter->first < Item::JOB_CAPE_SLOT && iter->second->getDurability())
                    {
                        if (iter->first == Item::WEAPON_SLOT)
                        {
                            m_HR += iter->second->hit_ratio();
                            m_CR += iter->second->critical_ratio();
                            m_AD = iter->second->attack_distance();
                            m_PA = iter->second->physical_attack();
                            m_MA = iter->second->magical_attack();
                        }
                        else
                        {
                            m_PD += iter->second->physical_defense();
                            m_MD += iter->second->magical_defense();

                            m_PR += iter->second->parry_ratio();

                            if (iter->first == Item::SHIELD_SLOT)
                            {
                                m_BR += iter->second->block_ratio();
                            }
                        }
                    }
                    else if (iter->first == Item::JOB_CAPE_SLOT)
                    {
                        //TODO: ADD JOB BLUES!!
                    }
                    else
                    {
                        m_PAbs += iter->second->physical_absorb();
                        m_MAbs += iter->second->magical_absorb();

                        //TODO: ADD BLUES!!
                    }

                    //m_Str += iter->second.get_str();
                    //m_Int += iter->second.get_int();
                }
            }
        }

        boost::lock_guard<boost::mutex> passiveLock(m_passive_mutex);

        for ( std::multimap<uint32_t,Buff>::const_iterator k = m_Passives.begin(); k != m_Passives.end(); ++k)
        {
            /// CHECK IF WE MEET THE PASSIVE REQUIREMENTS
            if (k->second.skill->IsShieldRequired())
            {
                boost::shared_ptr<Item> shield;

                if (m_InvStore)
                    shield = m_InvStore->Find(Item::SHIELD_SLOT);

                if (!shield || (shield->getType() != ITEM_CH_SHIELD && shield->getType() != ITEM_EU_SHIELD))
                    continue;
            }

            /// ADD BONUS

            if (k->second.active)
            {
                for (Skill::buff_const_iterator j = k->second.skill->buff_begin(); j != k->second.skill->buff_end(); ++j)
                {
                    switch(j->ID)
                    {
                    case BUFF_CRITICAL_INC:
                        m_CR += j->Arg[0];
                        break;
                    case BUFF_PARRY_INC:
                        m_PR += j->Arg[0];

                        if (j->Arg[1])
                            bonus.push_back(std::make_pair(STAT_PR_PERCENT,j->Arg[1]));
                        break;
                    case BUFF_HIT_INC:
                        m_HR += j->Arg[0];

                        if (j->Arg[1])
                            bonus.push_back(std::make_pair(STAT_HR_PERCENT,j->Arg[1]));
                        break;
                    case BUFF_HP_INC:
                        m_MaxHP += j->Arg[0];

                        if (j->Arg[1])
                            bonus.push_back(std::make_pair(STAT_HP_PERCENT,j->Arg[1]));
                        break;
                    case BUFF_MP_INC:
                        m_MaxMP += j->Arg[0];

                        if (j->Arg[1])
                            bonus.push_back(std::make_pair(STAT_MP_PERCENT,j->Arg[1]));
                        break;
                    case BUFF_RANGE_INC:
                        m_AD += j->Arg[0];
                        break;
                    case BUFF_DAMAGE_INC:
                        m_PRate += j->Arg[0];
                        m_MRate += j->Arg[0];
                        break;
                    case BUFF_CRIT_PARRY_INC:
                        m_Effects[STAT_CPR] += j->Arg[0];
                        break;
                    case BUFF_DAMAGE_PWR_INC:
                        m_PA.first += j->Arg[0];
                        m_PA.second += j->Arg[0];

                        m_MA.first += j->Arg[1];
                        m_MA.second += j->Arg[1];
                        break;
                    case BUFF_BLOCK_INC:
                        //BLOCK TYPE +0 (15 ALL) (11 Magic %) (7 Physical %)
                        //BLOCK RATE +1
                        m_BR += j->Arg[1];
                        break;
                    case BUFF_DEFENSE_INC:
                        m_PD += j->Arg[0];
                        m_MD += j->Arg[1];
                        //Caster Range
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    void Player::calculateBuffEffects (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus,
                                       const BuffEffect &effect)
    {
        switch(effect.ID)
        {
        case BUFF_INT_INC:
            if (m_BaseChar.Int*effect.Arg[1]/100 >= effect.Arg[0])
                m_Effects[STAT_INT] += effect.Arg[0];
            break;
        case BUFF_STR_INC:
            if (m_BaseChar.Str*effect.Arg[1]/100 >= effect.Arg[0])
                m_Effects[STAT_STR] += effect.Arg[0];
            break;
        case BUFF_SHIELD_PWR_4_DAMAGE:
            {
                boost::shared_ptr<Item> shield;

                if (m_InvStore)
                    shield = m_InvStore->Find(Item::SHIELD_SLOT);

                if (shield)
                {
                    m_PD -= effect.Arg[0]*shield->physical_defense()/100;

                    m_PA.first += effect.Arg[1];
                    m_PA.second += effect.Arg[1];
                }
            }
            break;
        default:
            break;
        }
    }

    void Player::calculatePostStats ()
    {
        uint32_t str = m_Effects[STAT_STR];
        uint32_t intt = m_Effects[STAT_INT];

        m_PA.first += str/2;
        m_PA.second += str/2;
        m_MA.first += intt/2;
        m_MA.second += intt/2;

        boost::shared_ptr<Item> weapon;

        if (m_InvStore)
            weapon = m_InvStore->Find(Item::WEAPON_SLOT);

        if (weapon)
        {
            m_PA.first += str*(static_cast<float>(weapon->physical_reinforce().first))/1000;

            m_PA.second += str*(static_cast<float>(weapon->physical_reinforce().second))/1000;

            m_MA.first += intt*(static_cast<float>(weapon->magical_reinforce().first))/1000;

            m_MA.second += intt*(static_cast<float>(weapon->magical_reinforce().second))/1000;
        }

        m_PD += ((str-20)/2);
        m_MD += ((intt-20)/2);

        m_MaxHP += std::pow(1.02,m_BaseChar.Level- 1) * ( str * 10);
        m_MaxMP += std::pow(1.02,m_BaseChar.Level- 1) * ( intt * 10);

        float M = 28.0f+4.0f*m_BaseChar.Level;

        m_PBalance = 1.0f - 1.0f*(2.0f/3.0f)*(M-str)/M;
        m_MBalance = 1.0f*intt/M;
    }

    bool CanPlayerInteract (const boost::shared_ptr<Player> &p1, const boost::shared_ptr<Player> &p2, bool isAttack)
    {
        bool allow = true;

        if (p1->IsJobbing())
        {
            if (p2->IsJobbing())
            {
                uint8_t inv_job = p1->getJob().Type;
                uint8_t player_job = p2->getJob().Type;

                if (inv_job == JOB_THIEF)
                {
                    if (player_job != JOB_THIEF)
                        allow = isAttack ? true : false;
                }
                else
                {
                    if (player_job == JOB_THIEF)
                        allow = isAttack ? true : false;
                }
            }
            else
                allow = false;
        }
        else
        {
            if (p2->IsJobbing())
                allow = false;
            else
            {
                if (p1->PvPCape())
                {
                    if (p2->PvPCape())
                    {
                        /// GOLD CAPE IS FREE TO ATTACK EVERYONE BUT CAN ONLY TEAM WITH THEIR OWN TYPE

                        if (p1->PvPCape() == PLAYER_PVP_CAPE_GOLD)
                        {
                            if (isAttack)
                                allow = true;
                            else
                                allow = (p1->PvPCape() == p2->PvPCape());
                        }
                        else
                            allow = (isAttack && p2->PvPCape() != p1->PvPCape()) ? true : false;
                    }
                    else
                        allow = false;
                }
                else
                {
                    if (p2->PvPCape())
                        allow = false;
                }
            }
        }

        return allow;
    }
}
