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

#include "parser/refcharacter_drop.h"

void ParserRefcharacter_drop (const std::vector<std::string> &entries,
                                      boost::function<void (const DropProperty&)> &func)
{
    size_t idx = 0;
    DropProperty property;

    property.refCharID = atol(entries[idx++].c_str());
    property.refItemID = atol(entries[idx++].c_str());
    property.Weight = strtod(entries[idx++].c_str(),NULL);
    property.Quality = atoi(entries[idx++].c_str());

    func(property);
}
