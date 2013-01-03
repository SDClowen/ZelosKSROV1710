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

#ifndef __ZELOS_LIBSSO_SSO_JOB_H__
#define __ZELOS_LIBSSO_SSO_JOB_H__

#include <map>
#include <stdint.h>
#include <string>
#include <sys/types.h>

#define JOB_LEAGUE_ENTRY_FEED 10000
#define JOB_CAPE_WAIT_TIME 10

#define THIEF_DEN_TELEPORT 8

namespace sso
{
    enum JOB_TYPE
    {
        JOB_NONE,
        JOB_TRADER,
        JOB_THIEF,
        JOB_HUNTER
    };

    struct Job
    {
        uint8_t Level;
        uint8_t Type;
        uint64_t Experience;
        uint32_t Contribution;
        std::string Alias;
    };
}

#endif // __ZELOS_LIBSSO_SSO_JOB_H__
