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

#ifndef __ZELOS_SERVICES_CHARACTERD_CHARACTER_H__
#define __ZELOS_SERVICES_CHARACTERD_CHARACTER_H__

#include <set>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

#include <sso/autopot.h>
#include <sso/hotkey.h>
#include <sso/job.h>
#include <sso/property/player.h>
#include <sso/property/item.h>

#define MAX_AVATAR_SLOTS 5

namespace sso
{
    struct CharacterView
    {
        uint32_t refID;
        std::string name;
        uint8_t scale;
        uint8_t level;
        uint64_t exp;
        uint16_t statStr;
        uint16_t statInt;
        uint16_t stats;
        uint32_t HP;
        uint32_t MP;
        uint16_t timeMin;

        std::vector<std::pair<uint32_t,uint8_t> > equipItems;
        std::vector<std::pair<uint32_t,uint8_t> > avatarItems;
    };

    struct CharacterInfo : public sso::property::Player
    {
        Job job;
        Autopot autopot;

        std::vector<sso::property::Item> inventory;
        std::vector<sso::property::Item> avatar;
        std::vector<sso::property::Item> storage;

        std::vector<std::pair<uint32_t,uint8_t> > masteries;
        std::set<uint32_t> skills;
        std::vector<std::pair<uint32_t,uint32_t> > buffs;
        std::vector<std::string> blocks;
        std::vector<Hotkey> hotkeys;
    };
}

#endif // __ZELOS_SERVICES_CHARACTERD_CHARACTER_H__
