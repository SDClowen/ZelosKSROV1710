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
#include <net/client/connection.h>
#include <packet/packet_group.h>

#include "certd/states.h"
#include "certd/state_handshake.h"
#include "certd/state_certify.h"
#include "globald/states.h"
#include "globald/state_handshake.h"
#include "globald/state_global.h"
#include "globald/packet_global.h"
#include "server/server.h"
#include "server/packet_agent.h"
#include "server/packet_game.h"

#define RECONNECT_TIME 10

enum CLIENT_SERVICES
{
    CLIENT_CERTD,
    CLIENT_GLOBALD
};

Service::Service ()
    : m_ID(0),
    m_locale(0),
    m_version(0),
    m_state(0),
    m_user_count(0),
    m_user_max(0),
    m_cert_timer(m_io_service),
    m_global_timer(m_io_service),
    m_server(new Server(this,m_io_service))
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

void Service::runGlobal ()
{
    m_io_service.post(boost::bind(&Service::OnGlobal,this));
}

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServer,this));
}

void Service::setShardID (const uint16_t ID)
{
    m_ID = ID;
}

void Service::setLocale (const uint8_t locale)
{
    m_locale = locale;
}

void Service::setRace (const uint8_t race)
{
    m_race = race;
}

void Service::setVersion (const uint32_t version)
{
    m_version = version;
}

void Service::setFilter(const std::string &chFilter, const std::vector<std::string> &strFilter)
{
    m_chFilter = chFilter;
    m_strFilter = strFilter;
}

void Service::addAgent (const std::string &host, const std::string &port, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnAgent,this,host,port,conn));
}

void Service::updateAgent (const uint32_t connID, const uint8_t state)
{
    m_io_service.post(boost::bind(&Service::OnAgentState,this,connID,state));
}

void Service::broadcastAgents (OPacket *pkt)
{
    boost::lock_guard<boost::mutex> lock(m_agsrv_mutex);

    for (std::vector<Agent>::iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->conn)
            it->conn->send(pkt);
    }
}

void Service::removeAgent (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveAgent,this,connID));
}

void Service::addGShard (const std::string &host, const std::string &port, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnGShard,this,host,port,conn));
}

void Service::setGShardState (const uint32_t connID, const uint8_t state)
{
    m_io_service.post(boost::bind(&Service::OnGShardState,this,connID,state));
}

void Service::broadcastGShards (OPacket *pkt)
{
    boost::lock_guard<boost::mutex> lock(m_gshards_mutex);

    for (std::vector<GShard>::iterator it = m_gshards.begin(); it != m_gshards.end(); ++it)
    {
        if (it->conn)
            it->conn->send(pkt);
    }
}

void Service::removeGShard (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveGShard,this,connID));
}

void Service::setWorld (const std::vector<Area> &world)
{
    boost::lock_guard<boost::mutex> lock(m_world_mutex);

    m_world = world;
}

void Service::addWorldArea (const Area &area)
{
    boost::lock_guard<boost::mutex> lock(m_world_mutex);

    m_world.push_back(area);
}

void Service::removeWorldArea (int areaID)
{
    boost::lock_guard<boost::mutex> lock(m_world_mutex);

    for (std::vector<Area>::iterator iter = m_world.begin(); iter != m_world.end(); ++iter)
    {
        if (iter->ID == areaID)
        {
            if (iter->serverID)
            {
                /// TODO: SEND CLOSE NOTIFICATION!!
            }

            m_world.erase(iter);

            break;
        }
    }
}

void Service::addTicket (const Ticket &ticket)
{
    m_io_service.post(boost::bind(&Service::OnTicket,this,ticket));
}

void Service::certifyTicket (const uint32_t ticketID, const std::string &usr, const std::string &pw,
                             const std::string &host, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnCertifyTicket,this,ticketID,usr,pw,host,conn));
}

void Service::addAccount (const uint32_t connID, const uint32_t accountID)
{
    m_io_service.post(boost::bind(&Service::OnAccount,this,connID,accountID));
}

void Service::removeAccount (const uint32_t connID, const uint32_t accountID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveAccount,this,connID,accountID));
}

