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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_PLAYER_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_PLAYER_H__

#include <sso/autopot.h>
#include <sso/entity/character.h>
#include <sso/hotkey.h>
#include <sso/invitation_type.h>
#include <sso/job.h>
#include <sso/location.h>
#include <sso/mastery.h>
#include <sso/property/player.h>

namespace sso
{
    class Item;
    class Stall;
    class Storage;
    class ExchangeWnd;
    class Skill;
    class Guild;
    class Party;
    class Transport;
    class COS;

    enum PLAYER_STAT_POINT
    {
        PLAYER_STAT_INT,
        PLAYER_STAT_STR
    };

    enum PLAYER_RANK
    {
        RANK_GRUNT,
        RANK_CAPTAIN,
        RANK_COMMANDER
    };

    enum PLAYER_PVP_CAPE
    {
        PLAYER_PVP_CAPE_NONE,
        PLAYER_PVP_CAPE_RED,
        PLAYER_PVP_CAPE_GREY,
        PLAYER_PVP_CAPE_BLUE,
        PLAYER_PVP_CAPE_WHITE,
        PLAYER_PVP_CAPE_GOLD
    };

    /**
     *
     *  @brief Available player task.
     *  Any task after PLAYER_TASK_ITEM_MOVE are not interruptable.
     *
     **/

    enum PLAYER_TASK
    {
        PLAYER_TASK_IDLE,
        PLAYER_TASK_MOVE,
        PLAYER_TASK_MOVE_TRANSPORT,
        PLAYER_TASK_MOVE_TO_TARGET,
        PLAYER_TASK_MOVE_TO_ITEM,
        PLAYER_TASK_CAST_WAIT,
        PLAYER_TASK_ITEM_MOVE,
        PLAYER_TASK_STANCE,
        PLAYER_TASK_PICKUP,
        PLAYER_TASK_CAST,
        PLAYER_TASK_STALL,
        PLAYER_TASK_EXCHANGE,
        PLAYER_TASK_ALCHEMY,
        PLAYER_TASK_TELEPORT,
        PLAYER_TASK_STUNNED,
        PLAYER_TASK_LOADING
    };

    enum PLAYER_NPC_TASK
    {
        PLAYER_NPC_TASK_NONE,
        PLAYER_NPC_TASK_TALK,
        PLAYER_NPC_TASK_BUY_SELL,
        PLAYER_NPC_TASK_BUY_SELL_SPECIAL_GOODS,
        PLAYER_NPC_TASK_STORAGE,
        PLAYER_NPC_TASK_GUILD_MANAGEMENT
    };

    enum PLAYER_COS
    {
        PLAYER_COS_TRANSPORT = ET_COS_TRANSPORT,
        PLAYER_COS_ABILITY = ET_COS_ABILITY,
        PLAYER_COS_ATTACK = ET_COS_GROWTH
    };

    enum PLAYER_BACKGROUND_TASK
    {
        PLAYER_BG_LOGOUT,
        PLAYER_BG_THIEF_TELEPORT,
        PLAYER_BG_DEATH_CAPE
    };

    class Player : public ICharacter
    {
    public:

        boost::signals2::signal<void (uint64_t)> signal_gold;

        boost::signals2::signal<void (uint32_t)> signal_sp;

        boost::signals2::signal<void ()> signal_connect_party;

    public:

        typedef ICharacter::attack_type attack_type;

        Player (const boost::shared_ptr<EntityProperty> &reference);

        Player(const boost::shared_ptr<EntityProperty> &reference,
               const sso::property::Player& property);

        Player (const boost::shared_ptr<EntityProperty> &reference,
                const std::string& name, const uint8_t scale);

        virtual ~Player();

        /**
         *
         *  @brief Set Character UniqueID (only used for DB operations).
         *
         *	@param newID: Database refID.
         *
         **/

        void ID (const uint64_t newID);

