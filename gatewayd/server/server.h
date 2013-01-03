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

#ifndef __ZELOS_GATEWAYD_SERVER_H__
#define __ZELOS_GATEWAYD_SERVER_H__

#include <net/server.h>

class Service;

class Server : public IServer, public boost::enable_shared_from_this<Server>
{
public:

    Server (Service *service, boost::asio::io_service &io_service);

    virtual ~Server ();

    void SetLocale (const uint8_t locale);

    void SetClientVersion (const uint32_t version);

    void SetClientID (const std::string &ID);

    void SetGlobalVersion (const uint8_t version);

    void SetServerListID (const std::string &ID);

    const std::string& ClientID () const;

    const std::string& ServerListID () const;

protected:

    virtual bool OnInitialize ();

    virtual void OnConfigure (const std::map<std::string,std::string> &entries);

    virtual void OnRemoveConnection (const uint32_t ID, const uint8_t state);

    virtual boost::shared_ptr<IConnection> CreateConnection ();

    virtual void OnRelease ();

private:

    std::string m_client_id;
    std::string m_server_list_id;

    Service *m_service;
};

#endif //__ZELOS_GATEWAYD_SERVER_H__
