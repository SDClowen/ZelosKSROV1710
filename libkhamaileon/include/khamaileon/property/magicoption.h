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

#ifndef __ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_MAGICOPTION_H__
#define __ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_MAGICOPTION_H__

#include <string>
#include <vector>
#include <stdint.h>

#define MAGIC_OPT_MAX_REQ_COUNT 4

struct MagicOptionValue
{
    uint16_t Val;
    uint8_t Probability;
};

struct MagicOptionProperty
{
    uint32_t ID;
    std::string Name;
    bool DecValue;
    uint8_t Degree;
    std::string Requirement[MAGIC_OPT_MAX_REQ_COUNT];

    std::vector<MagicOptionValue> Values;
};


#endif // __ZELOS_LIBKHAMAILEON_KHAMAILEON_PROPERTY_MAGICOPTION_PROPERTY_H__