        /**
         *
         *  @brief Get Character UniqueID (only used for DB operations).
         *
         *	@retval Associated database refID.
         *
         **/

        uint64_t ID () const;

        /**
         *
         *	@brief Set associated account refID.
         *
         *	@param accountID: Account refID.
         *
         **/

        void AccountID (const uint32_t accountID);

        /**
         *
         *	@brief Get associated account refID.
         *
         *	@retval Account refID.
         *
         **/

        uint32_t AccountID () const;

        /**
         *
         *	@brief Set associated connection ID.
         *
         *	@param connID: Connection ID.
         *
         **/

        void ConnectionID (const uint32_t connID);

        /**
         *
         *	@brief Get associated connection ID with the player.
         *
         *	@retval Connection ID.
         *
         **/

        uint32_t ConnectionID () const;

        /**
         *
         *	@brief Set player name (Max characters is 12).
         *
         *	@param Name: Player desired name.
         *
         **/

        void setName (const std::string& name);

        /**
         *
         *	@brief Retrieve player current name.
         *
         *	@retval Current displayed name.
         *
         **/

        std::string getName () const;

        /**
         *
         *	@brief Set character fake name (Mainly used for GM).
         *
         *	@param name: Fake name to display.
         *
         **/

        void setFakeName (const std::string &name);

        /**
         *
         *	@brief Clear fake name.
         *
         **/

        void resetFakeName ();

        /**
         *
         *	@brief Retrieve current fake name.
         *
         *	@retval Fake name.
         *
         **/

        const std::string& getFakeName () const;

        /**
         *
         *	@brief Check if the player has a fake name or not.
         *
         *	@retval True if the player has a fake name, false otherwise.
         *
         **/

        bool hasFakeName () const;

        /**
         *
         *	@brief Retrieve player current scale information. (Height//Volume)
         *
         *	@retval Current scale.
         *
         **/

        uint8_t getScale () const;

        /**
         *
         *	@brief Set player as a game master.
         *
         *	@param flag: Activation flag (True to add GM, false normal character).
         *
         **/

        void setGM (bool flag);

        /**
         *
         *	@brief Check if a player is a GM or not.
         *
         *	@retval True if its a GM, false otherwise.
         *
         **/

        bool IsGM() const;

        /**
         *
         *  @brief Player current level.
         *
         **/

        virtual uint8_t getLevel () const;

        /**
         *
         *  @brief Level the player, increase available stats and str,int points.
         *
         **/

        void Levelup ();

        /**
         *
         *  @brief Player maximum level achieved.
         *
         **/

        uint8_t getMaxLv () const;

        /**
         *
         *	@brief Retrieve current inventory gold amount.
         *
         *	@retval Gold amount.
         *
         **/

        uint64_t getGold () const;

        /**
         *
         *  @brief Increase the player current gold amount.
         *
         *  @param amount Gold amount gained.
         *
         **/

        void addGold (const uint64_t amount);

        /**
         *
         *  @brief Decrease the player current gold amount.
         *
         *  @param amount: Gold amount lost.
         *
         *  @retval if the operation is allowed or denied.
         *
         **/

        bool subGold (const uint64_t amount);

        /**
         *
         *	@brief Store gold in the storage. Inventory amount gets decreased automaticly.
         *
         *	@param amount: Amount of gold to transfer.
         *
         *	@retval True if the operation was successful, false not enough gold.
         *
         **/

        bool depositGold (const uint64_t amount);

        /**
         *
         *	@brief Withdraw gold from the storage. Inventory amount gets increased automaticly.
         *
         *	@param amount: Amount of gold to transfer.
         *
         *	@retval True if the operation was successful, false not enough gold.
         *
         **/

        bool withdrawGold (const uint64_t amount);

        /**
         *
         *	@brief Retrieve current stored gold amount.
         *
         *	@retval Gold amount.
         *
         **/

        uint64_t getStoredGold () const;

        /**
         *
         *	@brief Retrieve accumulated stat points.
         *
         *	@retval Current stat points.
         *
         **/

