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

#ifndef __ZELOS_LIBSERVER_NET_SERVER_CONNECTION_H__
#define __ZELOS_LIBSERVER_NET_SERVER_CONNECTION_H__

#include "net/connection.h"

class IServer;

namespace srv
{

class Connection : public IConnection
{
public:

    Connection (const uint32_t ID, const uint32_t timeout, boost::asio::io_service &io_service,
                const boost::weak_ptr<IServer> &srv);

    virtual ~Connection ();

protected:

    void OnListen ();

    void OnCleanup ();

    void OnError (const boost::system::error_code &error);

private:

    boost::weak_ptr<IServer> m_server;
};

}

#endif // __ZELOS_LIBSERVER_NET_SERVER_CONNECTION_H__
