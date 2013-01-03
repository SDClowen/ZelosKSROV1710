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

#include "item/item.h"

#include <khamaileon/property/item.h>

#include "utility/interpolate.h"

namespace sso
{
    Item::Item ()
        : m_Locked(false)
    {
        m_can_crit = false;

        m_PetStatus = 3;
        m_PetMinutes = 0;
    }

    Item::Item (const boost::shared_ptr<ItemProperty> &property)
        : m_reference(property),
        m_Locked(false)
    {
        m_property.quantity = 1;
        m_property.refID = m_reference->refID;

        m_PetStatus = 3;
        m_PetMinutes = 0;

        calculate_property();

        if (IsArmor() || IsWeapon() || IsShield())
            m_property.arg = m_reference->durability.first;
    }

    Item::Item (const boost::shared_ptr<ItemProperty> &reference,
                const sso::property::Item &property)
        : m_property(property),
        m_reference(reference),
        m_Locked(false)
    {
        m_PetStatus = 3;
        m_PetMinutes = 0;

        calculate_property();
    }

    void Item::ID (const uint64_t newID)
    {
        m_property.ID = newID;
    }

    uint64_t Item::ID () const
    {
        return m_property.ID;
    }

    boost::shared_ptr<ItemProperty> Item::property () const
    {
        return m_reference;
    }

    uint32_t Item::getRefID () const
    {
        return m_property.refID;
    }

    ITEM_TYPE Item::getType () const
    {
        return m_Type;
    }

    uint16_t Item::getTypeMask () const
    {
        return m_reference->GetTypeMask();
    }

    void Item::setLocation (const uint8_t location)
    {
        if (!m_Locked)
            m_property.location = location;
    }

    uint8_t Item::getLocation () const
    {
        return m_property.location;
    }

    void Item::setPosition (const uint8_t pos)
    {
        if (!m_Locked)
            m_property.position = pos;
    }

    uint8_t Item::getPosition () const
    {
        return m_property.position;
    }

    uint8_t Item::getDefaultPosition () const
    {
        uint8_t pos = 255;

        switch (m_Type)
        {
        case ITEM_CH_SWORD:
        case ITEM_CH_BLADE:
        case ITEM_CH_SPEAR:
        case ITEM_CH_BOW:
        case ITEM_CH_TBLADE:
        case ITEM_EU_SWORD:
        case ITEM_EU_TSWORD:
        case ITEM_EU_AXE:
        case ITEM_EU_DAGGER:
        case ITEM_EU_CROSSBOW:
        case ITEM_EU_HARP:
        case ITEM_EU_TSTAFF:
        case ITEM_EU_STAFF:
        case ITEM_EU_DARKSTAFF:
            pos = Item::WEAPON_SLOT;
            break;
        case ITEM_CH_RING:
        case ITEM_EU_RING:
            pos = Item::LEFT_RING_SLOT;
            break;
        case ITEM_CH_EARRING:
        case ITEM_EU_EARRING:
            pos = Item::EARRING_SLOT;
            break;
        case ITEM_CH_NECKLACE:
        case ITEM_EU_NECKLACE:
            pos = Item::NECKLACE_SLOT;
            break;
        case ITEM_CH_SHIELD:
        case ITEM_EU_SHIELD:
        case ITEM_ARROW:
        case ITEM_BOLT:
            pos = Item::SHIELD_SLOT;
            break;
        case ITEM_CH_HEAVY_HA:
        case ITEM_CH_LIGHT_HA:
        case ITEM_CH_CLOTHES_HA:
        case ITEM_EU_HEAVY_HA:
        case ITEM_EU_LIGHT_HA:
        case ITEM_EU_CLOTHES_HA:
            pos = Item::HEAD_SLOT;
            break;
        case ITEM_CH_HEAVY_SA:
        case ITEM_CH_LIGHT_SA:
        case ITEM_CH_CLOTHES_SA:
        case ITEM_EU_HEAVY_SA:
        case ITEM_EU_LIGHT_SA:
        case ITEM_EU_CLOTHES_SA:
            pos = Item::SHOULDER_SLOT;
            break;
        case ITEM_CH_HEAVY_BA:
        case ITEM_CH_LIGHT_BA:
        case ITEM_CH_CLOTHES_BA:
        case ITEM_EU_HEAVY_BA:
        case ITEM_EU_LIGHT_BA:
        case ITEM_EU_CLOTHES_BA:
            pos = Item::CHEST_SLOT;
            break;
        case ITEM_CH_HEAVY_LA:
        case ITEM_CH_LIGHT_LA:
        case ITEM_CH_CLOTHES_LA:
        case ITEM_EU_HEAVY_LA:
        case ITEM_EU_LIGHT_LA:
        case ITEM_EU_CLOTHES_LA:
            pos = Item::LEG_SLOT;
            break;
        case ITEM_CH_HEAVY_AA:
        case ITEM_CH_LIGHT_AA:
        case ITEM_CH_CLOTHES_AA:
        case ITEM_EU_HEAVY_AA:
        case ITEM_EU_LIGHT_AA:
        case ITEM_EU_CLOTHES_AA:
            pos = Item::HAND_SLOT;
            break;
        case ITEM_CH_HEAVY_FA:
        case ITEM_CH_LIGHT_FA:
        case ITEM_CH_CLOTHES_FA:
        case ITEM_EU_HEAVY_FA:
        case ITEM_EU_LIGHT_FA:
        case ITEM_EU_CLOTHES_FA:
            pos = Item::FOOT_SLOT;
            break;
        case ITEM_JOB_TRADER:
        case ITEM_JOB_THIEF:
        case ITEM_JOB_HUNTER:
        case ITEM_PVP_CAPE:
            pos = Item::JOB_CAPE_SLOT;
            break;
        default:
            break;
        }

        return pos;
    }

