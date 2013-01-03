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

#ifndef __ZELOS_LIBSSO_SSO_ITEM_ITEM_H__
#define __ZELOS_LIBSSO_SSO_ITEM_ITEM_H__

#include <vector>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include <khamaileon/property/item_type.h>
#include <sso/property/item.h>
#include <sso/types.h>

class ItemProperty;

namespace sso
{
    class Item
    {
    public:

        typedef std::pair<uint32_t,uint32_t> attribute;

        /// Default item position in player inventory.

        enum
        {
            HEAD_SLOT,
            CHEST_SLOT ,
            SHOULDER_SLOT,
            HAND_SLOT,
            LEG_SLOT,
            FOOT_SLOT,
            WEAPON_SLOT,
            SHIELD_SLOT,
            JOB_CAPE_SLOT,
            EARRING_SLOT,
            NECKLACE_SLOT,
            LEFT_RING_SLOT,
            RIGHT_RING_SLOT,
            MAX_EQUIPMENT_SLOT
        };

        Item ();

        Item (const boost::shared_ptr<ItemProperty> &reference);

        Item (const boost::shared_ptr<ItemProperty> &reference,
              const sso::property::Item &property);

        /**
         *
         *	@brief Set item database reference ID.
         *
         *	@param newID: Database associated ID.
         *
         **/

        void ID (const uint64_t newID);

        /**
         *
         *	@brief Get item associated database ID.
         *
         **/

        uint64_t ID () const;

        /**
         *
         *	@brief Get item base properties.
         *
         **/

        boost::shared_ptr<ItemProperty> property () const;

        /**
         *
         *	@brief Get Item reference ID (itemdata.txt)
         *
         **/

        uint32_t getRefID () const;

        /**
         *
         *	@brief Get item type.
         *
         **/

        ITEM_TYPE getType () const;

        /**
         *
         *	@brief Returns item packed types
         *
         **/

        uint16_t getTypeMask () const;

        /**
         *
         *  @brief Return Item gender.
         *
         **/

        GENDER getGender () const;

        /**
         *
         *	@brief Get item degree.
         *
         **/

        uint8_t getDegree () const;

        /**
         *
         *	@brief Get item quality (SOX,Honor,Nova,etc)
         *
         **/

        uint8_t getQuality () const;

        /**
         *
         *	@brief Change item storage location.
         *
         *	@param location: Selected location where the item is stored.
         *
         **/

        void setLocation (const uint8_t location);

        /**
         *
         *	@brief Obtain item current stored location.
         *
         **/

        uint8_t getLocation () const;

        /**
         *
         *	@brief Set item position.
         *
         *	@param pos: Selected position for the item.
         *
         **/

        void setPosition (const uint8_t pos);

        /**
         *
         *	@brief Get item current position.
         *
         **/

        uint8_t getPosition () const;

        /**
         *
         *	@brief Retrieve item default equip position.
         *
         *	@retval Default position, 255 if the item isnt equipable.
         *
         **/

        uint8_t getDefaultPosition () const;

        /**
         *
         *	@brief Lock item position and location.
         *
         **/

        void Lock ();

        /**
         *
         *	@brief Unlock item postion and location.
         *
         **/

        void Unlock ();

        /**
         *
         *	@brief Check if the item is locked or not.
         *
         *	@retval True if the item is locked, false otherwise.
         *
         **/

        bool IsLocked ();

        /**
         *
         *	@brief Get current item quantity.
         *
         **/

        uint16_t getQuantity () const;

        /**
         *
         *	@brief Set item quantity.
         *
         *	@param quantity: Selected quantity.
         *
         **/

        void setQuantity (const uint16_t quantity);

        /**
         *
         *	@brief Get item max stack size.
         *
         **/

        uint16_t getMaxStack () const;

        /**
         *
         *	@brief Set item enchanting level (+)
         *
         *	@param lv: Selected enchant level.
         *
         **/

        void setEnchantLv (const uint8_t lv);