bool Service::initializeCert ()
{
    m_cert_client.reset(new clt::Connection(CLIENT_CERTD,this,m_io_service));

    if (!m_cert_client)
        return false;

    boost::shared_ptr<SROAuth> auth;

    boost::shared_ptr<clt::Connection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_cert_client->setAuthentication(auth);

    state.reset(new certd::StateCertify(this,m_cert_client));

    if (!state)
        return false;

    m_cert_client->addState(state,certd::STATE_CERTIFY);

    state.reset(new certd::StateHandshake(m_cert_client));

    if (!state)
        return false;

    m_cert_client->addState(state,certd::STATE_HANDSHAKE);

    m_cert_client->setState(certd::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeGlobal ()
{
    m_global_client.reset(new clt::Connection(CLIENT_GLOBALD,this,m_io_service));

    if (!m_global_client)
        return false;

    boost::shared_ptr<SROAuth> auth;

    boost::shared_ptr<clt::Connection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_global_client->setAuthentication(auth);

    state.reset(new globald::StateGlobal(this,m_global_client));

    if (!state)
        return false;

    m_global_client->addState(state,globald::STATE_GLOBAL);

    state.reset(new globald::StateHandshake(m_name,m_global_client));

    if (!state)
        return false;

    m_global_client->addState(state,globald::STATE_HANDSHAKE);

    m_global_client->setState(globald::STATE_HANDSHAKE);

    return true;
}

bool Service::OnInitialize ()
{
    if (!m_server || !m_server->initialize())
        return false;

    if (!initializeCert())
    {
        m_error = "Failed to initialize certify client.";
        return false;
    }

    if (!initializeGlobal())
    {
        m_error = "Failed to initialize global client.";
        return false;
    }

    m_cert_client->connect(m_cert_host,m_cert_port);

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    if (m_server)
        m_server->configure(entries);

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "CertifyHost")
            m_cert_host = it->second;
        else if (it->first == "CertifyPort")
            m_cert_port = it->second;
        else if (it->first == "GlobalHost")
            m_global_host = it->second;
        else if (it->first == "GlobalPort")
            m_global_port = it->second;
        else if (it->first == "ShardName")
            m_name = it->second;
    }
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    assert(type == CLIENT_CERTD || type == CLIENT_GLOBALD);

    if (type == CLIENT_CERTD)
    {
        syslog(LOG_INFO,"Connection with certify server has been %s.",
               state ? "established" : "terminated");
    }
    else
    {
        syslog(LOG_INFO,"Connection with global server has been %s.",
               state ? "established" : "terminated");

        if (!state)
        {
            m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_global_timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    assert(type == CLIENT_CERTD || type == CLIENT_GLOBALD);

    if (type == CLIENT_CERTD)
    {
        syslog(LOG_INFO,"Certify client, %s",error.message().c_str());

        m_cert_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_cert_timer.async_wait(boost::bind(&Service::OnCertReconnect,this,
                                           boost::asio::placeholders::error));
    }
    else
    {
        syslog(LOG_INFO,"Global client, %s",error.message().c_str());

        m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_global_timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                              boost::asio::placeholders::error));
    }
}

void Service::OnRelease ()
{
    m_global_client->stop();

    m_server->Stop();
}

bool Service::useSlot (const uint32_t accountID, const uint8_t accLv, const uint8_t secLv)
{
    if (accLv > 0 || secLv > 0 || __sync_fetch_and_add(&m_user_count,0) < m_user_max)
    {
        uint16_t usrs = __sync_fetch_and_add(&m_user_count,1);

        OPacket pkt;
        clt_pkt::ShardNotify16(&pkt,clt_pkt::SHARD_NOTIFY_USR_CNT,m_ID,usrs);
        m_global_client->send(&pkt);

        return true;
    }

    return false;
}

void Service::OnCertReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        if (initializeCert())
        {
            syslog(LOG_INFO,"Trying to connect to certify server.");

            m_cert_client->connect(m_cert_host,m_cert_port);
        }
        else
        {
            m_cert_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_cert_timer.async_wait(boost::bind(&Service::OnCertReconnect,this,
                                                      boost::asio::placeholders::error));
        }
    }
}

void Service::OnGlobal ()
{
    syslog(LOG_INFO,"Connecting to global manager.");

    m_global_client->connect(m_global_host,m_global_port);
}

void Service::OnGlobalReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        if (initializeGlobal())
        {
            syslog(LOG_INFO,"Trying to connect to global server.");

            m_global_client->connect(m_global_host,m_global_port);
        }
        else
        {
            m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_global_timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnServer ()
{
    syslog(LOG_INFO,"Launching server.");

    m_server->Execute();
}

void Service::OnCheckState ()
{
    boost::unique_lock<boost::mutex> alock(m_agsrv_mutex);

    bool open = false;

    for (std::vector<Agent>::const_iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->state)
        {
            open = true;
            break;
        }
    }

    alock.unlock();

    if (open)
    {
        boost::lock(m_world_mutex,m_gshards_mutex);

        boost::unique_lock<boost::mutex> wlock(m_world_mutex,boost::adopt_lock);

        boost::unique_lock<boost::mutex> lock(m_gshards_mutex,boost::adopt_lock);

        if (m_world.size() == m_gshards.size())
        {
            wlock.unlock();

            for (std::vector<GShard>::iterator it = m_gshards.begin(); it != m_gshards.end(); ++it)
            {
                if (!it->state)
                    open = false;
            }

            lock.unlock();
        }
        else
            open = false;
    }

    if (__sync_bool_compare_and_swap(&m_state,!open,open))
    {
        syslog(LOG_INFO,"Shard state %s.",open ? "open" : "close");

        OPacket pkt;
        clt_pkt::ShardNotify8(&pkt,clt_pkt::SHARD_NOTIFY_STATE,m_ID,open);
        m_global_client->send(&pkt);

        pkt.Clear();

        srv_pkt::AgentAction(&pkt,open);
        broadcastAgents(&pkt);

        pkt.Clear();

        srv_pkt::AreaAction(&pkt,open);
        broadcastGShards(&pkt);
    }
}