    void Item::Lock ()
    {
        m_Locked = true;
    }

    void Item::Unlock ()
    {
        m_Locked = false;
    }

    bool Item::IsLocked ()
    {
        return m_Locked;
    }

    uint16_t Item::getQuantity () const
    {
        return m_property.quantity;
    }

    void Item::setQuantity (const uint16_t quantity)
    {
        m_property.quantity = quantity;
    }

    uint16_t Item::getMaxStack () const
    {
        return m_reference->MaxStack;
    }

    void Item::setEnchantLv (const uint8_t lv)
    {
        m_property.enchantLv = lv;
    }

    uint8_t Item::getEnchantLv () const
    {
        return m_property.enchantLv;
    }

    float Item::attack_distance () const
    {
        return m_reference->attk_dist;
    }

    uint16_t Item::physical_defense () const
    {
        return (uint16_t)calculate(0,m_property.enchantLv,m_reference->pd_lv,
                                   m_reference->pd);
    }

    uint16_t Item::magical_defense () const
    {
        return (uint16_t)calculate(0,m_property.enchantLv,m_reference->md_lv,
                                   m_reference->md);
    }

    uint16_t Item::hit_ratio () const
    {
        return m_reference->ar.first;
    }

    uint16_t Item::parry_ratio () const
    {
        return m_reference->pr;
    }

    float Item::physical_absorb () const
    {
        return calculate(0,m_property.enchantLv,m_reference->pa_lv,
                         m_reference->pa);
    }

    float Item::magical_absorb () const
    {
        return calculate(0,m_property.enchantLv,m_reference->ma_lv,
                         m_reference->ma);
    }

    std::pair<uint16_t,uint16_t> Item::physical_attack () const
    {
        std::pair<uint16_t,uint16_t> atk;

        atk.first = calculate(0,m_property.enchantLv,(uint16_t)m_reference->patk_lv,
                              m_reference->min_patk);

        atk.second = calculate(0,m_property.enchantLv,(uint16_t)m_reference->patk_lv,
                               m_reference->max_patk);

        return atk;
    }

    std::pair<uint16_t,uint16_t> Item::magical_attack () const
    {
        std::pair<uint16_t,uint16_t> atk;

        atk.first = calculate(0,m_property.enchantLv,(uint16_t)m_reference->matk_lv,
                              m_reference->min_matk);

        atk.second = calculate(0,m_property.enchantLv,(uint16_t)m_reference->matk_lv,
                               m_reference->max_matk);
        return atk;
    }

    std::pair<uint16_t,uint16_t> Item::physical_reinforce () const
    {
        std::pair<uint16_t,uint16_t> reinforce;

        reinforce.first = m_reference->min_phyref.first;

        reinforce.second = m_reference->max_phyref.first;

        return reinforce;
    }

    std::pair<uint16_t,uint16_t> Item::magical_reinforce () const
    {
        std::pair<uint16_t,uint16_t> reinforce;

        reinforce.first = m_reference->min_magref.first;

        reinforce.second = m_reference->max_magref.first;

        return reinforce;
    }

    uint8_t Item::critical_ratio () const
    {
        return m_reference->crit.first;
    }

    uint8_t Item::block_ratio () const
    {
        return m_reference->br.first;
    }

    uint64_t Item::encoded_mods () const
    {
        return m_property.mods;
    }

    GENDER Item::getGender () const
    {
        return static_cast<GENDER>(m_reference->Gender);
    }

    uint8_t Item::getDegree () const
    {
        return m_reference->Degree;
    }

    uint8_t Item::getQuality () const
    {
        return m_reference->Sox;
    }

