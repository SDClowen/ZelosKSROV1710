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

#ifndef __ZELOS_NAMED_SERVICE_H__
#define __ZELOS_NAMED_SERVICE_H__

#include <net/service.h>

#include "name_service.h"

class IConnection;
class Server;

namespace clt
{
    class Connection;
}

class Service : public IService
{
public:

    typedef std::vector<NameService>::iterator service_iterator;
    typedef std::vector<NameService>::const_iterator service_const_iterator;

    typedef std::pair<uint8_t,boost::shared_ptr<IConnection> > Query;

public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    void runServer ();

    /**
     *
     *	@brief Connect to global manager.
     *
     **/

    void runGlobal ();

    void addService (const NameService &service, const boost::shared_ptr<IConnection> &conn);

    void queryService (const uint8_t type, const boost::shared_ptr<IConnection> &conn);

    void removeService (const uint32_t connID);

    void removeConnection (const uint32_t connID);

private:

    bool initializeCert ();

    bool initializeGlobal ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

private:

    void OnGlobal ();

    void OnServer ();

    void OnService (const NameService &service, const boost::shared_ptr<IConnection> &conn);

    void OnQueryService (const uint8_t type, const boost::shared_ptr<IConnection> &conn);

    void OnCheckQuery (const NameService &service);

    void OnRemoveQuery (const uint32_t connID);

    void OnRemoveService (const uint32_t connID);

    void OnReconnectCert (const boost::system::error_code &error);

    void OnReconnectGlobal (const boost::system::error_code &error);

private:

    std::string m_error;

    bool m_srv_state;
    boost::shared_ptr<Server> m_server;

    std::string m_cert_host;
    std::string m_cert_port;
    boost::asio::deadline_timer m_cert_timer;
    boost::shared_ptr<clt::Connection> m_cert_client;

    bool m_global_state;
    std::string m_global_host;
    std::string m_global_port;
    boost::asio::deadline_timer m_global_timer;
    boost::shared_ptr<clt::Connection> m_global_client;

    boost::asio::io_service::strand m_service_strand;
    std::vector<NameService> m_services;

    boost::mutex m_query_mutex;
    std::vector<Query> m_queries; /// Pending services queries
};

#endif // __ZELOS_NAMED_SERVICE_H__
