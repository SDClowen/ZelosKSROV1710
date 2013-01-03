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
#include <auth/packet_srv_authentication.h>
#include <net/client/connection.h>

#include "globald/packet.h"
#include "globald/states.h"
#include "globald/state_handshake.h"
#include "globald/state_global.h"
#include "server/packet.h"
#include "server/server.h"

#define GLOBAL_RECONNECT_TIME 10

enum CLIENT_SERVICES
{
    CLIENT_GLOBALD
};

Service::Service ()
    : m_global(m_io_service),
    m_srv_state(false),
    m_server(new Server(this,m_io_service)),
    m_news_strand(m_io_service),
    m_shard_strand(m_io_service)
{
}

Service::~Service ()
{
}

const std::string& Service::getLastError () const
{
    return m_server->GetLastError();
}

uint8_t Service::getLocale() const
{
    return m_server->Locale();
}

const std::string& Service::ServerListID () const
{
    return m_server->ServerListID();
}

void Service::addShard (const Shard &shard)
{
    m_io_service.post(m_shard_strand.wrap(boost::bind(&Service::OnShardInsert,this,shard)));
}

void Service::updateShard (const uint8_t type, const uint16_t ID, const uint16_t arg)
{
    m_io_service.post(m_shard_strand.wrap(boost::bind(&Service::OnShardUpdate,this,type,ID,arg)));
}

void Service::removeShard (const uint32_t shardID)
{
    m_io_service.post(m_shard_strand.wrap(boost::bind(&Service::OnShardRemove,this,shardID)));
}

void Service::sendShards (const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(m_shard_strand.wrap(boost::bind(&Service::OnShards,this,conn)));
}

void Service::addNews (const Notice& news)
{
    m_io_service.post(m_news_strand.wrap(boost::bind(&Service::OnNewsInsert,this,news)));
}

void Service::removeNews (const uint32_t newsID)
{
    m_io_service.post(m_news_strand.wrap(boost::bind(&Service::OnNewsRemove,this,newsID)));
}

void Service::sendNews (const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(m_news_strand.wrap(boost::bind(&Service::OnNews,this,conn)));
}

void Service::requestTicket (const std::string &usr, const std::string &pw,
                             const uint16_t shardID, const std::string &IP,
                             const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(m_shard_strand.wrap(boost::bind(&Service::OnTicketRequest,this,
                                                      usr,pw,shardID,IP,conn)));
}

void Service::sendGlobal (OPacket *pkt)
{
    m_global.conn->send(pkt);
}

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServerRun,this));
}

boost::shared_ptr<Server> Service::getServer () const
{
    return m_server;
}

boost::shared_ptr<clt::Connection> Service::getClient () const
{
    return m_global.conn;
}

bool Service::initializeGlobal ()
{
    m_global.conn.reset(new clt::Connection(CLIENT_GLOBALD,this,m_io_service));

    if (!m_global.conn)
        return false;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_global.conn->setAuthentication(auth);

    boost::shared_ptr<ConnectionState<IConnection> > state;

    state.reset(new clt::StateHandshake("ManagerServer","GatewayServer",m_global.conn));

    m_global.conn->addState(state,clt::STATE_HANDSHAKE);

    state.reset(new clt::StateGlobal(this,m_global.conn));

    m_global.conn->addState(state,clt::STATE_GLOBAL);

    m_global.conn->setState(clt::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeServer ()
{
    if (!m_server)
    {
        return false;
    }

    if (!m_server->initialize())
    {
        return false;
    }

    return true;
}

bool Service::OnInitialize ()
{
    if (!initializeServer())
        return false;

    if (!initializeGlobal())
    {
        return false;
    }

    m_global.conn->connect(m_global.host,m_global.port);

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    if (m_server)
        m_server->configure(entries);

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "ClientHost")
            m_global.host = it->second;
        else if (it->first == "ClientPort")
            m_global.port = it->second;
    }
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    assert(type == CLIENT_GLOBALD);

    if (__sync_bool_compare_and_swap(&m_global.state,!state,state))
    {
        syslog(LOG_INFO,"Connection with global client %s.",state ? "established" : "terminated");

        if (!state)
        {
            m_global.timer.expires_from_now(boost::posix_time::seconds(GLOBAL_RECONNECT_TIME));
            m_global.timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    assert(type == CLIENT_GLOBALD);

    syslog(LOG_INFO,"Global Client, %s",error.message().c_str());

    m_global.timer.expires_from_now(boost::posix_time::seconds(GLOBAL_RECONNECT_TIME));
    m_global.timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                          boost::asio::placeholders::error));
}

void Service::OnRelease ()
{
    m_server->Stop();
}

void Service::OnServerRun ()
{
    if (__sync_bool_compare_and_swap(&m_srv_state,false,true))
    {
        syslog(LOG_INFO,"Launching server.");

        m_server->Execute();
    }
}

void Service::OnGlobalReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to global server.");

        if (initializeGlobal())
        {
            m_global.conn->connect(m_global.host,m_global.port);
        }
        else
        {
            m_global.timer.expires_from_now(boost::posix_time::seconds(GLOBAL_RECONNECT_TIME));
            m_global.timer.async_wait(boost::bind(&Service::OnGlobalReconnect,this,
                                                  boost::asio::placeholders::error));
        }
    }
}

void Service::OnNews (const boost::shared_ptr<IConnection> &conn)
{
    OPacket pkt;
    srv_pkt::WriteVersionG(&pkt);
    conn->send(&pkt);

    pkt.Clear();

    srv_pkt::WriteNews(&pkt,m_news);
    conn->send(&pkt);
}

void Service::OnNewsInsert (const Notice &news)
{
    m_news.insert(std::make_pair(news.id,news));
}

void Service::OnNewsRemove (const uint32_t newsID)
{
    m_news.erase(newsID);
}

void Service::OnShards (const boost::shared_ptr<IConnection> &conn)
{
    bool curState = __sync_fetch_and_add(&m_global.state,0);

    OPacket pkt;
    srv_pkt::WriteShards(&pkt,ServerListID(),getLocale(),m_shards,curState);
    conn->send(&pkt);
}

void Service::OnShardInsert (const Shard &shard)
{
    m_shards.insert(std::make_pair(shard.id,shard));
}

void Service::OnShardUpdate (const uint8_t type, const uint16_t ID, const uint16_t arg)
{
    std::map<uint16_t,Shard>::iterator i = m_shards.find(ID);

    if (i != m_shards.end())
    {
        switch(type)
        {
        case SHARD_NOTIFY_USR_CNT:
            i->second.usr_cnt = arg;
            break;
        case SHARD_NOTIFY_USR_MAX:
            i->second.usr_max = arg;
            break;
        case SHARD_NOTIFY_STATE:
            i->second.state = arg;

            if (!i->second.state)
                i->second.usr_cnt = 0;
            break;
        }
    }
}

void Service::OnShardRemove (const uint32_t shardID)
{
    m_shards.erase(shardID);
}

void Service::OnTicketRequest (const std::string &usr, const std::string &pw,
                               const uint16_t shardID, const std::string &IP,
                               const boost::shared_ptr<IConnection> &conn)
{
    OPacket pkt;
    std::map<uint16_t,Shard>::const_iterator iter = m_shards.find(shardID);

    if (__sync_fetch_and_add(&m_global.state,0) &&
        iter != m_shards.end() && iter->second.state)
    {
        clt_pkt::WriteQueue(&pkt,conn->ID(),usr,pw,shardID,IP);
        m_global.conn->send(&pkt);
    }
    else
    {
        srv_pkt::C5(&pkt);
        conn->send(&pkt);
    }
}