        uint16_t getStatPoints () const;

        /**
         *
         *	@brief Use a stat point on a player attribute (INT or STR)
         *
         *	@param type: Stat point type.
         *
         *	@retval True if operation was successful, false not enough stat points.
         *
         **/

        bool useStatPoint (const PLAYER_STAT_POINT type);

        /**
         *
         *	@brief Get current player accumulated experience points.
         *
         *	@retval Experience points.
         *
         **/

        uint64_t getExp () const;

        /**
         *
         *  @brief Increase player experience points.
         *
         *  @param exp: Amount of experience gained.
         *
         **/

        void addExp (const uint64_t exp);

        /**
         *
         *	@brief Decrease player accumulated experience points.
         *
         *	@param exp: Experience points to decrease.
         *	@param prevExp: Experience points of the previous level.
         *
         **/

        void subExp (const uint64_t exp, const uint64_t prevExp);

        /**
         *
         *	@brief Retrieve player current accumulated skill experience points.
         *
         *	@retval Skill experience points.
         *
         **/

        uint16_t getSExp () const;

        /**
         *
         *  @brief Increase player skill experience points.
         *
         *  @param exp: Amount of skill experience points gained.
         *
         **/

        void addSExp (const uint32_t exp);

        /**
         *
         *	@brief Retrieve current accumulated skill points.
         *
         *	@retval Current SP.
         *
         **/

        uint32_t getSP () const;

        /**
         *
         *	@brief Decrease the player skill points amount.
         *
         *	@param sp: Skill Points to spend.
         *
         *	@retval if the operation is allowed or denied.
         *
         **/

        bool subSP (const uint32_t sp);

        /**
         *
         *  @brief Set player rank.
         *
         **/

        void setRank (const PLAYER_RANK rank);

        /**
         *
         *  @brief Return player current rank.
         *
         **/

        PLAYER_RANK getRank () const;

        /**
         *
         *  @brief Returns player current berserk count.
         *
         **/

        uint8_t getBerserkCounter () const;

        /**
         *
         *  @brief Increase berserk counter by one.
         *
         **/

        void incBerserkCounter ();

        /**
         *
         *	@brief Set berserk counter at maximum.
         *
         **/

        void fillBerserkCounter ();

        /**
         *
         *	@brief Set player berserker state.
         *
         *	@param activate: Seletected state.
         *
         **/

        void Berserk (bool activate);

        /**
         *
         *	@brief Set player return point refID.
         *
         *	@param ID: Teleport reference ID.
         *
         **/

        void setReturnID (const uint32_t ID);

        /**
         *
         *	@brief Retrieve player return point refID.
         *
         **/

        uint32_t getReturnID () const;

        /**
         *
         *	@brief Get displayed tutorial list (bitfields)
         *
         **/

        uint64_t getTutorials () const;

        /**
         *
         *	@brief Mark a tutorial dialog as displayed.
         *
         *	@param tutorial: Tutorial refID.
         *
         **/

        void addTutorial (const uint64_t tutorial);

        /**
         *
         *	@brief Check if a tutorial has been already displayed.
         *
         *	@param tutorial: Tutorial refID.
         *
         *	@retval True if has been displayed, false otherwise.
         *
         **/

        bool hasTutorial (const uint64_t tutorial);

        /**
         *
         *	@brief Morph player into another character.
         *
         *	@param property: Character property description and attributes.
         *	@param scale: Model scale.
         *
         **/

        void Morph (const boost::shared_ptr<EntityProperty> &property, const uint8_t scale);

        /**
         *
         *	@brief Add an icon to player display mask.
         *
         *	@param IconMask: Icon Mask to add. (bitfield)
         *
         **/

        void addIcon (const uint8_t IconMask);

        /**
         *
         *	@brief Remove an icon from player display mask.
         *
         *	@param IconMask: Icons to remove. (bitfield)
         *
         **/

