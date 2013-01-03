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

#ifndef DEF_PKT_GUILD_H
#define DEF_PKT_GUILD_H

namespace pkt
{
    enum GUILD_NOTIFY_TYPE
    {
        GUILD_NOTIFY_DISBAND = 0x01,
        GUILD_NOTIFY_MEMBER_JOIN = 0x02,
        GUILD_NOTIFY_MEMBER_LEAVE = 0x03,
        GUILD_NOTIFY_GUILD = 0x05,
        GUILD_NOTIFY_MEMBER = 0x06,
        GUILD_NOTIFY_UNION_MEMBER = 0x0E,
        GUILD_NOTIFY_UNION_MEMBER_JOIN = 0x0D,
        GUILD_NOTIFY_UNION_MEMBER_LEAVE = 0x12,
        GUILD_NOTIFY_MEMBER_AUTHORITY = 0x16,
        GUILD_NOTIFY_WAR_END_TIME_A = 0x1A,
        GUILD_NOTIFY_WAR_END_TIME_B = 0x1B,
        GUILD_NOTIFY_WAR_OUTCOME = 0x1C,
        GUILD_NOTIFY_WAR_CASUALTY = 0x1D,
        GUILD_NOTIFY_WAR_REMAINING_TIME = 0x1F
    };

    enum GUILD_NOTIFY_GUILD_TYPE
    {
        GUILD_NOTIFY_GUILD_GP = 0x08,
        GUILD_NOTIFY_GUILD_PROMOTE = 0x0C,
        GUILD_NOTIFY_GUILD_NOTICE = 0x10
    };

    enum GUILD_NOTIFY_MEMBER_TYPE
    {
        GUILD_NOTIFY_MEMBER_LEVEL = 0x01,
        GUILD_NOTIFY_MEMBER_STATE = 0x02,
        GUILD_NOTIFY_MEMBER_GP = 0x08,
        GUILD_NOTIFY_MEMBER_POSITION = 0x40,
        GUILD_NOTIFY_MEMBER_ZONE = 0x80
    };

    enum GUILD_NOTIFY_UNION_TYPE
    {
        GUILD_NOTIFY_UNION_JOIN = 0x01,
        GUILD_NOTIFY_UNION_MEMBER_LEVEL = 0x02,
        GUILD_NOTIFY_UNION_MEMBER_LEADER = 0x04,
        GUILD_NOTIFY_UNION_MEMBER_COUNT = 0x08
    };

    enum GUILD_NOTIFY_UNION_MEMBER_LEAVE_TYPE
    {
        GUILD_NOTIFY_UNION_MEMBER_REASON_LEAVE = 1,
        GUILD_NOTIFY_UNION_MEMBER_REASON_KICK,
        GUILD_NOTIFY_UNION_MEMBER_REASON_DISBAND
    };

    enum GUILD_NOTIFY_MEMBER_AUTHORITY_TYPE
    {
        GUILD_NOTIFY_MEMBER_AUTHORITY_SETTING = 0x10,
        GUILD_NOTIFY_MEMBER_AUTHORITY_LEADER_TRANSFER = 0x54
    };

    enum GUILD_MEMBER_STATE
    {
        GUILD_MEMBER_LOGIN,
        GUILD_MEMBER_LOGOFF
    };

    enum GUILD_WAR_CASUALTY_TYPE
    {
        GUILD_WAR_CASUALTY_DEATH = 0x01,
        GUILD_WAR_CASUALTY_KILL = 0x02
    };

    enum GUILD_INVITE_ANSWER
    {
        INVITE_GUILD_REJECT = 0x1602,
        INVITE_GUILD_ACCEPT = 0x0101
    };

    enum GUILD_ERROR_TYPE
    {
        ERROR_GUILD_PLAYER_IS_BUSY = 0x4C06,
        ERROR_GUILD_MANAGE_GUILD_WHILE_DEAD = 0x4C07,
        ERROR_GUILD_CREATE_INSUFFICIENT_LEVEL = 0x4C0A,
        ERROR_GUILD_CREATE_INSUFFICIENT_GOLD = 0x4C0C,
        ERROR_GUILD_INVITE_SAME_GUILD = 0x4C12,
        ERROR_GUILD_INVITE_MEMBER_LIMIT = 0x4C13,
        ERROR_GUILD_CREATE_NAME_INVALID_LENGTH = 0x4C18,
        ERROR_GUILD_CREATE_NAME_INVALID = 0x4C19,
        ERROR_GUILD_CREATE_NAME_EXISTS = 0x4C1A,
        ERROR_GUILD_MAX_LEVEL = 0x4C21,
        ERROR_GUILD_NOTICE_TITLE_EMPTY = 0x4C22,
        ERROR_GUILD_NOTICE_MSG_EMPTY = 0x4C23,
        ERROR_GUILD_INSUFFICIENT_PERMISSION = 0x4C24,
        ERROR_GUILD_INVALID_NAME = 0x4C25,
        ERROR_GUILD_INSUFFICIENT_GOLD = 0x4C31,
        ERROR_GUILD_INSUFFICIENT_GP = 0x4C32,
        ERROR_GUILD_CREATE_PENALTY = 0x4C3C,
        ERROR_GUILD_ALLY = 0x4C41,
        ERROR_GUILD_STORAGE_IN_USE = 0x4C48,
        ERROR_GUILD_GRANT_INVALID = 0x4C4D,
        ERROR_GUILD_NOT_AUTHORIZED = 0x4C52, /// ALSO 0x1E
        ERROR_GUILD_CREATE_WHILE_JOBBING = 0x4C57,
        ERROR_GUILD_OBJECT_FAR = 0x4C60
    };
}

#endif // DEF_PKT_GUILD_H
