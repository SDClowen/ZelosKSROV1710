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

#ifndef DEF_PKT_COS_H
#define DEF_PKT_COS_H

namespace pkt
{
    enum COS_ACTION
    {
        COS_ACTION_MOVE = 0x01,
        COS_ACTION_ATTACK = 0x02,
        COS_ACTION_ROTATE = 0x04,
        COS_ACTION_FOLLOW = 0x09,
        COS_ACTION_EMOTE = 0x0B
    };

    enum COS_ACTION_MOVEMENT
    {
        COS_ACTION_MOVE_DIRECTION,
        COS_ACTION_MOVE_POSITION
    };

    enum COS_NOTIFY
    {
        COS_NOTIFY_REMOVE = 1,
        COS_NOTIFY_XP = 3,
        COS_NOTIFY_HGP = 4,
        COS_NOTIFY_NAME = 5,
        COS_NOTIFY_MORPH = 7
    };
}

#endif // DEF_PKT_COS_H