        void removeIcon (const uint8_t IconMask);

        /**
         *
         *	@brief Get player current icon display mask.
         *
         **/

        uint8_t getIconMask () const;

        /**
         *
         *	@brief Set player daily pk count.
         *
         *	@param pk: Number of subjects PKed.
         *
         **/

        void setPKDaily (uint8_t pk);

        /**
         *
         *	@brief Get player PK count.
         *
         **/

        uint8_t getPKDaily () const;

        /**
         *
         *	@brief Get player current PK level.
         *
         **/

        uint16_t getPKLevel () const;

        /**
         *
         *	@brief Increase player kill points.
         *
         *	@param points: PK Points to add.
         *
         **/

        void addPKPoints (const uint32_t points);

        /**
         *
         *	@brief Decrease player kill points.
         *
         *	@param points: PK Points to substract.
         *
         **/

        void subPKPoints (const uint32_t points);

        /**
         *
         *	@brief Retrieve current PK points.
         *
         *	@retval PK Points.
         *
         **/

        uint32_t getPKPoints () const;

        /**
         *
         *	@brief Set player storage.
         *
         *	@param storage: Selected storage.
         *	@param type: Storage type to set.
         *
         **/

        void setStorage (const boost::shared_ptr<Storage> &storage, const LOCATION_TYPE type);

        /**
         *
         *  @brief Obtain the player inventory storage where it keep all items
         *  that belongs to him equipped or not.
         *
         *  @param type: The type of storage you want to retrieve.
         *
         *	@retval Selected storage type.
         *
         **/

        boost::shared_ptr<Storage> getStorage (const LOCATION_TYPE type);

        /**
         *
         *  @brief: Check if the inventory was expanded or not.
         *
         *	@retval True if the inventory was expanded, false otherwise.
         *
         **/

        bool IsInvExpanded() const;

        /**
         *
         *	@brief Increase Inventory max capacity.
         *
         *	@param nslots: Numbers of slots to add.
         *
         **/

        void IncInventorySlots (const uint8_t nslots);

        /**
         *
         *	@brief Increase Storage max capacity.
         *
         *	@param nslots: Number of slots to add.
         *
         **/

        void IncStorageSlots (const uint8_t nslots);

        /**
         *
         *  @brief: Check if the main storage was expanded or not.
         *
         *	@retval True if its expanded, false otherwise.
         *
         **/

        bool IsStorageExpanded () const;

        /**
         *
         *	@brief Retrieve main storage, current max slots.
         *
         *	@retval Max slots available.
         *
         **/

        uint8_t getStorageSlots () const;

        void set_mastery_tree (const MasteryTree& tree);

        MasteryTree& get_mastery_tree ();

        const MasteryTree& get_mastery_tree () const;

        /**
         *
         *	@brief Decrease player mastery by one level.
         *
         *	@param masteryID: Mastery to decrease.
         *
         *	@retval: if the operation is allowed or denied.
         *
         **/

        bool downgrade_mastery (const uint32_t masteryID);

        /**
         *
         *	@brief Retrieve mastery bonus for the selected mastery.
         *
         *	@param masteryID: Selected mastery to retrieve bonus.
         *
         *	@retval Current bonus provided by the mastery from 0 to 255.
         *
         **/

        int getMasteryBonus (const uint32_t masteryID) const;

        /**
         *
         *	@brief Set player available skills.
         *
         *	@param list: Skill refID list.
         *
         **/

        void setSkills (const std::set<uint32_t> &list);

        /**
         *
         *	@brief Obtain player available skills.
         *
         *	@retval skill refID list.
         *
         **/

        const std::set<uint32_t>& getSkills () const;

        /**
         *
         *	@brief Add a new skill to the player.
         *
         *	@param refID: Selected skill refID to add.
         *
         **/

        void addSkill (const uint32_t refID);

        /**
         *
         *	@brief Remove an existent skill.
         *
         *	@param refID: Selected skill refID to remove.
         *
         **/

