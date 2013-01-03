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

#include "property/player.h"

namespace sso
{
    namespace property
    {
        Player::Player ()
        {
            ID = 0;
            RefCharID = 0;
            Volume = 0;
            Level = 1;
            MaxLevel = 1;
            Str = 20;
            Int = 20;
            HP = 200;
            MP = 200;
            Stats = 0;
            SP = 0;
            Exp = 0;
            SPExp = 0;
            Gold = 0;
            GoldStored = 0;
            BskCount = 0;
            PKDaily = 0;
            PKLevel = 0;
            PKPoints = 0;
            Zone = 0;
            X = 0;
            Y = 0;
            Z = 0;
            Angle = 0;
            Rank = 0;
            pvpCape = 0;
            GM = false;
            Tutorials = 1;
            InvSlots = 45;
            StoreSlots = 150;
            InvSlotsIncreased = false;
            StorageSlotsIncreased = false;
            GuildID = 0;
            AcademyID = 0;
            ReturnID = 0;
            StallAvatarID = 0;
            AbilityPetID = 0;
            AttackPetID = 0;
            TransportID = 0;
        }
    }
}
