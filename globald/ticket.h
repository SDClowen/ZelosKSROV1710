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

#ifndef __ZELOS_GLOBALD_TICKET_H__
#define __ZELOS_GLOBALD_TICKET_H__

#include <stdint.h>
#include <string>

#include <boost/shared_ptr.hpp>

class IConnection;

struct Ticket
{
    uint32_t ID;
    uint32_t shardID;
    std::string usr;
    std::string pw;
    std::string IP;

    boost::shared_ptr<IConnection> conn;
};

#endif // __ZELOS_GLOBALD_TICKET_H__