        void removeSkill (const uint32_t refID);

        /**
         *
         *	@brief Check if the player has a skill or not.
         *
         *	@retval True if the skill exists, false otherwise.
         *
         **/

        bool hasSkill (const uint32_t refID) const;

        /**
         *
         *	@brief Get player base attack skill (depends on the equipped weapon).
         *
         *	@retval Base attack skill refID.
         *
         **/

        uint32_t getBaseAttack () const;

        /**
         *
         *	@brief Add a new passive buff to the player (Its effects always are taken into account).
         *
         *	@param passive: Selected passive buff.
         *
         **/

        void addPassive (const Buff &passive);

        /**
         *
         *	@brief Remove a selected passive and activates the next highest one.
         *
         *	@param groupID: Selected passive group refID.
         *	@param skillID: Selected passive skill refID.
         *
         **/

        void removePassive (const uint32_t groupID, const uint32_t skillID);

        /**
         *
         *	@brief Set character associated stall.
         *
         *	@param stall: Stall description.
         *
         **/

        void setStall (const boost::shared_ptr<Stall> &stall);

        /**
         *
         *	@brief Retrieve player current stall.
         *
         **/

        boost::shared_ptr<Stall> getStall () const;

        /**
         *
         *	@brief Set player stall avatar.
         *
         *	@param avatarID: Selected avatar refID.
         *
         **/

        void setStallAvatar (const uint32_t avatarID);

        /**
         *
         *	@brief Get player current stall avatar.
         *
         **/

        uint32_t getStallAvatar () const;

        /**
         *
         *	@brief Clear player stall.
         *
         **/

        void resetStall ();

        /**
         *
         *	@brief Set the type of invitation sent to the player.
         *
         **/

        void setInvitation (const INVITATION_TYPE type);

        /**
         *
         *	@brief Get current invitation type.
         *
         **/

        INVITATION_TYPE getInvitation () const;

        /**
         *
         *  @brief Set the player that your performing the invite process with.
         *
         *	@param GUID: Invitation player associated GUID.
         *
         **/

        void setInvitationPlayer (const uint32_t GUID);

        /**
         *
         *  @brief Return the player your exchanging with.
         *
         **/

        uint32_t getInvitationPlayer () const;

        /**
         *
         *	@brief Obtain player exchange window.
         *
         *	@retval Player current exchange window.
         *
         **/

        boost::shared_ptr<ExchangeWnd> getExchangeWindow () const;

        /**
         *
         *	@brief Set player current party.
         *
         *	@param party: Party object to set it to.
         *
         **/

        void setParty (const boost::shared_ptr<Party> &party);

        /**
         *
         *	@brief Retrieve player current party.
         *
         *	@retval Current party.
         *
         **/

        boost::shared_ptr<Party> getParty () const;

        /**
         *
         *	@brief Clear player current party.
         *
         **/

        void resetParty ();

        /**
         *
         *	@brief Retrieve player current guild refID.
         *
         **/

        uint32_t getGuildID () const;

        /**
         *
         *	@brief Set player guild.
         *
         *	@param guild: Selected guild to set.
         *
         **/

        void setGuild (const boost::shared_ptr<Guild> &guild, const uint32_t guildID);

        /**
         *
         *	@brief Obtain player current guild.
         *
         **/

        boost::shared_ptr<Guild> getGuild () const;

        /**
         *
         *	@brief Clear a player guild.
         *
         **/

        void resetGuild ();

        /**
         *
         *	@brief Add a cos to the player (Only insert 1 of each type).
         *
         *	@param cos: COS object to add.
         *
         **/

        void addCOS (const boost::shared_ptr<COS> &cos);

        /**
         *
         *	@brief Check if the player has a COS of the selected type.
         *
         *	@param type: Type of the cos to check.
         *
         *	@retval True if exists, false otherwise.
         *
         **/

        bool hasCOS (const PLAYER_COS type) const;

