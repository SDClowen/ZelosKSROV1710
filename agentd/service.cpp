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

#include "gamed/connection.h"
#include "gamed/packet.h"
#include "gamed/states.h"
#include "gamed/state_handshake.h"
#include "gamed/state_game.h"
#include "characterd/states.h"
#include "characterd/state_handshake.h"
#include "characterd/state_player.h"
#include "named/states.h"
#include "named/state_handshake.h"
#include "named/state_name.h"
#include "server/connection.h"
#include "server/states.h"
#include "server/server.h"
#include "server/packet_login.h"
#include "shardd/states.h"
#include "shardd/state_handshake.h"
#include "shardd/state_shard.h"
#include "shardd/packet_shard.h"

#define MAX_RECONNECT 3
#define RECONNECT_TIME 5

enum CLIENT_TYPE
{
    CLIENT_GAMED,
    CLIENT_CHARD,
    CLIENT_NAMED,
    CLIENT_SHARDD
};

Service::Service ()
    : m_state(false),
    m_locale(0),
    m_race(0),
    m_maxAreas(0),
    m_version(0),
    m_shard_state(false),
    m_shard_timer(m_io_service),
    m_name_state(false),
    m_name_timer(m_io_service),
    m_chard_state(false),
    m_chard_active(false),
    m_chard_attempts(0),
    m_chard_timer(m_io_service),
    m_area_strand(m_io_service)
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

void Service::setLocale (const uint8_t locale)
{
    m_locale = locale;
}

uint8_t Service::getLocale () const
{
    return m_locale;
}

void Service::setRace (const uint8_t race)
{
    m_race = race;
}

void Service::setVersion (const uint32_t version)
{
    m_version = version;
}

void Service::setMaxAreas (const uint8_t size)
{
    m_maxAreas = size;
}

void Service::setFilter(const std::string &chFilter, const std::vector<std::string> &strFilter)
{
    m_chFilter = chFilter;
    m_strFilter = strFilter;
}

void Service::addArea (const Area &area)
{
    m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnArea,this,area)));
}

void Service::activateGShard (const uint32_t connID)
{
    m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardActivate,this,connID)));
}

void Service::addTicket (const Ticket &ticket)
{
    m_io_service.post(boost::bind(&Service::OnTicket,this,ticket));
}

void Service::processTicket (const uint32_t ticketID, const uint32_t accountID, const uint8_t secLv)
{
    m_io_service.post(boost::bind(&Service::OnLoginSuccess,this,ticketID,accountID,secLv));
}

void Service::processTicket (const uint32_t ticketID, const uint16_t error_code)
{
    m_io_service.post(boost::bind(&Service::OnLoginError,this,ticketID,error_code));
}

boost::shared_ptr<IConnection> Service::getCharClient () const
{
    return m_chard_client;
}

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServerRun,this));
}

void Service::stopServer ()
{
    m_io_service.post(boost::bind(&Service::OnServerStop,this));
}

void Service::addComponent(const uint8_t type, const std::string &host, const std::string &port)
{
    m_io_service.post(boost::bind(&Service::OnComponent,this,type,host,port));
}

void Service::reconnectComponent (const uint8_t type)
{
    m_io_service.post(boost::bind(&Service::OnComponentQuery,this,type));
}

void Service::activateComponent (const uint8_t type)
{
    if (__sync_bool_compare_and_swap(&m_chard_active,false,true))
        m_io_service.post(boost::bind(&Service::OnCheckStatus,this));
}

uint32_t Service::getAccountID (const uint32_t connID) const
{
    uint32_t id = 0;

    boost::lock_guard<boost::mutex> lock(m_client_mutex);

    client_set::const_iterator iter = m_clients.find(connID);

    if (iter != m_clients.end())
        id = iter->accountID;

    return id;
}

void Service::setAccountArea (const uint32_t accountID, const uint16_t zoneID)
{
    m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardMapAccount,this,accountID,zoneID)));
}