void Service::OnAgent (const std::string &host, const std::string &port, const boost::shared_ptr<IConnection> &conn)
{
    syslog(LOG_INFO,"Agent server registered - ID = %i",conn->ID());

    boost::unique_lock<boost::mutex> wlock(m_world_mutex);

    OPacket pkt;
    srv_pkt::AgentConfig(&pkt,m_locale,m_race,m_version,m_chFilter,m_strFilter,m_world.size());
    conn->send(&pkt);

    for (std::vector<Area>::const_iterator it = m_world.begin(); it != m_world.end(); ++it)
    {
        if (it->serverID)
        {
            pkt.Clear();

            srv_pkt::WorldArea(&pkt,&(*it));
            conn->send(&pkt);
        }
    }

    wlock.unlock();

    Agent agent;
    agent.conn = conn;
    agent.host = host;
    agent.port = port;
    agent.state = false;

    boost::lock_guard<boost::mutex> alock(m_agsrv_mutex);

    m_agent_srvs.push_back(agent);
}

void Service::OnAgentState (const uint32_t connID, const uint8_t state)
{
    syslog(LOG_INFO,"Agent server - ID = %i current state is %s",connID,state ? "open" : "close");

    boost::lock_guard<boost::mutex> lock(m_agsrv_mutex);

    for (std::vector<Agent>::iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->conn && it->conn->ID() == connID)
        {
            it->state = state;

            OPacket pkt;

            if (state)
                clt_pkt::AgentInsert(&pkt,m_ID,connID,it->host,it->port);
            else
                clt_pkt::AgentRemove(&pkt,m_ID,connID);

            m_global_client->send(&pkt);

            m_io_service.post(boost::bind(&Service::OnCheckState,this));
            break;
        }
    }
}

void Service::OnRemoveAgent (const uint32_t connID)
{
    syslog(LOG_INFO,"Connection with Agent server - ID = %i has been terminated.",connID);

    boost::unique_lock<boost::mutex> lock(m_agsrv_mutex);

    std::set<uint32_t> accounts;

    for (std::vector<Agent>::iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->conn && it->conn->ID() == connID)
        {
            if (it->state)
            {
                OPacket pkt;
                clt_pkt::AgentRemove(&pkt,m_ID,connID);
                m_global_client->send(&pkt);
            }

            accounts = it->accounts;

            m_agent_srvs.erase(it);

            m_io_service.post(boost::bind(&Service::OnCheckState,this));
            break;
        }
    }

    lock.unlock();

    OPacket pkt;
    for (std::set<uint32_t>::const_iterator it = accounts.begin(); it != accounts.end(); ++it)
    {
        clt_pkt::AccountNotify8(&pkt,clt_pkt::ACCOUNT_NOTIFY_STATE,*it,false);
        m_global_client->send(&pkt);

        pkt.Clear();
    }
}

void Service::OnGShard (const std::string &host, const std::string &port,
                      const boost::shared_ptr<IConnection> &conn)
{
    syslog(LOG_INFO,"Game Shard registered - ID = %i",conn->ID());

    boost::unique_lock<boost::mutex> wlock(m_world_mutex);

    Area area;
    for (std::vector<Area>::iterator it = m_world.begin(); it != m_world.end(); ++it)
    {
        if (!it->serverID)
        {
            syslog(LOG_INFO,"Assigning World Area = %i to Game Shard = %i",it->ID,conn->ID());

            it->serverID = conn->ID();
            it->host = host;
            it->port = port;
            area = *it;
            break;
        }
    }

    wlock.unlock();

    if (area.serverID)
    {
        OPacket pkt;
        srv_pkt::AreaConfig(&pkt,&area);
        conn->send(&pkt);

        pkt.Clear();

        srv_pkt::WorldArea(&pkt,&area);
        broadcastAgents(&pkt);

        GShard srv;
        srv.conn = conn;

        boost::lock_guard<boost::mutex> lock(m_gshards_mutex);
        m_gshards.push_back(srv);
    }
    else
    {
        syslog(LOG_INFO,"All world area slots are taken - Closing Game Shard ID = %i",conn->ID());

        conn->stop();
    }
}

