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

#include "parser/refcharacter_drop_quality.h"

void ParserRefcharacter_drop_quality (const std::vector<std::string> &entries,
                                      boost::function<void (const DropWeight&)> &func)
{
    size_t idx = 0;
    DropWeight drop;

    drop.refCharID = atol(entries[idx++].c_str());
    drop.Weight = strtod(entries[idx++].c_str(),NULL);
    drop.Quality = atoi(entries[idx++].c_str());

    func(drop);
}
