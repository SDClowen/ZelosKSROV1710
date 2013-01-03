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

#ifndef __ZELOS_SHARDD_AGENT_H__
#define __ZELOS_SHARDD_AGENT_H__

#include <set>
#include <string>

#include <boost/shared_ptr.hpp>

class IConnection;

struct Agent
{
    int state;
    std::string host;
    std::string port;
    boost::shared_ptr<IConnection> conn;

    std::set<uint32_t> accounts;
};

#endif // __ZELOS_SHARDD_AGENT_H__
