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

#ifndef __ZELOS_GAMED_SERVICE_H__
#define __ZELOS_GAMED_SERVICE_H__

#include <net/service.h>
#include <net/service_client.h>

#include "area.h"
#include "client.h"
#include "peer.h"

namespace clt
{
    class Connection;
}

namespace sso
{
    class CharacterInfo;
}

class Server;
class IConnection;
class OPacket;
class World;

class Service : public IService
{
public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    const std::string& getHost () const;

    const std::string& getPort () const;

    void runServer ();

    void addAgent (const boost::shared_ptr<IConnection> &conn);

    void removeAgent (const uint32_t connID);

    void addComponent (const uint8_t type, const std::string &host, const std::string &port);

    void loadCharacter (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

    void runSimulation ();

    void stopSimulation ();

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

    /**
     *
     *	@brief Initialize server object to its default state.
     *	@retval True if the operation was successful, false otherwise.
     *
     **/

    bool initializeServer ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

private:

    void OnShardReconnect (const boost::system::error_code &error);

    void OnNameReconnect (const boost::system::error_code &error);

    void OnServer ();

    void OnServerClose ();

    void OnAgent (const boost::shared_ptr<IConnection> &conn);

    void OnAgentRemove (const uint32_t connID);

    void OnActiveGame (const boost::shared_ptr<IConnection> &conn);

    void OnComponent (const uint8_t type, const std::string &host, const std::string &port);

    void OnComponentQuery (const uint8_t type);

    void OnLoadCharacter (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

    void OnSimulationRun ();

    void OnSimulationStop ();

private:

    std::string m_error;
    std::string m_host;
    std::string m_port;

    ServiceClt m_shard;
    ServiceClt m_name;

    boost::shared_ptr<Server> m_server;

    bool m_chard_state;
    boost::shared_ptr<clt::Connection> m_chard_client;

    boost::mutex m_agent_mutex;
    std::vector<boost::shared_ptr<IConnection> > m_agents;

    boost::mutex m_client_mutex;
    client_index m_clients;

    boost::shared_ptr<World> m_world;

    std::map<std::string, std::string> m_config;
};

#endif // __ZELOS_GAMED_SERVICE_H__