        /**
         *
         *	@brief Get item current enchant level.
         *
         **/

        uint8_t getEnchantLv () const;

        float attack_distance () const;

        uint16_t physical_defense () const;

        uint16_t magical_defense () const;

        float physical_absorb () const;

        float magical_absorb () const;

        uint16_t hit_ratio () const;

        uint16_t parry_ratio () const;

        uint8_t critical_ratio () const;

        uint8_t block_ratio () const;

        std::pair<uint16_t,uint16_t> physical_attack () const;

        std::pair<uint16_t,uint16_t> magical_attack () const;

        std::pair<uint16_t,uint16_t> physical_reinforce () const;

        std::pair<uint16_t,uint16_t> magical_reinforce () const;

        uint64_t encoded_mods () const;

        void set_physical_percent (int percent);

        void set_magical_percent (int percent);

        void set_durability_percent (int percent);

        void set_ap_ratio_percent (int percent);

        void set_cb_ratio_percent (int percent);

        void set_physical_reinf_percent (int percent);

        void set_magical_reinf_percent (int percent);

        /**
         *
         *  @brief Returns Physical Percent.
         *  Weapon: physical attack power.
         *  Shield and Armor: physical defense.
         *  Accessory: physical absorb.
         *
         **/

        int get_physical_percent () const;

        /**
         *
         *  @brief Returns Magical Percent.
         *  Weapon: Magical attack power.
         *  Shield: Magical defense.
         *  Accessory: Magical Absorb.
         *
         **/

        int get_magical_percent () const;

        /**
         *
         *  @brief Returns durability percent.
         *
         **/

        int get_durability_percent () const;

        int get_physical_reinf_percent () const;

        int get_magical_reinf_percent () const;

        /**
         *
         *  @brief Returns Attack Rating or Parry Ratio percent.
         *  Weapon: Attack rating.
         *  Armor: Parry Ratio.
         *
         **/

        int get_ap_ratio_percent () const;

        /**
         *
         *  @brief Returns critical or Blocking ratio percent.
         *  Weapon: Critical.
         *  Shield: Blocking Ratio.
         *
         **/

        int get_cb_ratio_percent () const;

        /*---- STONE FUNCTIONS ---- */

        void set_assimilation (const uint8_t percent);

        uint8_t get_assimilation () const;

        /*---- PET FUNCTIONS ---- */

        void set_pet_name (const std::string &name);

        void set_pet_npc_id (const uint32_t ID);

        void set_pet_status (const uint8_t status);

        void set_pet_duration (const uint32_t minutes);

        void set_pet_id (const uint32_t ID);

        const std::string& get_pet_name () const;

        uint32_t get_pet_npc_id () const;

        uint8_t get_pet_status () const;

        uint32_t get_pet_duration () const;

        uint32_t get_pet_id () const;

        /**
         *
         *	@brief Check if the item is a weapon.
         *
         *	@retval True if its a weapon, false otherwise.
         *
         **/

        bool IsWeapon () const;

        /**
         *
         *	@brief Check if the item belongs to the shield group.
         *
         *	@retval True if its a shield, false otherwise.
         *
         **/

        bool IsShield () const;

        /**
         *
         *	@brief Check if the item belongs to the armor group.
         *
         *	@retval True if its an armor, false otherwise.
         *
         **/

        bool IsArmor () const;

        /**
         *
         *	@brief Check if the item belongs to the accessory group.
         *
         *	@retval True if its an accessory, false otherwise.
         *
         **/

        bool IsAccessory () const;

        /**
         *
         *	@brief Check if the items is a job suit/cape.
         *
         *	@retval True if its a job suit/cape, false otherwise.
         *
         **/

        bool IsCape () const;

        /**
         *
         *	@brief Check if the items belongs to the alchemy stone group.
         *
         *	@retval True if its an alchemy stone, false otherwise.
         *
         **/

        bool IsAlchemyStone () const;

