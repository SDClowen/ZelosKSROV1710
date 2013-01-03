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

#ifndef __ZELOS_GAMED_AREA_H__
#define __ZELOS_GAMED_AREA_H__

#include <set>
#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

#include "player.h"

struct Zone
{
    std::set<uint32_t> entities;
};

struct WorldArea
{
    typedef boost::unordered_map<uint32_t,Zone>::iterator grid_iterator;
    typedef boost::unordered_map<uint32_t,Zone>::const_iterator grid_const_iterator;

    int ID;					/// Assigned reference ID
    int Instanced;			/// Instanced area flag
    std::set<int> Zones;	/// Attached zones

    boost::mutex mutex;
    PlayerIndex playerIndex;
    boost::unordered_map<uint32_t,Zone> ZoneGrid;	/// Partitioned map
};

#endif // __ZELOS_GAMED_AREA_H__
