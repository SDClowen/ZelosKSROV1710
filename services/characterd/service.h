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

#ifndef __ZELOS_SERVICES_CHARACTERD_SERVICE_H__
#define __ZELOS_SERVICES_CHARACTERD_SERVICE_H__

#include <net/service.h>

namespace clt
{
    class Connection;
}

class Server;
class Manager;

class Service : public IService
{
public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    const std::string& getHost () const;

    const std::string& getPort () const;

    boost::shared_ptr<Manager> getCharService () const;

private:

    bool initializeName ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

    void OnReconnect (const boost::system::error_code &error);

private:

    std::string m_error;

    std::string m_host;
    std::string m_port;

    bool m_name_state;
    std::string m_name_host;
    std::string m_name_port;
    boost::asio::deadline_timer m_name_timer;
    boost::shared_ptr<clt::Connection> m_name_client;

    boost::shared_ptr<Manager> m_char_service;

    boost::shared_ptr<Server> m_server;
};

#endif // __ZELOS_SERVICES_CHARACTERD_SERVICE_H__
