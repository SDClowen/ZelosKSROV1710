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

#ifndef DEF_PKT_STALL_H
#define DEF_PKT_STALL_H

namespace pkt
{
    enum STALL_ACTION_TYPE
    {
        STALL_ACTION_PRICE = 1,
        STALL_ACTION_ADD = 2,
        STALL_ACTION_REMOVE = 3,
        STALL_ACTION_STATE = 5,
        STALL_ACTION_GREETING = 6,
        STALL_ACTION_TITLE = 7
    };

    enum STALL_NOTIFY_TYPE
    {
        STALL_NOTIFY_LEAVE = 1,
        STALL_NOTIFY_ENTER,
        STALL_NOTIFY_BUY
    };

    enum STALL_ERROR
    {
        ERROR_STALL_INSUFFICIENT_GOLD = 0x3C11,
        ERROR_STALL_INVENTORY_FULL = 0x3C12,
        ERROR_STALL_DESPAWN = 0x3C17,
        ERROR_STALL_MAX_CLIENTS = 0x3C18,
        ERROR_STALL_OPEN_WITH_TRANSPORT = 0x3C34
    };
}

#endif // DEF_PKT_STALL_H
