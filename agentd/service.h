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

#ifndef __ZELOS_AGENTD_SERVICE_H__
#define __ZELOS_AGENTD_SERVICE_H__

#include <boost/bimap.hpp>
#include <boost/unordered_map.hpp>

#include <net/service.h>

#include "area.h"
#include "client.h"
#include "ticket.h"

namespace clt
{
    class Connection;
}

class Server;

class Service : public IService
{
public:

    typedef std::map<uint32_t,boost::shared_ptr<IConnection> >::iterator ticket_iterator;

    typedef std::map<uint32_t,boost::shared_ptr<IConnection> >::const_iterator ticket_const_iterator;

public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    const std::string& getHost () const;

    const std::string& getPort () const;

    void setLocale (const uint8_t locale);

    uint8_t getLocale () const;

    void setVersion (const uint32_t version);

    void setMaxAreas (const uint8_t size);

    void setRace (const uint8_t race);

    void setFilter (const std::string &chFilter, const std::vector<std::string> &strFilter);

    void addArea (const Area &area);

    void activateGShard (const uint32_t connID);

    void addTicket (const Ticket &ticket);

    void processTicket (const uint32_t ticketID, const uint32_t accountID, const uint8_t secLv);

    void processTicket (const uint32_t ticketID, const uint16_t error_code);

    boost::shared_ptr<IConnection> getCharClient () const;

    /**
     *
     *	@brief Start listening to client connections.
     *
     **/

    void runServer ();

    /**
     *
     *	@brief Close listening to client connection.
     *
     **/

    void stopServer ();

    /**
     *
     *	@brief Connect to a new service component required by the service.
     *	@param type: Component type.
     *	@param host: Component host address.
     *	@param port: Component port.
     *
     **/

    void addComponent (const uint8_t type, const std::string &host, const std::string &port);

    /**
     *
     *	@brief Remove a component because the connection has been terminated and
     *	query name service to restablish the connection.
     *
     **/

    void reconnectComponent (const uint8_t type);

    void activateComponent (const uint8_t type);

    /**
     *
     *	@brief Get the account ID associated with a connection.
     *	@param connID: Connection ID to search.
     *	@retval Account ID associated with the connection, if the connection
     *	dont exists or dont have an associated account returns 0.
     *
     **/

    uint32_t getAccountID (const uint32_t connID) const;

    void setAccountArea (const uint32_t accountID, const uint16_t zoneID);

    /**
     *
     *	@brief Get the connection ID associated with an account.
     *	@param accountID: account ID to search.
     *	@retval connection ID associated with the account, if the account
     *	dont exists or dont have an associated account returns 0.
     *
     **/

    uint32_t getConnectionID (const uint32_t accountID) const;

    boost::shared_ptr<IConnection> findConnection (const uint32_t accountID) const;

    void removeConnection (const uint32_t connID);

    void killConnection (const uint32_t accountID);

private:

    /**
     *
     *	@brief Initialize shard service client.
     *	@retval True if the operation was successful, false otherwise.
     *
     */

    bool initializeShard ();

    /**
     *
     *	@brief Initialize name service client.
     *	@retval True if the operation was successful, false otherwise.
     *
     */

    bool initializeName ();

    /**
     *
     *	@brief Initialize character service client.
     *	@retval True if the operation was successful, false otherwise.
     *
     **/

    bool initializeChar ();

    bool initializeServer ();

    boost::shared_ptr<IConnection> createGShardConnection (const uint32_t areaID);

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

private:

    void OnCheckStatus ();

    void OnStatus (bool status);

    void OnShardState (const bool state);

    void OnShardReconnect (const boost::system::error_code &error);

    void OnNameReconnect (const boost::system::error_code &error);

    void OnNameState (const bool state);

    void OnServerRun ();

    void OnServerStop ();

    void OnServerClosed ();

    void OnArea (Area area);

    void OnGShardReconnect (const uint32_t areaID, const boost::system::error_code &error);

    void OnGShardState (const uint32_t connID, const bool state);

    void OnGShardError (const uint32_t connID);

    void OnGShardActivate (const uint32_t connID);

    void OnGShardMapAccount (const uint32_t accountID, const uint16_t zoneID);

    void OnGShardClose ();

    void OnTicket (const Ticket &ticket);

    void OnLoginSuccess (const uint32_t ticketID, const uint32_t accountID, const uint8_t secLv);

    void OnLoginError (const uint32_t ticketID, const int error_code);

    void OnComponent (const uint8_t type, const std::string &host, const std::string &port);

    void OnComponentState (const uint8_t type, const bool state);

    void OnComponentError (const uint8_t type);

    void OnComponentQuery (const uint8_t type);

    void OnComponentReconnect (const uint8_t type, const boost::system::error_code &error);

    void OnRemoveConnection (const uint32_t connID);

    void OnKillConnection (const uint32_t accountID);

private:

    std::string m_error;
    std::string m_host;
    std::string m_port;

    bool m_state;
    uint8_t m_locale;
    uint8_t m_race;
    uint8_t m_maxAreas;
    uint32_t m_version;
    std::string m_name;
    std::string m_chFilter;
    std::vector<std::string> m_strFilter;

    bool m_shard_state;
    std::string m_shard_host;
    std::string m_shard_port;
    boost::asio::deadline_timer m_shard_timer;
    boost::shared_ptr<clt::Connection> m_shard_client;

    bool m_name_state;
    std::string m_name_host;
    std::string m_name_port;
    boost::asio::deadline_timer m_name_timer;
    boost::shared_ptr<clt::Connection> m_name_client;

    boost::shared_ptr<Server> m_server;

    bool m_chard_state;
    bool m_chard_active;
    int m_chard_attempts;
    std::string m_chard_host;
    std::string m_chard_port;
    boost::asio::deadline_timer m_chard_timer;
    boost::shared_ptr<clt::Connection> m_chard_client;

    boost::asio::io_service::strand m_area_strand;
    std::vector<Area> m_areas;

    boost::mutex m_ticket_mutex;
    std::map<uint32_t,boost::shared_ptr<IConnection> > m_tickets;

    mutable boost::mutex m_client_mutex;
    client_set m_clients;

    std::map<std::string, std::string> m_config;
};

#endif // __ZELOS_AGENTD_SERVICE_H__
