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

#ifndef DEF_PKT_PARTY_H
#define DEF_PKT_PARTY_H

namespace pkt
{
    enum PARTY_NOTIFY
    {
        PARTY_NOTIFY_DISMISS = 1,
        PARTY_NOTIFY_MEMBER_JOIN,
        PARTY_NOTIFY_MEMBER_STATE,
        PARTY_NOTIFY_MEMBER_INFO = 6,
        PARTY_NOTIFY_LEADER = 9
    };

    enum PARTY_NOTIFY_MEMBER
    {
        PARTY_NOTIFY_MEMBER_LEVEL = 0x02,
        PARTY_NOTIFY_MEMBER_HEALTH = 0x04,
        PARTY_NOTIFY_MEMBER_MASTERY = 0x08,
        PARTY_NOTIFY_MEMBER_POSITION = 0x20,
        PARTY_NOTIFY_MEMBER_GUILD = 0x40
    };

    enum PARTY_ANSWER_TYPE
    {
        PARTY_ANSWER_DENY,
        PARTY_ANSWER_ACCEPT,
        PARTY_ANSWER_IGNORE
    };

    enum PARTY_ERROR
    {
        PARTY_ERROR_PLAYER_BUSY = 0x2C06,
        PARTY_ERROR_MEMBER_FULL = 0x2C08,
        PARTY_ERROR_INSUFFICIENT_LEVEL = 0x2C0A,
        PARTY_ERROR_INVALID_APPLICANT = 0x2C0E,
        PARTY_ERROR_INVALID_TARGET = 0x2C0F,
        PARTY_ERROR_REJECTED = 0x2C10,
        PARTY_ERROR_LEADER_DISMISSED = 0x2C11,
        PARTY_ERROR_REQUEST_DENY = 0x2C17,
        PARTY_ERROR_TARGET_IN_PARTY = 0x2C18,
        PARTY_ERROR_INVALID_PARTY = 0x2C1C,
        PARTY_ERROR_INVALID_LEVEL = 0x2C1E,
        PARTY_ERROR_PENDING_INVITATION = 0x2C1F,
        PARTY_ERROR_OPPOSING_ROLES = 0x2C23
    };
}

#endif // DEF_PKT_PARTY_H