        /**
         *
         *	@brief Check if the item is a pet.
         *
         *	@retval True if its a pet, false otherwise.
         *
         **/

        bool IsPet () const;

        /**
         *
         *	@brief Check if the item is ammo.
         *
         *	@retval True if its ammo, false otherwise.
         *
         **/

        bool IsAmmo () const;

        /**
         *
         *	@brief Check if the item is an avatar.
         *
         *	@retval True if its an avatar, false otherwise.
         *
         **/

        bool IsAvatar () const;

        /**
         *
         *	@brief Check if the item can be equipped by an user.
         *
         *	@retval True if its equipable, false otherwise.
         *
         **/

        bool IsEquippable () const;

        /**
         *
         *	@brief Check if the item can be stacked.
         *
         *	@retval True if the item is stackable, false otherwise.
         *
         **/

        bool IsStackable () const;

        /**
         *
         *	@brief Check if the item is tradeable.
         *
         *	@retval True if the item can be traded with another user, false otherwise.
         *
         **/

        bool IsTradeable () const;

        /**
         *
         *	@brief Check if the item can be stored.
         *
         *	@retval True if it can be stored, false otherwise.
         *
         **/

        bool IsStorageable () const;

        /**
         *
         *	@brief Check if the item can be repaired.
         *
         *	@retval True if its repairable, false otherwise.
         *
         **/

        bool IsRepairable () const;

        /**
         *
         *	@brief Check if usable by a the selected level.
         *
         *	@param level: Current user level.
         *
         *	@retval True if its usable, false otherwise.
         *
         **/

        bool IsUseable_Level (const uint8_t level) const;

        /**
         *
         *	@brief Check if its usable by the selected mastery.
         *
         *	@param mastery: Mastery to check.
         *
         *	@retval True if its usable, false otherwise.
         *
         **/

        bool IsUseable_Mastery (const std::pair<uint32_t,uint8_t> &mastery) const;

        /**
         *
         *	@brief Check if its usable by the selected race.
         *
         *	@param race: Race to check.
         *
         *	@retval True if its useable, false otherwise.
         *
         **/

        bool IsUseable_Race (const RACE race) const;

        /**
         *
         *	@brief Check if the item can perform critical hits (Mainly for weapons).
         *
         *	@retval True if the item can perform a critical hit, false otherwise.
         *
         **/

        bool CanCrit () const;

        /**
         *
         *	@brief Restore item durability.
         *
         **/

        void Repair ();

        /**
         *
         *	@brief Obtain item repair costs.
         *
         **/

        uint32_t RepairCost () const;

        /**
         *
         *	@brief Set item remaining durability.
         *
         *	@param durability: Remaining durability.
         *
         **/

        void setDurability (const uint32_t durability);

        /**
         *
         *	@brief Get item current durability.
         *
         **/

        uint32_t getDurability () const;

        /**
         *
         *	@brief Get item max allowed durability.
         *
         **/

        uint32_t getMaxDurability () const;

        /**
         *
         *	@brief Insert a new magic option to the item.
         *
         *	@param attr: Magic option to insert.
         *
         **/

        void addMagicOption (const std::pair<uint32_t,uint32_t> &attr);

        /**
         *
         *	@brief Check if the item magic options slots are all taken.
         *
         *	@retval True if its full, false otherwise.
         *
         **/

        bool IsOptsMax () const;

        /**
         *
         *	@brief Get items magic options list.
         *
         **/

        std::vector<attribute> getMagicOpts () const;

        void Morph (const boost::shared_ptr<ItemProperty> &property);

    private:

        void calculate_property ();

        void calculate_type ();

    private:

        ITEM_TYPE m_Type;

        sso::property::Item m_property;
        boost::shared_ptr<ItemProperty> m_reference;

        std::string m_PetName;
        uint32_t m_PetNPC;
        uint8_t m_PetStatus;
        uint32_t m_PetMinutes;

        bool m_can_crit;

        bool m_Locked;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ITEM_ITEM_H__
