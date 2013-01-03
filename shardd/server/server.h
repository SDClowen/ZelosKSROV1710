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

#ifndef __ZELOS_SHARDD_SERVER_SERVER_H__
#define __ZELOS_SHARDD_SERVER_SERVER_H__

#include <net/server.h>

class Service;

class Server : public IServer, public boost::enable_shared_from_this<Server>
{
public:

    Server (Service *service, boost::asio::io_service &io_service);

    virtual ~Server ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnRelease ();

    void OnRemoveConnection (const uint32_t connID, const uint8_t state);

    boost::shared_ptr<IConnection> CreateConnection ();

private:

    Service *m_service;
};

#endif // __ZELOS_SHARDD_SERVER_SERVER_H__
