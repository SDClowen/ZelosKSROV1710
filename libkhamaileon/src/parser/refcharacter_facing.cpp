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

#include "parser/refcharacter_facing.h"

void ParserRefcharacter_facing (const std::vector<std::string> &entries,
                                boost::function<void (const std::pair<uint32_t,uint16_t>&)> &func)
{
    size_t idx = 0;
    std::pair<uint32_t,uint16_t> entry;

    entry.first = atol(entries[idx++].c_str());
    entry.second = atoi(entries[idx++].c_str());

    func(entry);
}
