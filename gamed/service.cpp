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

#include "service.h"

#include <auth/authentication_sro.h>
#include <name/packet.h>
#include <name/services.h>
#include <net/client/connection.h>
#include <sso/property/character.h>

#include "characterd/packet.h"
#include "characterd/states.h"
#include "characterd/state_handshake.h"
#include "characterd/state_chard.h"
#include "named/states.h"
#include "named/state_handshake.h"
#include "named/state_name.h"
#include "server/server.h"
#include "server/packet_game.h"
#include "shardd/states.h"
#include "shardd/state_handshake.h"
#include "shardd/state_shard.h"
#include "shardd/packet_shard.h"

#define RECONNECT_TIME 10

#define ZONE_GRID_COUNT 5
#define ZONE_GRID_SIZE (1920/ZONE_GRID_COUNT)

uint32_t ZoneID (const uint16_t zone, float x, float z)
{
    uint16_t dx = ZONE_GRID_COUNT*((zone & 0xFF)-1) + (x/ZONE_GRID_SIZE);
    uint16_t dz = ZONE_GRID_COUNT*((zone >> 8)-1) + (z/ZONE_GRID_SIZE);

    return (dz << 16) + dx;
}

enum CLIENT_SERVICES
{
    CLIENT_CHARD,
    CLIENT_NAMED,
    CLIENT_SHARDD
};

Service::Service()
    : m_shard(m_io_service),
    m_name(m_io_service),
    m_chard_state(false)
{
}

Service::~Service ()
{
}

const std::string& Service::getLastError () const
{
    if (m_error.empty() && m_server)
        return m_server->GetLastError();

    return m_error;
}

const std::string& Service::getHost () const
{
    return m_host;
}

const std::string& Service::getPort () const
{
    return m_port;
}

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServer,this));
}

void Service::addAgent (const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnAgent,this,conn));
}

void Service::removeAgent (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnAgentRemove,this,connID));
}

void Service::addComponent (const uint8_t type, const std::string &host, const std::string &port)
{
    m_io_service.post(boost::bind(&Service::OnComponent,this,type,host,port));
}

void Service::loadCharacter (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnLoadCharacter,this,accountID,conn));
}

void Service::runSimulation ()
{
    m_io_service.post(boost::bind(&Service::OnSimulationRun,this));
}

void Service::stopSimulation ()
{
    m_io_service.post(boost::bind(&Service::OnSimulationStop,this));
}