    void Item::set_assimilation (const uint8_t assimilation)
    {
        m_property.arg = assimilation;
    }

    uint8_t Item::get_assimilation () const
    {
        return m_property.arg;
    }

    void Item::set_pet_name (const std::string &name)
    {
        m_PetName = name;
    }

    void Item::set_pet_npc_id (const uint32_t ID)
    {
        m_PetNPC = ID;
    }

    void Item::set_pet_status(uint8_t status)
    {
        m_PetStatus = status;
    }

    void Item::set_pet_duration (const uint32_t minutes)
    {
        m_PetMinutes = minutes;
    }

    void Item::set_pet_id (const uint32_t ID)
    {
        m_property.cos.ID = ID;
    }

    const std::string& Item::get_pet_name() const
    {
        return m_PetName;
    }

    uint32_t Item::get_pet_npc_id () const
    {
        return m_PetNPC;
    }

    uint8_t Item::get_pet_status() const
    {
        return m_PetStatus;
    }

    uint32_t Item::get_pet_duration() const
    {
        return m_PetMinutes;
    }

    uint32_t Item::get_pet_id () const
    {
        return m_property.cos.ID;
    }

    bool Item::IsWeapon () const
    {
        bool weapon = false;

        switch(getType())
        {
        case ITEM_CH_SWORD:
        case ITEM_CH_BLADE:
        case ITEM_CH_BOW:
        case ITEM_CH_SPEAR:
        case ITEM_CH_TBLADE:
        case ITEM_EU_SWORD:
        case ITEM_EU_TSWORD:
        case ITEM_EU_AXE:
        case ITEM_EU_STAFF:
        case ITEM_EU_TSTAFF:
        case ITEM_EU_DARKSTAFF:
        case ITEM_EU_DAGGER:
        case ITEM_EU_CROSSBOW:
        case ITEM_EU_HARP:
            weapon = true;
            break;
        default:
            break;
        }

        return weapon;
    }

    bool Item::IsShield () const
    {
        bool shield = false;

        switch(getType())
        {
        case ITEM_CH_SHIELD:
        case ITEM_EU_SHIELD:
            shield = true;
            break;
        default:
            break;
        }

        return shield;
    }

    bool Item::IsArmor () const
    {
        bool armor = false;

        switch(getType())
        {
        case ITEM_CH_HEAVY_HA:
        case ITEM_CH_HEAVY_SA:
        case ITEM_CH_HEAVY_BA:
        case ITEM_CH_HEAVY_LA:
        case ITEM_CH_HEAVY_AA:
        case ITEM_CH_HEAVY_FA:
        case ITEM_CH_LIGHT_HA:
        case ITEM_CH_LIGHT_SA:
        case ITEM_CH_LIGHT_BA:
        case ITEM_CH_LIGHT_LA:
        case ITEM_CH_LIGHT_AA:
        case ITEM_CH_LIGHT_FA:
        case ITEM_CH_CLOTHES_HA:
        case ITEM_CH_CLOTHES_SA:
        case ITEM_CH_CLOTHES_BA:
        case ITEM_CH_CLOTHES_LA:
        case ITEM_CH_CLOTHES_AA:
        case ITEM_CH_CLOTHES_FA:
        case ITEM_EU_HEAVY_HA:
        case ITEM_EU_HEAVY_SA:
        case ITEM_EU_HEAVY_BA:
        case ITEM_EU_HEAVY_LA:
        case ITEM_EU_HEAVY_AA:
        case ITEM_EU_HEAVY_FA:
        case ITEM_EU_LIGHT_HA:
        case ITEM_EU_LIGHT_SA:
        case ITEM_EU_LIGHT_BA:
        case ITEM_EU_LIGHT_LA:
        case ITEM_EU_LIGHT_AA:
        case ITEM_EU_LIGHT_FA:
        case ITEM_EU_CLOTHES_HA:
        case ITEM_EU_CLOTHES_SA:
        case ITEM_EU_CLOTHES_BA:
        case ITEM_EU_CLOTHES_LA:
        case ITEM_EU_CLOTHES_AA:
        case ITEM_EU_CLOTHES_FA:
            armor = true;
            break;
        default:
            break;
        }

        return armor;
    }

    bool Item::IsAccessory () const
    {
        bool acc = false;

        switch(getType())
        {
        case ITEM_CH_RING:
        case ITEM_CH_EARRING:
        case ITEM_CH_NECKLACE:
        case ITEM_EU_RING:
        case ITEM_EU_EARRING:
        case ITEM_EU_NECKLACE:
            acc = true;
            break;
        default:
            break;
        }

        return acc;
    }

