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

#ifndef __ZELOS_GLOBALD_SHARD_HPP__
#define __ZELOS_GLOBALD_SHARD_HPP__

#include <stdint.h>
#include <string>
#include <vector>

enum SHARD_NOTIFY
{
    SHARD_NOTIFY_USR_CNT,
    SHARD_NOTIFY_USR_MAX,
    SHARD_NOTIFY_STATE
};

struct Agent
{
    Agent ()
        : ID(0), port(0)
    {}

    uint32_t ID;
    std::string host;		/// Server IP to connect.
    uint16_t port;		/// Server
};

struct Shard
{
    Shard ()
        : connID(0)
    {}

    bool is_new;		/// Flag that indicates if its a new server.
    std::string name;	/// Server name
    uint16_t id;		/// Server Identification number.
    uint16_t usr_cnt;	/// Current users count.
    uint16_t usr_max;	/// Max number of users spaces available.
    uint8_t state;		/// Server state (01 - Online, 00 - Check).
    uint32_t connID;

    std::vector<Agent> agents;
};

#endif //__ZELOS_GLOBALD_SHARD_HPP__