        /**
         *
         *	@brief Check if the player has a COS of the selected GUID.
         *
         *	@param cosID: COS GUID to check.
         *
         *	@retval: True if the COS exists, false otherwise.
         *
         **/

        bool hasCOS (const uint32_t cosID) const;

        /**
         *
         *	@brief Retrieve a COS by its GUID.
         *
         *	@param cosID: Selected COS associated GUID.
         *
         *	@retval COS object with the selected GUID, if not found its an invalid ptr.
         *
         **/

        boost::shared_ptr<COS> getCOS (const uint32_t cosID) const;

        /**
         *
         *	@brief Retrieve a COS by its type.
         *
         *	@param type: Selected COS associated type.
         *
         *	@retval COS object with the selected type, if not found its an invalid ptr.
         *
         **/

        boost::shared_ptr<COS> getCOS (const PLAYER_COS type) const;

        /**
         *
         *	@brief Retrieve all the active COS.
         *
         **/

        std::vector<boost::shared_ptr<COS> > getCOSs () const;

        /**
         *
         *	@brief Remove a COS from the active list.
         *
         *	@param cosID: Selected COS associated GUID.
         *
         **/

        void removeCOS (const uint32_t cosID);

        /**
         *
         *  @brief: Resurrect a dead player.
         *
         *  @param set_invulnerable: indicates is player is invunerable after resurrecting.
         *  @param max_recovery: indicates if the HP and MP must be fully recovered.
         *
         **/

        void Resurrect (bool setInvulnerable, bool maxRecovery);

        /**
         *
         *	@brief Set player job.
         *
         *	@param job: Job description.
         *
         **/

        void setJob (const Job &job);

        /**
         *
         *	@brief Retrieve player current job.
         *
         *	@retval Job description.
         *
         **/

        Job getJob () const;

        /**
         *
         *	@brief Check if the player has a job suit equipped.
         *
         *	@retval True if it has a job suit equipped, false otherwise.
         *
         **/

        bool IsJobbing () const;

        /**
         *
         *	@brief Get player current PvP team.
         *
         **/

        uint8_t PvPCape () const;

        /**
         *
         *	@brief Set player PvP team.
         *
         **/

        void setPvPCape (uint8_t cape);

        /**
         *
         *	@brief Set player equipping wait time.
         *
         *	@param time: Remaining time.
         *
         **/

        void setEquipWaitTime (uint8_t time);

        /**
         *
         *	@brief Get player remaining equip time.
         *
         **/

        uint8_t getEquipWaitTime () const;

        /**
         *
         *	@brief Establish current position as last recall point.
         *
         **/

        void EstablishRecallPoint ();

        /**
         *
         *	@brief Get player last recall point.
         *
         **/

        const Coord& GetRecallPoint () const;

        /**
         *
         *	@brief Set current position as death point.
         *
         **/

        void EstablishDeathPoint ();

        /**
         *
         *	@brief Obtain last death point position.
         *
         **/

        const Coord& GetDeathPoint () const;

        /**
         *
         *	@brief Check if the current task can be interrumped.
         *
         *	@retval True if its interruptable, false otherwise.
         *
         **/

        virtual bool IsTaskInterruptable () const;

        /**
         *
         *	@brief Set player current task with an NPC.
         *
         *	@param task: Task currently on process.
         *
         **/

        void setTaskNPC (const PLAYER_NPC_TASK task);

        /**
         *
         *	@brief Get player current task with an NPC.
         *
         **/

        PLAYER_NPC_TASK getTaskNPC () const;

        /**
         *
         *	@brief Check if player is currently busy talking/interacting with an NPC.
         *
         *
         *	@retval True if player is busy, false otherwise.
         *
         **/

        bool IsBusyNPC () const;

        /**
         *
         *	@brief Check if the player can be invited or not by another player.
         *
         *	@retval True the player is available to be invited.
         *
         **/

        bool IsInviteable () const;

