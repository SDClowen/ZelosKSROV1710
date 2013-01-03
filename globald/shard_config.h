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

#ifndef __ZELOS_GLOBALD_SHARD_CONFIG_H__
#define __ZELOS_GLOBALD_SHARD_CONFIG_H__

#include <string>
#include <utility>
#include <vector>

#include "area.h"

struct ShardConfig
{
    int ID;
    int locale;
    int version;
    int races;
    std::pair<std::string,std::vector<std::string> > filter;
    std::vector<Area> world;
};

#endif // __ZELOS_GLOBALD_SHARD_CONFIG_H__
