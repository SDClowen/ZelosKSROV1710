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

#include "parser/refcharacter_task.h"

void ParserRefcharacter_task (const std::vector<std::string> &entries,
                              boost::function<void (const CharacterTask&)> &func)
{
    size_t idx = 0;
    CharacterTask task;

    task.refID = atol(entries[idx++].c_str());
    task.refName = entries[idx++];
    task.taskID = atoi(entries[idx++].c_str());
    task.minLv = atoi(entries[idx++].c_str());

    func(task);
}