bool Service::initializeShard ()
{
    m_shard.conn.reset(new clt::Connection(CLIENT_SHARDD,this,m_io_service));

    if (!m_shard.conn)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_shard.conn->setAuthentication((auth));

    state.reset(new shard::StateShard(this,m_shard.conn));

    if (!state)
        return false;

    m_shard.conn->addState(state,shard::STATE_SHARD);

    state.reset(new shard::StateHandshake(this,"ShardManager","GameServer",m_shard.conn));

    if (!state)
        return false;

    m_shard.conn->addState(state,shard::STATE_HANDSHAKE);

    m_shard.conn->setState(shard::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeName ()
{
    m_name.conn.reset(new clt::Connection(CLIENT_NAMED,this,m_io_service));

    if (!m_name.conn)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_name.conn->setAuthentication(auth);

    state.reset(new named::StateName(this,m_name.conn));

    if (!state)
        return false;

    m_name.conn->addState(state,named::STATE_NAME);

    state.reset(new named::StateHandshake(m_name.conn));

    if (!state)
        return false;

    m_name.conn->addState(state,named::STATE_HANDSHAKE);

    m_name.conn->setState(named::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeChar ()
{
    m_chard_client.reset(new clt::Connection(CLIENT_CHARD,this,m_io_service));

    if (!m_chard_client)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_chard_client->setAuthentication(auth);

    state.reset(new chard::StateChard(this,m_chard_client));

    if (!state)
        return false;

    m_chard_client->addState(state,chard::STATE_CHARD);

    state.reset(new chard::StateHandshake("CharacterServer","GameServer",m_chard_client));

    if (!state)
        return false;

    m_chard_client->addState(state,chard::STATE_HANDSHAKE);

    m_chard_client->setState(chard::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeServer ()
{
    m_server.reset(new Server(this,m_io_service));

    if (!m_server)
    {
        m_error = "Failed to initialize server.";
        return false;
    }

    m_server->configure(m_config);

    if (!m_server->initialize())
        return false;

    m_server->setCloseCallback(boost::bind(&Service::OnServerClose,this));

    return true;
}

bool Service::OnInitialize ()
{
    if (m_host.empty())
    {
        return false;
    }

    if (m_port.empty())
    {
        return false;
    }

    if (m_shard.host.empty())
    {
        return false;
    }

    if (m_shard.port.empty())
    {
        return false;
    }

    if (m_name.host.empty())
    {
        return false;
    }

    if (m_name.port.empty())
    {
        return false;
    }

    if (!initializeServer())
        return false;

    if (!initializeShard())
    {
        m_error = "Failed to initialize shard client.";
        return false;
    }

    if (!initializeName())
    {
        m_error = "Failed to initialize name client";
        return false;
    }

    if (!initializeChar())
    {
        m_error = "Failed to initialize character client.";
        return false;
    }

    syslog(LOG_INFO,"Connecting to shard server.");

    m_shard.connect();

    syslog(LOG_INFO,"Connecting to name server.");

    m_name.connect();

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "ShardHost")
            m_shard.host = it->second;
        else if (it->first == "ShardPort")
            m_shard.port = it->second;
        else if (it->first == "NameHost")
            m_name.host = it->second;
        else if (it->first == "NamePort")
            m_name.port = it->second;
        else if (it->first == "Host")
            m_host = it->second;
        else if (it->first == "Port")
            m_port = it->second;
    }

    m_config = entries;
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    switch(type)
    {
    case CLIENT_SHARDD:

        if (__sync_bool_compare_and_swap(&m_shard.state,!state,state))
        {
            syslog(LOG_INFO,"Connection with shard server has been %s.",
                   state ? "established" : "terminated");

            if (!state)
            {
                m_server->Stop();

                m_shard.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
                m_shard.timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                                     boost::asio::placeholders::error));
            }
        }

        break;
    case CLIENT_NAMED:

        if (__sync_bool_compare_and_swap(&m_name.state,!state,state))
        {
            syslog(LOG_INFO,"Connection with name server has been %s.",
                   state ? "established" : "terminated");

            if (!state)
            {
                m_name.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
                m_name.timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                                    boost::asio::placeholders::error));
            }
        }

        break;
    case CLIENT_CHARD:

        if (__sync_bool_compare_and_swap(&m_chard_state,!state,state))
        {
            syslog(LOG_INFO,"Connection with character server has been %s.",
                   state ? "established" : "terminated");

            if (!state)
                m_io_service.post(boost::bind(&Service::OnComponentQuery,this,SERVICE_CHARACTER));
        }

        break;
    default:
        assert(0);
        break;
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    switch(type)
    {
    case CLIENT_SHARDD:

        syslog(LOG_INFO,"Shard client, %s.",error.message().c_str());

        m_shard.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_shard.timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                             boost::asio::placeholders::error));
        break;
    case CLIENT_NAMED:

        syslog(LOG_INFO,"Name client, %s.",error.message().c_str());

        m_name.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_name.timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                            boost::asio::placeholders::error));
        break;
    case CLIENT_CHARD:
        break;
    default:
        assert(0);
        break;
    }
}

void Service::OnRelease ()
{
    if (m_shard.conn)
        m_shard.conn->stop();

    if (m_name.conn)
        m_name.conn->stop();

    if (m_chard_client)
        m_chard_client->stop();

    if (m_server)
    {
        m_server->setCloseCallback(boost::function<void ()>());
        m_server->Stop();
    }
}