uint32_t Service::getConnectionID (const uint32_t accountID) const
{
    uint32_t id = 0;

    boost::lock_guard<boost::mutex> lock(m_client_mutex);

    const CIndexAcc &idx = boost::multi_index::get<Client::tagAcc>(m_clients);

    CIndexAcc::const_iterator iter = idx.find(accountID);

    if (iter != idx.end())
        id = iter->connID;

    return id;
}

boost::shared_ptr<IConnection> Service::findConnection (const uint32_t accountID) const
{
    boost::shared_ptr<IConnection> conn;

    boost::lock_guard<boost::mutex> lock(m_client_mutex);

    const CIndexAcc &idx = boost::multi_index::get<Client::tagAcc>(m_clients);

    CIndexAcc::const_iterator iter = idx.find(accountID);

    if (iter != idx.end())
        conn = iter->conn;

    return conn;
}

void Service::removeConnection (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveConnection,this,connID));
}

void Service::killConnection (const uint32_t accountID)
{
    m_io_service.post(boost::bind(&Service::OnKillConnection,this,accountID));
}

bool Service::initializeShard ()
{
    m_shard_client.reset(new clt::Connection(CLIENT_SHARDD,this,m_io_service));

    if (!m_shard_client)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_shard_client->setAuthentication((auth));

    state.reset(new shard::StateShard(this,m_shard_client));

    if (!state)
        return false;

    m_shard_client->addState(state,shard::STATE_SHARD);

    state.reset(new shard::StateHandshake(this,"ShardManager","AgentServer",m_shard_client));

    if (!state)
        return false;

    m_shard_client->addState(state,shard::STATE_HANDSHAKE);

    m_shard_client->setState(shard::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeName ()
{
    m_name_client.reset(new clt::Connection(CLIENT_NAMED,this,m_io_service));

    if (!m_name_client)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_name_client->setAuthentication(auth);

    state.reset(new named::StateName(this,m_name_client));

    if (!state)
        return false;

    m_name_client->addState(state,named::STATE_NAME);

    state.reset(new named::StateHandshake(m_name_client));

    if (!state)
        return false;

    m_name_client->addState(state,named::STATE_HANDSHAKE);

    m_name_client->setState(named::STATE_HANDSHAKE);

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

    state.reset(new chard::StatePlayer(this,m_chard_client));

    if (!state)
        return false;

    m_chard_client->addState(state,chard::STATE_PLAYER);

    state.reset(new chard::StateHandshake(this,"CharacterServer","AgentServer",m_chard_client));

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
        return false;

    m_server->configure(m_config);

    if (!m_server->initialize())
        return false;

    return true;
}

boost::shared_ptr<IConnection> Service::createGShardConnection (const uint32_t areaID)
{
    boost::shared_ptr<IConnection> conn;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return conn;

    conn.reset(new gamed::Connection(areaID,CLIENT_GAMED,this,m_io_service));

    if (conn)
    {
        conn->setAuthentication(auth);

        boost::shared_ptr<IConnection::State> state;

        state.reset(new gamed::StateGame(conn));

        if (!state)
        {
            conn.reset();

            return conn;
        }

        conn->addState(state,gamed::STATE_GAME);

        state.reset(new gamed::StateHandshake(this,"GameServer","AgentServer",conn));

        if (!state)
        {
            conn.reset();

            return conn;

        }

        conn->addState(state,gamed::STATE_HANDSHAKE);

        conn->setState(gamed::STATE_HANDSHAKE);
    }

    return conn;
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

    if (m_name_host.empty())
    {
        return false;
    }

    if (m_name_port.empty())
    {
        return false;
    }

    if (!initializeServer())
        return false;

    m_server->setCloseCallback(boost::bind(&Service::OnServerClosed,this));

    if (!initializeShard())
    {
        m_error = "Failed to initialize shard client.";
        return false;
    }

    if (!initializeName())
    {
        m_error = "Failed to initialize name client.";
        return false;
    }

    if (!initializeChar())
    {
        m_error = "Failed to initialize character client.";
        return false;
    }

    m_shard_client->connect(m_shard_host,m_shard_port);

    m_name_client->connect(m_name_host,m_name_port);

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "ShardHost")
            m_shard_host = it->second;
        else if (it->first == "ShardPort")
            m_shard_port = it->second;
        else if (it->first == "NameHost")
            m_name_host = it->second;
        else if (it->first == "NamePort")
            m_name_port = it->second;
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
    case CLIENT_CHARD:
        m_io_service.post(boost::bind(&Service::OnComponentState,this,SERVICE_CHARACTER,state));
        break;
    case CLIENT_NAMED:
        m_io_service.post(boost::bind(&Service::OnNameState,this,state));
        break;
    case CLIENT_SHARDD:
        m_io_service.post(boost::bind(&Service::OnShardState,this,state));
        break;
    case CLIENT_GAMED:
        m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardState,this,connID,state)));
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
    case CLIENT_GAMED:

        syslog(LOG_INFO,"Area server %i connection, %s",connID,error.message().c_str());

        m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardError,this,connID)));
        break;
    case CLIENT_CHARD:

        syslog(LOG_INFO,"Character service client, %s",error.message().c_str());

        m_io_service.post(boost::bind(&Service::OnComponentError,this,SERVICE_CHARACTER));
        break;
    case CLIENT_NAMED:

        syslog(LOG_INFO,"Name client, %s",error.message().c_str());

        m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_name_timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                            boost::asio::placeholders::error));

        break;
    case CLIENT_SHARDD:

        syslog(LOG_INFO,"Shard client, %s",error.message().c_str());

        m_shard_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_shard_timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                             boost::asio::placeholders::error));

        break;
    default:
        assert(0);
        break;
    }
}

