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

#ifndef __ZELOS_LIBNET_NET_CLIENT_CONNECTION_H__
#define __ZELOS_LIBNET_NET_CLIENT_CONNECTION_H__

#include <net/connection.h>

class IService;

namespace clt
{

class Connection : public IConnection
{
public:

    Connection (const int type, IService *service, boost::asio::io_service &io_service);

    virtual ~Connection ();

    void KeepAlive ();

private:

    void OnListen ();

    void OnCleanup ();

    void OnError (const boost::system::error_code &error);

    void OnKeepAlive (const boost::system::error_code &error);

private:

    int m_type;
    IService *m_service;
    boost::asio::deadline_timer m_ping_timer;
};

}

#endif // __ZELOS_LIBNET_NET_CLIENT_CONNECTION_H__