    bool Item::IsCape () const
    {
        bool cape = false;

        switch(getType())
        {
        case ITEM_PVP_CAPE:
        case ITEM_JOB_TRADER:
        case ITEM_JOB_THIEF:
        case ITEM_JOB_HUNTER:
            cape = true;
            break;
        default:
            break;
        }

        return cape;
    }

    bool Item::IsAlchemyStone () const
    {
        return m_Type >= ITEM_ALCHEMY_MAGIC_STONE && m_Type <= ITEM_ALCHEMY_ATTR_STONE;
    }

    bool Item::IsPet () const
    {
        return m_Type >= ITEM_COS_PET && m_Type <= ITEM_COS_GRAB_PET;
    }

    bool Item::IsAmmo () const
    {
        return m_Type == ITEM_ARROW || m_Type == ITEM_BOLT;
    }

    bool Item::IsAvatar () const
    {
        return m_Type >= ITEM_AVATAR_ATTACH && m_Type <= ITEM_AVATAR_HAT;
    }

    bool Item::IsEquippable () const
    {
        return ((m_Type >= ITEM_CH_SWORD && m_Type <= ITEM_AVATAR_HAT) ||
                (m_Type >= ITEM_JOB_TRADER && m_Type <= ITEM_PVP_CAPE));
    }

    bool Item::IsStackable () const
    {
        return m_reference->MaxStack > 1;
    }

    bool Item::IsTradeable() const
    {
        return m_reference->Authority & ITM_AUTH_TRADEABLE;
    }

    bool Item::IsStorageable () const
    {
        return m_reference->Authority & ITM_AUTH_STORAGEABLE;
    }

    bool Item::IsRepairable () const
    {
        return m_reference->durability.first;
    }

    bool Item::IsUseable_Level (const uint8_t level) const
    {
        for (int i = 0; i < MAX_ITEM_REQUIREMENTS; ++i)
        {
            if (m_reference->Requirements[i].first == 1 &&
                m_reference->Requirements[i].second <= level)
                return true;

            if (i == 0 && m_reference->Requirements[i].first == -1)
                return true;
        }

        return false;
    }

    bool Item::IsUseable_Mastery (const std::pair<uint32_t,uint8_t> &mastery) const
    {
        for (int i = 0; i < MAX_ITEM_REQUIREMENTS; ++i)
        {
            if (m_reference->Requirements[i].first == static_cast<int32_t>(mastery.first) &&
                m_reference->Requirements[i].second <= mastery.second)
                return true;

            if (i == 0 && m_reference->Requirements[i].first == -1)
                return true;
        }

        return false;
    }

    bool Item::IsUseable_Race (const RACE race) const
    {
        if (m_reference->Race == RACE_ANY || m_reference->Race == race)
            return true;

        return false;
    }

    bool Item::CanCrit () const
    {
        return m_can_crit;
    }

    void Item::Repair ()
    {
        m_property.arg = getMaxDurability();
    }

    uint32_t Item::RepairCost () const
    {
        /*
        CH BOW D1 - 10/47 repair cost = 171
        CH BOW D1 - 0/47 repair cost = 540
        CH BOW D1 - 46/47 repair cost = 4
        CH BOW D1 - 45/47 repair cost = 9
        */

        return 5*getDegree()*(getMaxDurability() - m_property.arg);
    }

    void Item::setDurability (const uint32_t durability)
    {
        m_property.arg = durability;
    }

    uint32_t Item::getDurability () const
    {
        return m_property.arg;
    }

    uint32_t Item::getMaxDurability () const
    {
        return m_reference->durability.first;
    }

    void Item::addMagicOption (const std::pair<uint32_t,uint32_t> &attr)
    {
        m_property.magicoptions.push_back(attr);
    }

    bool Item::IsOptsMax () const
    {
        return m_property.magicoptions.size() == m_reference->MaxOpts;
    }

    std::vector<Item::attribute> Item::getMagicOpts () const
    {
        return m_property.magicoptions;
    }

    void Item::Morph (const boost::shared_ptr<ItemProperty> &property)
    {
        m_reference = property;
    }

    void Item::calculate_property ()
    {
        calculate_type();

        switch(getType())
        {
        case ITEM_CH_SWORD:
        case ITEM_CH_BLADE:
        case ITEM_CH_BOW:
        case ITEM_CH_SPEAR:
        case ITEM_CH_TBLADE:
        case ITEM_EU_SWORD:
        case ITEM_EU_TSWORD:
        case ITEM_EU_AXE:
        case ITEM_EU_DAGGER:
        case ITEM_EU_CROSSBOW:
            m_can_crit = true;
            break;
        default:
            m_can_crit = false;
            break;
        }
    }

    void Item::calculate_type ()
    {
        m_Type = m_reference->GetType();
    }
}
