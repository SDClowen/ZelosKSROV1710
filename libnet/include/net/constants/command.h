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

#ifndef DEF_PKT_COMMAND_H
#define DEF_PKT_COMMAND_H

namespace pkt
{
    enum GM_COMMAND
    {
        GM_COMMAND_LOAD_MONSTER = 6,
        GM_COMMAND_CREATE_ITEM,
        GM_COMMAND_MOVE_TO_USER,
        GM_COMMAND_BAN_USER = 13,
        GM_COMMAND_RECALL_USER = 17,
        GM_COMMAND_RECALL_GUILD,
        GM_COMMAND_KILL = 20,
        GM_COMMAND_BLOCK_LOGOUT = 23,
        GM_COMMAND_ALLOW_LOGOUT,
        GM_COMMAND_LIE_NAME,
        GM_COMMAND_SPAWN_ALL_UNIQUES = 43,
        GM_COMMAND_CLEAR_INVENTORY = 49
    };
}

#endif // DEF_PKT_COMMAND_H
