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

#ifndef DEF_PKT_FRIEND_H
#define DEF_PKT_FRIEND_H

namespace pkt
{
    enum FRIEND_STATE
    {
        FRIEND_ONLINE,
        FRIEND_OFFLINE
    };

    enum FRIEND_NOTIFY
    {
        FRIEND_NOTIFY_ACCEPT = 0x02,
        FRIEND_NOTIFY_DELETE,
        FRIEND_NOTIFY_STATE
    };

    enum FRIEND_ERROR
    {
        FRIEND_ERROR_ADD_LIST_FULL = 0x6408,				/// UIIT_MSG_FRIENDERR_FULLLIST
        FRIEND_ERROR_ADD_TARGET_ALREADY_EXISTS = 0x6409,	/// UIIT_MSG_FRIENDERR_ALREADYFRIEND
        FRIEND_ERROR_ADD_CANCEL = 0x640A,					/// UIIT_MSG_FRIEND_SYSCANCEL
        FRIEND_ERROR_ADD_REFUSED = 0x640B,					/// UIIT_MSG_FRIENDERR_REFUSAL
        FRIEND_ERROR_ADD_INVALID_TARGET = 0x640D,			/// UIIT_MSG_FRIENDERR_UNKNOWNUSER
        FRIEND_ERROR_ADD_PENDING = 0x640E,					/// UIIT_FRIEND_SYSPROGRESS
        FRIEND_ERROR_ADD_TARGET_LIST_FULL = 0x640F,			/// UIIT_FRIENDERR_TARGETLISTFULL

        FRIEND_ERROR_GROUP_OVER = 0x6418,					/// UIIT_MSG_FRIEND_GROUP_OVER
        FRIEND_ERROR_GROUP_ALREADY_EXISTS = 0x6419,			/// UIIT_MSG_FRIEND_GROUP_OVERLAP
        FRIEND_ERROR_GROUP_NAME_SIZE = 0x641D				/// UIIT_MSG_FRIEND_GROUP_STRINGSIZE
    };

    enum FRIEND_ANSWER
    {
        ANSWER_DENY = 0x640B,
        ANSWER_IGNORE = 0x000B
    };
}

#endif // DEF_PKT_FRIEND_H