void Service::OnGShardState (const uint32_t connID, const uint8_t state)
{
    syslog(LOG_INFO,"Game Shard - ID = %i current state is %s",connID,state ? "open" : "close");

    boost::unique_lock<boost::mutex> lock(m_gshards_mutex);

    for (std::vector<GShard>::iterator it = m_gshards.begin(); it != m_gshards.end(); ++it)
    {
        if (it->conn->ID() == connID)
        {
            it->state = state;

            m_io_service.post(boost::bind(&Service::OnCheckState,this));
            break;
        }
    }
}

void Service::OnRemoveGShard (const uint32_t connID)
{
    syslog(LOG_INFO,"Connection with Game Shard - ID = %i has been terminated.",connID);

    boost::unique_lock<boost::mutex> wlock(m_world_mutex);

    for (std::vector<Area>::iterator it = m_world.begin(); it != m_world.end(); ++it)
    {
        if (it->serverID == connID)
        {
            syslog(LOG_INFO,"Closing Area = %i assigned to Game Shard = %i",it->ID,connID);

            it->serverID = 0;
            it->host.clear();
            it->port.clear();

            break;
        }
    }

    wlock.unlock();

    boost::unique_lock<boost::mutex> lock(m_gshards_mutex);

    for (std::vector<GShard>::iterator it = m_gshards.begin(); it != m_gshards.end(); ++it)
    {
        if (it->conn->ID() == connID)
        {
            m_gshards.erase(it);

            m_io_service.post(boost::bind(&Service::OnCheckState,this));
            break;
        }
    }
}

void Service::OnTicket (const Ticket &ticket)
{
    boost::lock_guard<boost::mutex> lock(m_ticket_mutex);

    m_tickets.insert(std::make_pair(ticket.ID,ticket));
}

void Service::OnCertifyTicket (const uint32_t ticketID, const std::string &usr, const std::string &pw,
                               const std::string &host, const boost::shared_ptr<IConnection> &conn)
{
    boost::unique_lock<boost::mutex> lock(m_ticket_mutex);

    std::map<uint32_t,Ticket>::iterator iter = m_tickets.find(ticketID);

    Ticket ticket;

    if (iter != m_tickets.end())
    {
        ticket = iter->second;
        m_tickets.erase(iter);
    }

    lock.unlock();

    for (size_t i = 0; i < ticket.usr.size(); ++i)
        ticket.usr[i] = tolower(ticket.usr[i]);

    OPacket pkt;

    if (ticket.usr == usr && ticket.pw == pw)
    {
        if (ticket.IP != host)
        {
            syslog(LOG_INFO,"IP Mismatch, Gateway = %s, Agent = %s",ticket.IP.c_str(),host.c_str());
        }

        if (useSlot(ticket.AccountID,ticket.access_lv,ticket.sec_lv))
            srv_pkt::LoginTicket(&pkt,&ticket);
        else
            srv_pkt::LoginTicket(&pkt,srv_pkt::LOGIN_ERROR_USER_LIMIT,ticketID);
    }
    else
    {
        syslog(LOG_INFO,"Invalid ID request, could be desync????, Agentd Values = %i %s %s || Client Values = %i %s %s",
            ticket.ID,ticket.usr.c_str(),ticket.pw.c_str(),ticketID,usr.c_str(),pw.c_str());

        srv_pkt::LoginTicket(&pkt,srv_pkt::LOGIN_ERROR_BAD_CREDENTIALS,ticketID);
    }

    conn->send(&pkt);
}

void Service::OnAccount (const uint32_t connID, const uint32_t accountID)
{
    OPacket pkt;
    clt_pkt::AccountNotify8(&pkt,clt_pkt::ACCOUNT_NOTIFY_STATE,accountID,true);

    boost::lock_guard<boost::mutex> lock(m_agsrv_mutex);

    for (std::vector<Agent>::iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->conn->ID() == connID)
        {
            it->accounts.insert(accountID);
            m_global_client->send(&pkt);
            break;
        }
    }
}

void Service::OnRemoveAccount (const uint32_t connID, const uint32_t accountID)
{
    OPacket pkt;
    clt_pkt::AccountNotify8(&pkt,clt_pkt::ACCOUNT_NOTIFY_STATE,accountID,false);

    boost::lock_guard<boost::mutex> lock(m_agsrv_mutex);

    for (std::vector<Agent>::iterator it = m_agent_srvs.begin(); it != m_agent_srvs.end(); ++it)
    {
        if (it->conn->ID() == connID)
        {
            it->accounts.erase(accountID);
            m_global_client->send(&pkt);
            break;
        }
    }
}
