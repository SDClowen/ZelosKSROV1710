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

#include "characterd/state_chard.h"

#include <name/services.h>
#include <net/opcode/chard_srv.h>
#include <sso/property/character.h>

#include "service.h"

void ParseStorage (const IPacket &packet, uint8_t *capacity, std::vector<sso::property::Item> *items);

void ParseItem (const IPacket &packet, std::vector<sso::property::Item> *items);

void ParseAutopot (const IPacket &packet, sso::Autopot *setting);

namespace chard
{
    StateChard::StateChard (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_CHARD_CHARACTER] = boost::bind(&StateChard::OnCharacter,this,_1);
    }

    StateChard::~StateChard ()
    {
    }

    void StateChard::Close ()
    {
    }

    int StateChard::OnCharacter (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        boost::shared_ptr<sso::CharacterInfo> character(new sso::CharacterInfo());

        if (character)
        {
            character->ID = packet.Read<uint64_t>();
            character->RefCharID = packet.Read<uint32_t>();
            character->Volume = packet.Read<uint8_t>();
            character->Level = packet.Read<uint8_t>();
            character->MaxLevel = packet.Read<uint8_t>();
            character->Exp = packet.Read<uint64_t>();
            character->SPExp = packet.Read<uint32_t>();
            character->Gold = packet.Read<uint64_t>();
            character->GoldStored = packet.Read<uint64_t>();
            character->SP = packet.Read<uint32_t>();
            character->Stats = packet.Read<uint16_t>();
            character->Str = packet.Read<uint16_t>();
            character->Int = packet.Read<uint16_t>();
            character->BskCount = packet.Read<uint8_t>();
            character->HP = packet.Read<uint32_t>();
            character->MP = packet.Read<uint32_t>();
            character->PKDaily = packet.Read<uint8_t>();
            character->PKLevel = packet.Read<uint16_t>();
            character->PKPoints = packet.Read<uint32_t>();
            character->Rank = packet.Read<uint8_t>();
            character->pvpCape = packet.Read<uint8_t>();

            ParseStorage(packet,&character->InvSlots,&character->inventory);
            ParseStorage(packet,NULL,&character->avatar);
            ParseStorage(packet,&character->StoreSlots,&character->storage);

            uint8_t mast_size = packet.Read<uint8_t>();

            std::pair<uint32_t,uint8_t> mast;
            for (int i = 0; i < mast_size; ++i)
            {
                mast.first = packet.Read<uint32_t>();
                mast.second = packet.Read<uint8_t>();

                character->masteries.push_back(mast);
            }

            uint32_t skill_size = packet.Read<uint32_t>();

            uint32_t skillID = 0;
            for (size_t i = 0; i < skill_size; ++i)
            {
                skillID = packet.Read<uint32_t>();

                character->skills.insert(skillID);
            }

            character->Zone = packet.Read<uint16_t>();
            character->X = packet.Read<float>();
            character->Y = packet.Read<float>();
            character->Z = packet.Read<float>();
            character->Angle = packet.Read<uint16_t>();

            assert(character->Zone == 25000);

            uint16_t buff_size = packet.Read<uint16_t>();

            std::pair<uint32_t,uint32_t> buff;
            for (long i = 0; i < buff_size; ++i)
            {
                buff.first = packet.Read<uint32_t>();
                buff.second = packet.Read<uint32_t>();

                character->buffs.push_back(buff);
            }

            character->Name = packet.Read();

            character->job.Alias = packet.Read();
            character->job.Level = packet.Read<uint8_t>();
            character->job.Type = packet.Read<uint8_t>();
            character->job.Experience = packet.Read<uint64_t>();

            character->Tutorials = packet.Read<uint64_t>();
            character->GM = packet.Read<uint8_t>();

            uint8_t hotkey_size = packet.Read<uint8_t>();

            sso::Hotkey hotkey;
            for (int i = 0; i < hotkey_size; ++i)
            {
                hotkey.pos = packet.Read<uint8_t>();
                hotkey.type = packet.Read<uint8_t>();
                hotkey.id = packet.Read<uint32_t>();

                character->hotkeys.push_back(hotkey);
            }

            ParseAutopot(packet,&character->autopot);

            uint8_t block_size = packet.Read<uint8_t>();

            std::string name;
            for (int i = 0; i < block_size; ++i)
            {
                name = packet.Read();

                character->blocks.push_back(name);
            }

            if (!packet.EndOfStream())
                return MSG_ERROR_SIZE;
        }

        return MSG_SUCCESS;
    }
}

void ParseStorage (const IPacket &packet, uint8_t *capacity, std::vector<sso::property::Item> *items)
{
    if (capacity)
        *capacity = packet.Read<uint8_t>();
    else
        packet.Skip(1);

    uint8_t size = packet.Read<uint8_t>();

    for (int i = 0; i < size; ++i)
        ParseItem(packet,items);
}

void ParseItem (const IPacket &packet, std::vector<sso::property::Item> *items)
{
    sso::property::Item item;

    item.ID = packet.Read<uint64_t>();
    item.refID = packet.Read<uint32_t>();
    item.position = packet.Read<uint8_t>();
    item.enchantLv = packet.Read<uint8_t>();
    item.quantity = packet.Read<uint16_t>();
    item.mods = packet.Read<uint64_t>();
    item.arg = packet.Read<uint32_t>();

    bool hasCOS = packet.Read<uint8_t>();

    if (hasCOS)
    {
        item.cos.ID = packet.Read<uint32_t>();
        item.cos.state = packet.Read<uint8_t>();
        item.cos.name = packet.Read();
        item.cos.HP = packet.Read<uint32_t>();
        item.cos.HGP = packet.Read<uint16_t>();
        item.cos.XP = packet.Read<uint64_t>();
        item.cos.Minutes = packet.Read<uint32_t>();
        item.cos.settings = packet.Read<uint32_t>();
    }

    uint8_t attr_size = packet.Read<uint8_t>();

    for (int i = 0; i < attr_size; ++i)
        item.magicoptions.push_back(std::make_pair(packet.Read<uint32_t>(),packet.Read<uint32_t>()));

    items->push_back(item);
}

void ParseAutopot (const IPacket &packet, sso::Autopot *setting)
{
    uint8_t hp_bar_slot = packet.Read<uint8_t>();
    uint8_t hp_active_value = packet.Read<uint8_t>();

    uint8_t mp_bar_slot = packet.Read<uint8_t>();
    uint8_t mp_active_value = packet.Read<uint8_t>();

    uint8_t pill_bar_slot = packet.Read<uint8_t>();
    uint8_t pill_active = packet.Read<uint8_t>();

    uint8_t delay_active_value = packet.Read<uint8_t>();

    setting->hp_bar = (hp_bar_slot & 0xF0) >> 4;
    setting->hp_slot = (hp_bar_slot & 0x0F);
    setting->hp_active = (hp_active_value & 0x80) >> 7;
    setting->hp_value = (hp_active_value) & 0x7F;
    setting->mp_bar = (mp_bar_slot & 0xF0) >> 4;
    setting->mp_slot = (mp_bar_slot & 0x0F);
    setting->mp_active = (mp_active_value & 0x80) >> 7;
    setting->mp_value = (mp_active_value) & 0x7F;
    setting->pill_bar = (pill_bar_slot & 0xF0) >> 4;
    setting->pill_slot = (pill_bar_slot & 0x0F);
    setting->pill_active = pill_active >>= 7;
    setting->delay_active = (delay_active_value & 0x80) >> 7;
    setting->delay_value = (delay_active_value) & 0x7F;
}