void Service::OnShardReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        if (initializeShard())
        {
            syslog(LOG_INFO,"Trying to connect to shard server.");

            m_shard.connect();
        }
        else
        {
            m_shard.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_shard.timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                                 boost::asio::placeholders::error));
        }
    }
}

void Service::OnNameReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        if (initializeName())
        {
            syslog(LOG_INFO,"Trying to connect to name server.");

            m_name.connect();
        }
        else
        {
            m_name.timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_name.timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnServer ()
{
    syslog(LOG_INFO,"Starting Server");

    m_server->Execute();
}

void Service::OnServerClose ()
{
    /// CLEAR ALL SERVER SIDE DATA

    boost::lock(m_client_mutex,m_agent_mutex);
    boost::unique_lock<boost::mutex> clock(m_client_mutex,boost::adopt_lock);
    boost::unique_lock<boost::mutex> alock(m_agent_mutex,boost::adopt_lock);

    /// SERVICE SIDE CONNECTIONS
    m_agents.clear();

    ///	QUEUE CLIENTS
    m_clients.clear();

    /// WORLD DATA

    if(!initializeServer())
    {
        syslog(LOG_INFO,"Failed to initialize server.");

        stop();
    }
}

void Service::OnAgent (const boost::shared_ptr<IConnection> &conn)
{
    syslog(LOG_INFO,"Registering agent server - ID = %i",conn->ID());

    boost::lock_guard<boost::mutex> lock(m_agent_mutex);

    m_agents.push_back(conn);
}

void Service::OnAgentRemove (const uint32_t connID)
{
    syslog(LOG_INFO,"Connection with agent server - ID = %i has been terminated.",connID);

    boost::lock(m_agent_mutex,m_client_mutex);
    boost::unique_lock<boost::mutex> alock(m_agent_mutex,boost::adopt_lock);
    boost::unique_lock<boost::mutex> clock(m_client_mutex,boost::adopt_lock);

    m_agents.erase(std::remove_if(m_agents.begin(),m_agents.end(),
                                  boost::bind(&IConnection::ID,_1) == connID),m_agents.end());

    /// REMOVE ALL CLIENTS ASSOCIATED WITH THE AGENT
    CIndexAgent &cidx = boost::multi_index::get<Client::tagAgent>(m_clients);

    cidx.erase(connID);

    /// TODO: REMOVE ALL ASSOCIATED PLAYERS FROM WORLD AND ZONES
}

void Service::OnComponent (const uint8_t type, const std::string &host, const std::string &port)
{
    switch(type)
    {
    case SERVICE_CHARACTER:

        syslog(LOG_INFO,"Connecting to character service.");

        if (!__sync_fetch_and_add(&m_chard_state,0))
            m_chard_client->connect(host,port);

        break;
    default:
        break;
    }
}

void Service::OnComponentQuery (const uint8_t type)
{
    if (type == SERVICE_CHARACTER)
    {
        syslog(LOG_INFO,"Connection with character service has been terminated.");

        if (initializeChar() && __sync_fetch_and_add(&m_name.state,0))
        {
            syslog(LOG_INFO,"Searching for a new character service.");

            OPacket pkt;
            clt_pkt::Query(&pkt,SERVICE_CHARACTER);
            m_name.conn->send(&pkt);
        }
        else
            syslog(LOG_INFO,"Failed to initialize character client.");
    }
}

void Service::OnLoadCharacter (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{
    OPacket pkt;
    clt_pkt::Character(&pkt,accountID);
    m_chard_client->send(&pkt);

    Client clt;
    clt.accountID = accountID;
    clt.agentID = conn->ID();
    clt.agent = conn;

    boost::lock_guard<boost::mutex> lock(m_client_mutex);
    m_clients.insert(clt);
}

void Service::OnSimulationRun ()
{
    syslog(LOG_INFO,"Starting world simulation.");
}

void Service::OnSimulationStop ()
{
    syslog(LOG_INFO,"Ending world simulation.");
}
