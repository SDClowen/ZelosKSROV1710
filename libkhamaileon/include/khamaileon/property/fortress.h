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

#ifndef __ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_FORTRESS_H__
#define __ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_FORTRESS_H__

#include <stdint.h>
#include <string>

struct FortressProperty
{
    uint32_t refID;
    std::string refName;
    std::string refResName;
    std::string refBuilding;
    bool TypeFlag;              /// INDICATES IF THE FORTRESS IS BIG OR SMALL
    uint16_t Capacity;
    uint32_t Feed;
    std::string Icon;
    std::string refNPC;
};

#endif //__ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_ FORTRESS_H__
