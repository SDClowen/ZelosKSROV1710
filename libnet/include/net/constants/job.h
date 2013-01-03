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

#ifndef DEF_PKT_JOB_H
#define DEF_PKT_JOB_H

namespace pkt
{
    enum JOB_ERROR
    {
        JOB_ERROR_ALIAS_TAKEN = 0x4802,
        JOB_ERROR_INSUFFICIENT_GOLD = 0x4807,
        JOB_ERROR_INSUFFICIENT_LEVEL = 0x4819,
        JOB_ERROR_ALREDY_JOINED_A_LEAGUE = 0x480A,
        JOB_ERROR_NEED_JOB_LEAGUE = 0x480B,
        JOB_ERROR_JOB_CAPE_UNEQUIPPED = 0x480C,
        JOB_ERROR_INVALID_NAME = 0x480D,
        JOB_ERROR_JOIN_PENALTY = 0x480E,
        JOB_ERROR_ALIAS_EXISTS = 0x480F,
        JOB_ERROR_NO_PAST_JOB_INFORMATION = 0x4829,
        JOB_ERROR_MAX_SAFE_TRADES = 0x4841
    };

    enum JOB_MSG_TYPE
    {
        JOB_ALIAS_VALID,
        JOB_ALIAS_CREATE,
        JOB_ALIAS_EXISTS = 0x25
    };
}

#endif // DEF_PKT_JOB_H
