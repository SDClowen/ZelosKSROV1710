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

#ifndef DEF_PKT_LOBBY_H
#define DEF_PKT_LOBBY_H

namespace pkt
{
    enum LOBBY_ACTION
    {
        LOBBY_ACTION_CREATE,
        LOBBY_ACTION_LIST,
        LOBBY_ACTION_ERASE,
        LOBBY_ACTION_VALIDATE_NAME,
        LOBBY_ACTION_RESTORE
    };

    enum LOBBY_ERROR
    {
        LOBBY_ERROR_MAX_CHARACTERS = 0x0405,
        LOBBY_ERROR_NAME_EXIST = 0x0410
    };
}

#endif // DEF_PKT_LOBBY_H
