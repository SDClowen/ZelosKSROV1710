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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_NPC_TASK_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_NPC_TASK_H__

#include <stdint.h>

namespace sso
{
    enum NPC_TASK_TYPE
    {
        NPC_TASK_BUY_SELL = 1,
        NPC_TASK_TALK_TO_NPC = 2,
        NPC_TASK_STORAGE = 3,
        NPC_TASK_BLACKSMITH = 4,
        NPC_TASK_DESIGNATE_RETURN_POINT = 7,
        NPC_TASK_TELEPORT_TO_AREA = 8,
        NPC_TASK_TELEPORT_TO_RETURN = 9,
        NPC_TASK_SKILL_EDIT = 10,
        NPC_TASK_STABLE = 11,
        NPC_TASK_BUY_SELL_SPECIAL_GOODS = 12,
        NPC_TASK_INFORMATION_SPECIAL_GOODS = 13,
        NPC_TASK_HUNTER = 14,
        NPC_TASK_GUILD_MANAGEMENT = 15,
        NPC_TASK_GACHA = 17,
        NPC_TASK_GACHA_REWARD = 18,
        NPC_TASK_EVENT_REWARD = 19,
        NPC_TASK_TRADER = 20,
        NPC_TASK_THIEF = 21,
        NPC_TASK_HUNTER_2 = 22,
        NPC_TASK_FORTRESS_ADMINISTRATOR = 23,
        NPC_TASK_FORTRESS_CLERK = 24,
        NPC_TASK_FORTRESS_COMBAT_ASSISTENT = 25,
        NPC_TASK_FOTRESS_BLACKSMITH = 26,
        NPC_TASK_REVERSE_RETURN = 30,
        NPC_TASK_GATE_PULLEY = 31,
        NPC_TASK_AVATAR_GRANT_MAGICOPTION = 32,
        NPC_TASK_BATTLE_ARENA = 33,
        NPC_TASK_BATTLE_ARENA_REWARD = 34,
        NPC_TASK_AUCTION_HOUSE = 35
    };

    struct NPCTask
    {
        uint8_t ID;
        uint8_t MinLv;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_NPC_TASK_H__
