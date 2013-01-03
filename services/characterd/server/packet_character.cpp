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

#include "packet_character.h"

#include <boost/bind.hpp>

#include <net/opcode/chard_srv.h>
#include <packet/packet.h>
#include <sso/property/character.h>

namespace srv_pkt
{
    void WriteStorage (OPacket *pkt, const uint8_t maxCapacity,
                       const std::vector<sso::property::Item> &items);

    void WriteItem (OPacket *pkt, const sso::property::Item &item);

    void WriteAttr (OPacket *pkt, const std::vector<std::pair<uint32_t,uint32_t> > &attr);

    void WriteHotkey (OPacket *pkt, const sso::Hotkey &hotkey);

    void WriteAutopot (OPacket *pkt, const sso::Autopot &setting);

    void CharacterInfo (OPacket *pkt, const uint32_t accountID, const sso::CharacterInfo *character)
    {
        typedef std::vector<std::pair<uint32_t,uint8_t> >::const_iterator mastery_const_iterator;

        pkt->WriteOpcode(SRV_CHARD_CHARACTER);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint64_t>(character->ID);

        pkt->Write<uint32_t>(character->RefCharID);
        pkt->Write<uint8_t>(character->Volume);
        pkt->Write<uint8_t>(character->Level);
        pkt->Write<uint8_t>(character->MaxLevel);
        pkt->Write<uint64_t>(character->Exp);
        pkt->Write<uint32_t>(character->SPExp);
        pkt->Write<uint64_t>(character->Gold);
        pkt->Write<uint64_t>(character->GoldStored);
        pkt->Write<uint32_t>(character->SP);
        pkt->Write<uint16_t>(character->Stats);
        pkt->Write<uint16_t>(character->Str);
        pkt->Write<uint16_t>(character->Int);
        pkt->Write<uint8_t>(character->BskCount);
        pkt->Write<uint32_t>(character->HP);
        pkt->Write<uint32_t>(character->MP);
        pkt->Write<uint8_t>(character->PKDaily);
        pkt->Write<uint16_t>(character->PKLevel);
        pkt->Write<uint32_t>(character->PKPoints);
        pkt->Write<uint8_t>(character->Rank);
        pkt->Write<uint8_t>(character->pvpCape);

        /// Item Information
        WriteStorage(pkt,character->InvSlots,character->inventory);
        WriteStorage(pkt,MAX_AVATAR_SLOTS,character->avatar);
        WriteStorage(pkt,character->StoreSlots,character->storage);

        pkt->Write<uint8_t>(character->masteries.size());

        /// Mastery Information
        for (mastery_const_iterator it = character->masteries.begin();
                it != character->masteries.end(); ++it)
        {
            pkt->Write<uint32_t>(it->first);
            pkt->Write<uint8_t>(it->second);
        }

        /// Skill Information

        pkt->Write<uint32_t>(character->skills.size());

        for (std::set<uint32_t>::const_iterator it = character->skills.begin();
                it != character->skills.end(); ++it)
            pkt->Write<uint32_t>(*it);

        pkt->Write<int16_t>(character->Zone);
        pkt->Write(character->X);
        pkt->Write(character->Y);
        pkt->Write(character->Z);

        pkt->Write<uint16_t>(character->Angle);

        pkt->Write<uint16_t>(character->buffs.size());

        for (size_t j = 0; j < character->buffs.size(); ++j)
        {
            pkt->Write<uint32_t>(character->buffs[j].first);
            pkt->Write<uint32_t>(character->buffs[j].second);
        }

        pkt->Write(character->Name);

        /// Job Information

        pkt->Write(character->job.Alias);
        pkt->Write<uint8_t>(character->job.Type);
        pkt->Write<uint8_t>(character->job.Level);
        pkt->Write<uint64_t>(character->job.Experience);

        pkt->Write<uint64_t>(character->Tutorials);
        pkt->Write<uint8_t>(character->GM);

        pkt->Write<uint8_t>(character->hotkeys.size());
        std::for_each(character->hotkeys.begin(),character->hotkeys.end(),
                      boost::bind(WriteHotkey,pkt,_1));

        WriteAutopot(pkt,character->autopot);

        pkt->Write<uint8_t>(character->blocks.size());
        for ( std::vector<std::string>::const_iterator i = character->blocks.begin();
                i != character->blocks.end(); ++i)
            pkt->Write(*i);
    }

    void WriteStorage (OPacket *pkt, const uint8_t maxCapacity,
                       const std::vector<sso::property::Item> &items)
    {
        typedef std::vector<sso::property::Item>::const_iterator item_const_iterator;

        pkt->Write<uint8_t>(maxCapacity);
        pkt->Write<uint8_t>(items.size());

        for (item_const_iterator iter = items.begin(); iter != items.end(); ++iter )
            WriteItem(pkt,*iter);
    }

    void WriteItem (OPacket *pkt, const sso::property::Item &item)
    {
        pkt->Write<uint64_t>(item.ID);
        pkt->Write<uint32_t>(item.refID);
        pkt->Write<uint8_t>(item.position);
        pkt->Write<uint8_t>(item.enchantLv);
        pkt->Write<uint16_t>(item.quantity);
        pkt->Write<uint64_t>(item.mods);
        pkt->Write<uint32_t>(item.arg);

        pkt->Write<uint8_t>(item.cos.ID ? true : false);

        if (item.cos.ID)
        {
            pkt->Write<uint32_t>(item.cos.ID);
            pkt->Write<uint8_t>(item.cos.state);
            pkt->Write(item.cos.name);
            pkt->Write<uint32_t>(item.cos.HP);
            pkt->Write<uint16_t>(item.cos.HGP);
            pkt->Write<uint64_t>(item.cos.XP);
            pkt->Write<uint32_t>(item.cos.Minutes);
            pkt->Write<uint32_t>(item.cos.settings);
        }

        WriteAttr(pkt,item.magicoptions);
    }

    void WriteAttr (OPacket *pkt, const std::vector<std::pair<uint32_t,uint32_t> > &attr)
    {
        pkt->Write<uint8_t>(attr.size());

        for (size_t i = 0; i < attr.size(); ++i )
        {
            pkt->Write<uint32_t>(attr[i].first);
            pkt->Write<uint32_t>(attr[i].second);
        }
    }

    void WriteHotkey (OPacket *pkt, const sso::Hotkey &hotkey)
    {
        pkt->Write<uint8_t>(hotkey.pos);
        pkt->Write<uint8_t>(hotkey.type);
        pkt->Write<uint32_t>(hotkey.id);
    }

    void WriteAutopot (OPacket *pkt, const sso::Autopot &setting)
    {
        pkt->Write<uint8_t>((setting.hp_bar << 4) | setting.hp_slot);
        pkt->Write<uint8_t>((setting.hp_active << 7) | setting.hp_value);
        pkt->Write<uint8_t>((setting.mp_bar << 4) | setting.mp_slot);
        pkt->Write<uint8_t>((setting.mp_active << 7) | setting.mp_value);
        pkt->Write<uint8_t>((setting.pill_bar << 4) | setting.pill_slot);
        pkt->Write<uint8_t>(setting.pill_active << 7);
        pkt->Write<uint8_t>((setting.delay_active << 7) | setting.delay_value);
    }
}
