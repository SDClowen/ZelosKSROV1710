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

#ifndef __ZELOS_LIBSSO_SSO_PROPERTY_PLAYER_H__
#define __ZELOS_LIBSSO_SSO_PROPERTY_PLAYER_H__

#include <stdint.h>
#include <string>

#define MAX_PLAYER_BERSERK_COUNT 5
#define MAX_PLAYER_BERSERK_TIME 60
#define MAX_PLAYER_NAME_LENTH 12

#define PLAYER_INVUNERABLE_STATE_TIME 5

#define PLAYER_MAX_RES_POS_LEVEL 10
#define PLAYER_MIN_JOB_LEVEL 20

#define STORAGE_AVA_SIZE 5
#define STORAGE_INV_SIZE 45
#define STORAGE_INV_START_POS 13

#define PLAYER_MIN_LV_DISPLAY 0x01
#define PLAYER_HELP_DISPLAY 0x02

#define PLAYER_EXP_PERCENT_LOSS 2

namespace sso
{
    namespace property
    {
        /**
         *
         *  BASE PLAYER PROPERTIES THIS ONES ARE SAVED IN THE DB
         *
         **/

        struct Player
        {
            Player ();

            uint64_t ID;            ///< Character UniqueID (only used for DB functions).
            uint32_t RefCharID;     ///< ID of the refcharacterdata used.
            std::string Name;       ///< Character name.
            uint8_t Volume;         ///< Character volumes.
            uint8_t Level;          ///< Character current level.
            uint8_t MaxLevel;       ///< Character max level achieved.
            uint16_t Str;           ///< Character base strength.
            uint16_t Int;           ///< Character base intelligence.
            uint32_t HP;            ///< Character current hit points.
            uint32_t MP;            ///< Character current mana points.
            uint32_t Stats;
            uint32_t SP;
            uint64_t Exp;
            uint32_t SPExp;
            uint64_t Gold;
            uint64_t GoldStored;
            uint8_t BskCount;
            uint8_t PKDaily;
            uint16_t PKLevel;
            uint32_t PKPoints;
            int32_t Zone;
            float X;
            float Y;
            float Z;
            uint16_t Angle;
            uint8_t Rank;
            uint8_t pvpCape;
            bool GM;
            uint64_t Tutorials;		///< Tutorial displayed list. (BIT FIELDS)
            uint8_t InvSlots;
            bool InvSlotsIncreased;
            uint8_t StoreSlots;
            bool StorageSlotsIncreased;
            uint32_t GuildID;
            uint32_t AcademyID;
            uint32_t ReturnID;
            uint32_t StallAvatarID;
            uint32_t AbilityPetID;
            uint32_t AttackPetID;
            uint32_t TransportID;
        };
    }
}

#endif // __ZELOS_LIBSSO_SSO_PROPERTY_PLAYER_H__
