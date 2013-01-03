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

#ifndef __ZELOS_LIBNET_NET_SERVICE_CLIENT_H__
#define __ZELOS_LIBNET_NET_SERVICE_CLIENT_H__

#include <stdint.h>

#include <string>

#include <boost/asio.hpp>

namespace clt
{
    class Connection;
}

struct ServiceClt
{
    ServiceClt (boost::asio::io_service &io_service);

    void connect ();

    bool state;
    bool active;
    int8_t attempts;
    int8_t max_attempts;

    std::string host;
    std::string port;
    boost::asio::deadline_timer timer;
    boost::shared_ptr<clt::Connection> conn;
};

#endif // __ZELOS_LIBNET_NET_SERVICE_CLIENT_H__