        /**
         *
         *  @brief Indicates is a item group is usable by the player or is on cooldown.
         *
         *  @param type: Item group.
         *
         *	@retval: True if the item is available for use, false otherwise.
         *
         **/

        bool IsItemAvailable (const uint32_t itemType) const;

        /**
         *
         *  @brief Insert an item group into the cooldown list.
         *
         *  @param taskID: associated taskID in the world queue.
         *  @param itemType: Item group thats being added into cooldown.
         *
         **/

        void addItemCooldown (const uint32_t taskID, const uint32_t itemType);

        /**
         *
         *  @brief Remove an item group from the cooldown list.
         *
         *  @param itemType: Item group.
         *
         **/

        void removeItemCooldown (const uint32_t itemType);

        /**
         *
         *  @brief Obtain current item group cooldown list.
         *
         **/

        const std::map<uint32_t,uint32_t>& getItemCooldownList() const;

        /**
         *
         *	@brief Remove buffs that are not longed valid to use. (Use on equipment change mainly).
         *
         *	@retval List of the removed buffs [taskID,castID].
         *
         **/

        std::vector<std::pair<uint32_t,uint32_t> > removeInvalidatedBuffs ();

    protected:

        /**
         *
         *	@brief Calculate character base stats based on equipped items and passives.
         *
         *	@param bonus Processed bonus list for appending new bonus (ID,Value).
         *
         **/

        virtual void calculatePreStats (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus);

        virtual void calculateBuffEffects (std::vector<std::pair<CHAR_STATS,int32_t> > &bonus,
                                           const BuffEffect &effect);

        /**
         *
         *	@brief Calculate/Update character stats that depends on the other stats.
         *
         **/

        virtual void calculatePostStats ();

    protected:

        uint32_t m_AccountID;
        uint32_t m_ConnectionID;

        mutable boost::mutex m_player_mutex;

        sso::property::Player m_BaseChar;  ///< Player properties.
        std::string m_FakeName;

        boost::shared_ptr<Storage> m_InvStore;
        boost::shared_ptr<Storage> m_AvaStore;
        boost::shared_ptr<Storage> m_MainStore;

        MasteryTree m_mastery_tree;
        std::set<uint32_t> m_SkillList;

        Job m_Job;
        uint8_t m_EquipTime;

        mutable boost::mutex m_guild_mutex;
        boost::shared_ptr<Guild> m_Guild;

        mutable boost::mutex m_stall_mutex;
        boost::shared_ptr<Stall> m_Stall;

        mutable boost::mutex m_party_mutex;
        boost::shared_ptr<Party> m_Party;

        boost::shared_ptr<ExchangeWnd> m_ExchangeWnd;

        mutable boost::mutex m_cos_mutex;
        std::vector<boost::shared_ptr<COS> > m_cos;

        mutable boost::mutex m_passive_mutex;
        std::multimap<uint32_t,Buff> m_Passives;

        PLAYER_NPC_TASK m_TaskNPC;

        uint8_t m_IconMask;

        INVITATION_TYPE m_InvType;
        uint32_t m_InvTarget;

        Coord m_RecallPoint;    /// LAST TELEPORT RETURN POINT.
        Coord m_DeathPoint;     /// LAST DEATH POSITION.

        mutable boost::mutex m_item_mutex;
        std::map<uint32_t,uint32_t> m_cooldown_itemgroup;

    public:

        sso::Autopot m_Autopot;
        std::vector<Hotkey> m_Hotkeys;
        std::vector<std::string> m_BlockList;
    };

    /**
     *
     *  @brief Check if two player can interact with other in combat.
     *  While jobbing the traders with hunter and thief with thief.
     *
     *  @param p1: First player being checked.
     *  @param p2: Second player being checked.
     *
     **/

    bool CanPlayerInteract (const boost::shared_ptr<Player> &p1,
                            const boost::shared_ptr<Player> &p2, bool isAttack);

}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_PLAYER_H__