void Service::OnRelease ()
{
    m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardClose,this)));

    if (m_shard_client)
        m_shard_client->stop();

    if (m_name_client)
        m_name_client->stop();

    if (m_chard_client)
        m_chard_client->stop();

    if (m_server)
        m_server->Stop();
}

void Service::OnCheckStatus ()
{
    if (__sync_fetch_and_add(&m_chard_active,0) && __sync_bool_compare_and_swap(&m_state,false,true))
        OnStatus(true);
}

void Service::OnStatus (bool status)
{
    if (__sync_fetch_and_add(&m_shard_state,0))
    {
        OPacket pkt;
        clt_pkt::State(&pkt,status);
        m_shard_client->send(&pkt);
    }
}

void Service::OnShardState (const bool state)
{
    if (__sync_bool_compare_and_swap(&m_shard_state,!state,state))
    {
        syslog(LOG_INFO,"Connection with shard server %s.",state ? "established" : "terminated");

        if (!state)
        {
            __sync_lock_test_and_set(&m_state,false);

            m_server->Stop();

            m_io_service.post(m_area_strand.wrap(boost::bind(&Service::OnGShardClose,this)));

            m_shard_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_shard_timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnShardReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to shard server.");

        if (initializeShard())
        {
            m_shard_client->connect(m_shard_host,m_shard_port);
        }
        else
        {
            m_shard_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_shard_timer.async_wait(boost::bind(&Service::OnShardReconnect,this,
                                                 boost::asio::placeholders::error));
        }
    }
}

void Service::OnNameReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to name server.");

        if (initializeName())
        {
            m_name_client->connect(m_name_host,m_name_port);
        }
        else
        {
            m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_name_timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnNameState (const bool state)
{
    if (__sync_bool_compare_and_swap(&m_name_state,!state,state))
    {
        syslog(LOG_INFO,"Connection with name server %s.",state ? "established" : "terminated");

        if (!state)
        {
            m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_name_timer.async_wait(boost::bind(&Service::OnNameReconnect,this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnServerRun ()
{
    if (m_server)
    {
        syslog(LOG_INFO,"Launching server.");

        m_server->Execute();
    }
}

void Service::OnServerStop ()
{
    if (m_server)
    {
        syslog(LOG_INFO,"Closing server.");

        m_server->Stop();
    }
}

void Service::OnServerClosed ()
{
    if(!initializeServer())
    {
        syslog(LOG_INFO,"Failed to initialize server.");

        stop();
    }
}

void Service::OnArea (Area area)
{
    std::vector<Area>::iterator iter = std::find_if(m_areas.begin(),m_areas.end(),
                                                    boost::bind(&Area::ID,_1) == area.ID);

    if (iter == m_areas.end())
    {
        area.timer.reset(new boost::asio::deadline_timer(m_io_service));

        if (area.timer)
        {
            area.conn = createGShardConnection(area.ID);

            m_areas.push_back(area);
        }
        else
        {
            syslog(LOG_INFO,"Insufficient memory.");

            stop();
        }
    }
    else
    {
        if (iter->host != area.host || iter->port != area.port)
        {
            if (iter->conn)
            {
                iter->state = false;
                iter->active = false;

                if (__sync_bool_compare_and_swap(&m_state,true,false))
                    OnStatus(false);

                iter->conn->stop();

                iter->conn.reset();
            }

            area.conn = iter->conn = createGShardConnection(area.ID);
            area.timer = iter->timer;
            iter->host = area.host;
            iter->port = area.port;
        }
    }

    if (area.conn)
    {
        syslog(LOG_INFO,"Connecting to Game Shard = %i",area.ID);

        area.conn->connect(area.host,area.port);
    }
    else
    {
        area.timer->expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        area.timer->async_wait(m_area_strand.wrap(boost::bind(&Service::OnGShardReconnect,this,area.ID,
                                                              boost::asio::placeholders::error)));
    }
}

void Service::OnGShardReconnect (const uint32_t areaID, const boost::system::error_code &error)
{
    if (!error)
    {
        std::vector<Area>::iterator iter = std::find_if(m_areas.begin(),m_areas.end(),
                                                        boost::bind(&Area::ID,_1) == areaID);

        if (iter != m_areas.end())
        {
            iter->conn = createGShardConnection(areaID);

            if (iter->conn)
            {
                syslog(LOG_INFO,"Trying to connect to Game Shard %i",areaID);

                iter->conn->connect(iter->host,iter->port);
            }
            else
            {
                iter->timer->expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
                iter->timer->async_wait(m_area_strand.wrap(boost::bind(&Service::OnGShardReconnect,this,areaID,
                                                                       boost::asio::placeholders::error)));
            }
        }
    }
}

void Service::OnGShardState (const uint32_t connID, const bool state)
{
    std::vector<Area>::iterator iter = std::find_if(m_areas.begin(),m_areas.end(),
                                                    boost::bind(&Area::ID,_1) == connID);

    if (iter != m_areas.end())
    {
        iter->state = state;

        syslog(LOG_INFO,"Connection to Game Shard %i has been %s.",
               connID,state ? "established" : "terminated");

        if (!state)
        {
            iter->active = false;

            if (__sync_bool_compare_and_swap(&m_state,true,false))
                OnStatus(false);
        }
    }
}

void Service::OnGShardError (const uint32_t connID)
{
    std::vector<Area>::iterator iter = std::find_if(m_areas.begin(),m_areas.end(),
                                                    boost::bind(&Area::ID,_1) == connID);

    if (iter != m_areas.end())
    {
        iter->timer->expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        iter->timer->async_wait(m_area_strand.wrap(boost::bind(&Service::OnGShardReconnect,this,iter->ID,
                                                               boost::asio::placeholders::error)));
    }
}

void Service::OnGShardActivate (const uint32_t connID)
{
    std::vector<Area>::iterator iter = std::find_if(m_areas.begin(),m_areas.end(),
                                                    boost::bind(&Area::ID,_1) == connID);

    if (iter != m_areas.end())
    {
        iter->active = true;

        syslog(LOG_INFO,"Connection to Game Shard %i is active.",iter->conn->ID());

        /// CHECK IF ALL AREA CLIENT CONNECTION HAS BEEN ESTABLISHED AND HANDSHAKED SUCCESSFULLY
        iter = std::find_if(m_areas.begin(),m_areas.end(),
                            boost::bind(&Area::active,_1) == false);

        if (iter == m_areas.end())
            m_io_service.post(boost::bind(&Service::OnCheckStatus,this));
    }
}

void Service::OnGShardMapAccount (const uint32_t accountID, const uint16_t zoneID)
{
    uint32_t areaID = 0;
    boost::shared_ptr<IConnection> conn;

    for (size_t i = 0; i < m_areas.size(); ++i)
    {
        if (m_areas[i].Zones.find(zoneID) != m_areas[i].Zones.end())
        {
            areaID = m_areas[i].ID;
            conn = m_areas[i].conn;
            break;
        }
    }

    if (areaID)
    {
        boost::unique_lock<boost::mutex> lock(m_client_mutex);

        CIndexAcc &idx = boost::multi_index::get<Client::tagAcc>(m_clients);

        CIndexAcc::iterator iter = idx.find(accountID);

        if (iter != idx.end())
        {
            Client clt = *iter;
            clt.areaID = areaID;

            idx.replace(iter,clt);
        }

        lock.unlock();

        OPacket pkt;
        clt_pkt::LoadCharacter(&pkt,accountID);
        conn->send(&pkt);
    }
}

void Service::OnGShardClose ()
{
#ifdef DEBUG
    syslog(LOG_INFO,"Terminating Game Shards connections.");
#endif

    for (std::vector<Area>::iterator it = m_areas.begin(); it != m_areas.end(); ++it)
    {
        if (it->conn)
            it->conn->stop();
    }

    m_areas.clear();
}

void Service::OnTicket (const Ticket &ticket)
{
    OPacket pkt;
    clt_pkt::LoginTicket(&pkt,&ticket);
    m_shard_client->send(&pkt);

    boost::lock_guard<boost::mutex> lock(m_ticket_mutex);
    m_tickets.insert(std::make_pair(ticket.ID,ticket.conn));
}

void Service::OnLoginSuccess (const uint32_t ticketID, const uint32_t accountID, const uint8_t secLv)
{
    boost::unique_lock<boost::mutex> tlock(m_ticket_mutex);

    ticket_iterator iter = m_tickets.find(ticketID);

    boost::shared_ptr<IConnection> conn;

    if (iter != m_tickets.end())
    {
        conn = iter->second;

        m_tickets.erase(iter);
    }

    tlock.unlock();

    if (conn)
    {
        OPacket pkt;
        srv_pkt::Login(&pkt);
        conn->send(&pkt);

        conn->setState(srv::STATE_LOBBY);

        boost::shared_ptr<agentd::Connection> srvConn =
                boost::shared_static_cast<agentd::Connection,IConnection>(conn);

        assert(srvConn);

        srvConn->setAccountID(accountID);

        pkt.Clear();

        clt_pkt::AccountState(&pkt,accountID,true);
        m_shard_client->send(&pkt);

        Client client;
        client.connID = conn->ID();
        client.accountID = accountID;
        client.areaID = 0;
        client.conn = conn;

        boost::lock_guard<boost::mutex> mlock(m_client_mutex);
        m_clients.insert(client);
    }
}

void Service::OnLoginError (const uint32_t ticketID, const int error_code)
{
    boost::unique_lock<boost::mutex> tlock(m_ticket_mutex);

    ticket_iterator iter = m_tickets.find(ticketID);

    boost::shared_ptr<IConnection> conn;

    if (iter != m_tickets.end())
    {
        conn = iter->second;

        m_tickets.erase(iter);
    }

    tlock.unlock();

    if (conn)
    {
        OPacket pkt;
        srv_pkt::Login(&pkt,error_code);
        conn->send(&pkt);
    }
}

void Service::OnComponent (const uint8_t type, const std::string &host, const std::string &port)
{
    if (type == SERVICE_CHARACTER)
    {
        if (!__sync_fetch_and_add(&m_chard_state,0))
        {
            syslog(LOG_INFO,"Connecting to character service.");

            m_chard_host = host;
            m_chard_port = port;

            m_chard_client->connect(host,port);
        }
    }
}

void Service::OnComponentState (const uint8_t type, const bool state)
{
    if (type == SERVICE_CHARACTER)
    {
        if (__sync_bool_compare_and_swap(&m_chard_state,!state,state))
        {
            syslog(LOG_INFO,"Connection with character service has been %s.",
                   state ? "established" : "terminated");

            if (!state && __sync_bool_compare_and_swap(&m_chard_active,true,false))
            {
                OnStatus(false);

                m_io_service.post(boost::bind(&Service::OnComponentQuery,this,type));
            }
        }
    }
}

void Service::OnComponentError (const uint8_t type)
{
    if (type == SERVICE_CHARACTER)
    {
        if (__sync_fetch_and_add(&m_chard_attempts,1) <= MAX_RECONNECT )
        {
            m_chard_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_chard_timer.async_wait(boost::bind(&Service::OnComponentReconnect,this,type,
                                                 boost::asio::placeholders::error));
        }
        else
        {
            m_io_service.post(boost::bind(&Service::OnComponentQuery,this,type));
        }
    }
}

void Service::OnComponentQuery (const uint8_t type)
{
    if (type == SERVICE_CHARACTER)
    {
        syslog(LOG_INFO,"Connection with character service has been terminated.");

        if (initializeChar())
        {
            if (__sync_fetch_and_add(&m_name_state,0))
            {
                syslog(LOG_INFO,"Searching for a new character service.");

                OPacket pkt;
                clt_pkt::Query(&pkt,SERVICE_CHARACTER);
                m_name_client->send(&pkt);
            }
        }
        else
            syslog(LOG_INFO,"Failed to initialize character client.");
    }
}

void Service::OnComponentReconnect (const uint8_t type, const boost::system::error_code &error)
{
    if (!error)
    {
        if (type == SERVICE_CHARACTER)
        {
            if (initializeChar())
            {
                syslog(LOG_INFO,"Trying to connect to character service.");

                m_chard_client->connect(m_chard_host,m_chard_port);
            }
            else
            {
                m_chard_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
                m_chard_timer.async_wait(boost::bind(&Service::OnComponentReconnect,this,type,
                                                     boost::asio::placeholders::error));
            }
        }
    }
}

void Service::OnRemoveConnection (const uint32_t connID)
{
    boost::lock_guard<boost::mutex> lock(m_client_mutex);

    client_set::iterator iter = m_clients.find(connID);

    if (iter != m_clients.end())
    {
        OPacket pkt;
        clt_pkt::AccountState(&pkt,iter->accountID,false);
        m_shard_client->send(&pkt);

        m_clients.erase(iter);
    }
}

void Service::OnKillConnection (const uint32_t accountID)
{
    boost::lock_guard<boost::mutex> lock(m_client_mutex);

    CIndexAcc &idx = boost::multi_index::get<Client::tagAcc>(m_clients);

    CIndexAcc::iterator iter = idx.find(accountID);

    if (iter != idx.end())
    {
        OPacket pkt;
        clt_pkt::AccountState(&pkt,iter->accountID,false);
        m_shard_client->send(&pkt);

        if (iter->conn)
            iter->conn->stop();

        m_clients.erase(accountID);
    }
}
