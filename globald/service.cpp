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

#include <fstream>

#include <boost/tuple/tuple.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include <auth/authentication_sro.h>
#include <db/pool.h>
#include <net/client/connection.h>

#include "certd/states.h"
#include "certd/state_certify.h"
#include "certd/state_handshake.h"
#include "packet/packet_manager_shard.h"
#include "query/query_account.h"
#include "query/query_manager.h"
#include "query/query_news.h"
#include "query/query_shard.h"
#include "server/server.h"
#include "server/packet_gateway.h"
#include "shard_config.h"
#include "strutil.h"

#define MAX_ATTEMPTS 3
#define CERT_RECONNECT_TIME 10

enum CLIENT_TYPE
{
    CLIENT_CERTD
};

Service::Service ()
    : m_srv_state(false),
    m_server(new Server(this,m_io_service)),
    m_cert_state(false),
    m_cert_timer(m_io_service),
    m_ban_timer(m_io_service),
    m_attempt_timer(m_io_service),
    m_ticket_timer(m_io_service)
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

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServer,this));
}

void Service::addGateway (const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnGateway,this,conn));
}

void Service::broadcastGateway (OPacket *pkt)
{
    boost::lock_guard<boost::mutex> lock(m_gw_mutex);

    for (std::vector<boost::shared_ptr<IConnection> >::iterator i = m_gws.begin(); i != m_gws.end(); ++i)
        (*i)->send(pkt);
}

void Service::removeGateway (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveGateway,this,connID));
}

void Service::addShard (const std::string &name, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnShard,this,name,conn));
}

uint16_t Service::setShardConnID (const std::string &name, const uint32_t connID)
{
    uint16_t ID = 0;
    boost::mutex::scoped_lock lock(m_global_mutex);

    std::map<uint16_t,Shard>::iterator it;

    for (it = m_shards.begin(); it != m_shards.end(); ++it)
    {
        if (it->second.name == name)
        {
            it->second.connID = connID;
            ID = it->second.id;
            break;
        }
    }

    return ID;
}

uint16_t Service::getShardID (const uint32_t connID) const
{
    uint16_t shardID = 0;
    boost::mutex::scoped_lock lock(m_global_mutex);

    for (std::map<uint16_t,Shard>::const_iterator it = m_shards.begin(); it != m_shards.end(); ++it)
    {
        if (it->second.connID == connID)
        {
            shardID = it->second.id;
            break;
        }
    }

    return shardID;
}

void Service::updateShard (const uint8_t type, const uint16_t ID, const uint16_t arg)
{
    m_io_service.post(boost::bind(&Service::OnNotifyShard,this,type,ID,arg));
}

void Service::removeShard (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveShard,this,connID));
}

void Service::addAgent (const uint16_t shardID, const uint32_t agentID,
                        const std::string &host, const std::string &port)
{
    m_io_service.post(boost::bind(&Service::OnAgent,this,shardID,agentID,host,port));
}

void Service::removeAgent (const uint16_t shardID, const uint32_t agentID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveAgent,this,shardID,agentID));
}

void Service::addAccount (const uint32_t accountID, const uint32_t shardConnID)
{
    m_io_service.post(boost::bind(&Service::OnAccount,this,accountID,shardConnID));
}

void Service::setAccountBan (const uint32_t accountID, const int years)
{
    m_io_service.post(boost::bind(&Service::OnAccountBan,this,accountID,years));
}

void Service::setAccountSilk (const uint32_t accountID, const uint32_t silk)
{
    m_io_service.post(boost::bind(&Service::OnAccountSilk,this,accountID,silk));
}

void Service::removeAccount (const uint32_t accountID, const uint32_t shardConnID)
{
    m_io_service.post(boost::bind(&Service::OnRemoveAccount,this,accountID,shardConnID));
}

void Service::addTicket (const Ticket &ticket)
{
    m_io_service.post(boost::bind(&Service::OnTicket,this,ticket));
}

void Service::addIPBan (const std::string &IP)
{
    /// BAN THE IP FOR 10 MINS
    boost::lock_guard<boost::mutex> lock(m_ip_mutex);

    m_banned_ips.insert(std::make_pair(IP,10));
}

void Service::removeIPBan (const std::string &IP)
{
    boost::lock_guard<boost::mutex> lock(m_ip_mutex);

    m_banned_ips.erase(IP);
}

bool Service::isIPBanned (const std::string &IP) const
{
    boost::lock_guard<boost::mutex> lock(m_ip_mutex);

    return m_banned_ips.find(IP) != m_banned_ips.end();
}

uint8_t Service::getRemainingBanTime (const std::string &IP) const
{
    uint8_t time = 0;

    boost::mutex::scoped_lock lock(m_ip_mutex);
    std::map<std::string,uint8_t>::const_iterator it = m_banned_ips.find(IP);

    if (it != m_banned_ips.end())
        time = it->second;

    return time;
}

uint8_t Service::addLoginAttempt (const std::string &IP)
{
    enum { MAX_LOGIN_ATTEMPTS = 3 };

    uint8_t count = 0;

    boost::mutex::scoped_lock lock(m_attempt_mutex);
    std::map<std::string,uint8_t>::iterator it = m_attempts.find(IP);

    if (it != m_attempts.end())
        count = it->second;

    ++count;

    if (count == MAX_LOGIN_ATTEMPTS)
    {
        addIPBan(IP);

        m_attempts.erase(it);
    }
    else
        m_attempts[IP] = count;

    return count;
}

uint32_t Service::generateTicket (const uint32_t accountID, const uint16_t shardID)
{
    boost::lock_guard<boost::mutex> lock(m_ticket_mutex);

    TIndexAcc &idx = boost::multi_index::get<LoginTicket::tagAcc>(m_tickets);

    if (idx.find(accountID) != idx.end())
        return 0;

    LoginTicket ticket(rand(),accountID,shardID);
    ticket.ID = rand();

    while(!m_tickets.insert(ticket).second)
        ticket.ID = rand();

    return ticket.ID;
}

void Service::removeTicket (const uint32_t ticketID)
{
    boost::mutex::scoped_lock lock(m_ticket_mutex);

    m_tickets.erase(ticketID);
}

bool Service::initializeCert ()
{
    m_cert_client.reset(new clt::Connection(CLIENT_CERTD,this,m_io_service));

    if (!m_cert_client)
        return false;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_cert_client->setAuthentication(auth);

    boost::shared_ptr<clt::Connection::State> state;

    state.reset(new clt::StateCertify(this,m_cert_client));

    if (!state)
        return false;

    m_cert_client->addState(state,clt::STATE_CERTIFY);

    state.reset(new clt::StateHandshake(this,"CertifyServer","ManagerServer",m_cert_client));

    if (!state)
        return false;

    m_cert_client->addState(state,clt::STATE_HANDSHAKE);

    m_cert_client->setState(clt::STATE_HANDSHAKE);

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

bool Service::OnInitialize()
{
    try
    {
        m_dbPool.reset(new db::Pool(m_db_name.c_str(),m_db_ip.c_str(),
                                    m_db_user.c_str(),m_db_password.c_str()));

        if (!m_dbPool)
        {
            m_error = "Failed to initialize database pool";
            return false;
        }

        db::ScopedConnection dbConn(m_dbPool);

        std::vector<Notice> news = db::news::Select(dbConn);

        for (size_t i = 0; i < news.size(); ++i)
            m_news.insert(std::make_pair(news[i].id,news[i]));

        db::account::writeStateClear(dbConn);

        db::shard::writeStateClear(dbConn);

        m_shards = db::shard::readShards(dbConn);
    }
    catch (const mysqlpp::ConnectionFailed &error)
    {
        m_error = "Failed to establish a connection to the database.";
        return false;
    }
    catch (const mysqlpp::BadQuery &error)
    {
        m_error = error.what();
        return false;
    }

    if (!initializeServer())
        return false;

    if (!initializeCert())
    {
        m_error = "Failed to initialize certify client.";
        return false;
    }

    if (m_config_path.empty())
    {
        return false;
    }

    if (!readWorld())
    {
        return false;
    }

    m_cert_client->connect(m_host,m_port);

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    if (m_server)
        m_server->configure(entries);

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "Locale")
            m_locale = atoi(it->second.c_str());
        else if (it->first == "Version")
            m_version = atoi(it->second.c_str());
        else if (it->first == "CertifyHost")
            m_host = it->second;
        else if (it->first == "CertifyPort")
            m_port = it->second;
        else if (it->first == "ConfigPath")
            m_config_path = it->second;
        else if (it->first == "DatabaseIP")
            m_db_ip = it->second;
        else if (it->first == "DatabaseName")
            m_db_name = it->second;
        else if (it->first == "DatabaseUser")
            m_db_user = it->second;
        else if (it->first == "DatabasePassword")
            m_db_password = it->second;
    }
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    if (__sync_bool_compare_and_swap(&m_cert_state,!state,state))
    {
        if (!state)
        {
            m_cert_timer.expires_from_now(boost::posix_time::seconds(CERT_RECONNECT_TIME));
            m_cert_timer.async_wait(boost::bind(&Service::OnCertReconnect,
                                                this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    syslog(LOG_INFO,"Certify Client, %s",error.message().c_str());

    m_cert_timer.expires_from_now(boost::posix_time::seconds(CERT_RECONNECT_TIME));
    m_cert_timer.async_wait(boost::bind(&Service::OnCertReconnect,
                                        this,
                                        boost::asio::placeholders::error));
}

void Service::OnRelease ()
{
    m_server->Stop();

    db::ScopedConnection dbConn(m_dbPool);

    db::account::writeStateClear(dbConn);

    db::shard::writeStateClear(dbConn);
}

bool Service::readWorld ()
{
    std::string filename = m_config_path + "/world/world_areas.txt";

    std::ifstream file(filename.c_str());

    if (!file)
    {
        m_error = "Missing world_areas.txt file\n. Check your config file, ConfigPath = " + filename;
        return false;
    }

    std::string line;
    while (!file.eof())
    {
        std::getline(file,line);

        if (!line.empty() && *(line.begin()) != '#')
        {
            std::vector<std::string> entries = split(line,'\t');

            if (atoi(entries[0].c_str()) == 1)
            {
                Area area;
                area.ID = atoi(entries[1].c_str());
                area.Instance = atoi(entries[2].c_str());
                area.Name = entries[3];

                m_world.insert(std::make_pair(area.ID,area));
            }
        }
    }

    file.close();

    filename = m_config_path + "/world/world_zones.txt";

    file.open(filename.c_str());

    if (!file)
    {
        m_error = "Missing world_zones.txt file.\n Check your config file, ConfigPath = " + filename;
        return false;
    }

    while (!file.eof())
    {
        std::getline(file,line);

        if (!line.empty() && *(line.begin()) != '#')
        {
            std::vector<std::string> entries = split(line,'\t');

            if (atoi(entries[0].c_str()) == 1)
            {
                int areaID = atoi(entries[1].c_str());
                int zoneID = atoi(entries[2].c_str());

                std::map<int,Area>::iterator iter = m_world.find(areaID);

                if (iter != m_world.end())
                    iter->second.Zones.push_back(zoneID);
            }
        }
    }

    file.close();

    return true;
}

void Service::OnCertReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to certify server.");

        if (initializeCert())
            m_cert_client->connect(m_host,m_port);
        else
        {
            m_cert_timer.expires_from_now(boost::posix_time::seconds(CERT_RECONNECT_TIME));
            m_cert_timer.async_wait(boost::bind(&Service::OnCertReconnect,
                                                this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnServer ()
{
    if (__sync_bool_compare_and_swap(&m_srv_state,false,true))
    {
        syslog(LOG_INFO,"Launching server.");

        m_server->Execute();

        m_ban_timer.expires_from_now(boost::posix_time::minutes(1));
        m_ban_timer.async_wait(boost::bind(&Service::OnBanTimeout,this,
                                           boost::asio::placeholders::error));

        m_attempt_timer.expires_from_now(boost::posix_time::minutes(1));
        m_attempt_timer.async_wait(boost::bind(&Service::OnAttemptTimeout,this,
                                               boost::asio::placeholders::error));

        m_ticket_timer.expires_from_now(boost::posix_time::minutes(1));
        m_ticket_timer.async_wait(boost::bind(&Service::OnTicketTimeout,this,
                                              boost::asio::placeholders::error()));
    }
}

void Service::OnGateway (const boost::shared_ptr<IConnection> &conn)
{
    boost::lock_guard<boost::mutex> glock(m_gw_mutex);

    m_gws.push_back(conn);

    boost::lock_guard<boost::mutex> lock(m_global_mutex);

    OPacket pkt;
    srv_pkt::WriteConfig(&pkt,m_locale,m_version,m_shards,m_news);
    conn->send(&pkt);
}

void Service::OnRemoveGateway (const uint32_t connID)
{
    typedef std::vector<boost::shared_ptr<IConnection> >::iterator gw_iterator;

    boost::lock_guard<boost::mutex> glock(m_gw_mutex);

    for (gw_iterator iter = m_gws.begin(); iter != m_gws.end(); ++iter)
    {
        if ((*iter)->ID() == connID)
        {
            m_gws.erase(iter);
            break;
        }
    }
}

void Service::OnShard (const std::string &name, const boost::shared_ptr<IConnection> &conn)
{
    ShardConfig config;

    config.ID = setShardConnID(name,conn->ID());
    config.version = m_version;
    config.locale = m_locale;

    if (config.ID)
    {
        for (std::map<int,Area>::const_iterator iter = m_world.begin(); iter != m_world.end(); ++iter)
            config.world.push_back(iter->second);

        db::ScopedConnection dbConn(m_dbPool);

        /// READ SERVER RESPECTIVE CONFIGURATION FILE
        ServConfig cfg = db::manager::readConfig(dbConn,name);

        config.filter = db::manager::readNameFilter(dbConn);

        config.races = cfg.ch_char | (cfg.eu_char << 1);

        /// SEND THE SERVER CURRENT ID AND ANY OTHER NECESARY CONFIGURATION INFORMATION NEEDED
        OPacket pkt;
        srv_pkt::Config(&pkt,&config);
        conn->send(&pkt);
    }
}

void Service::OnNotifyShard (const uint8_t type, const uint16_t ID, const uint16_t arg)
{
    boost::mutex::scoped_lock lock(m_global_mutex);
    shard_iterator i = m_shards.find(ID);

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
            {
                i->second.state = arg;

                if (!i->second.state)
                    i->second.usr_cnt = 0;

                db::ScopedConnection dbConn(m_dbPool);

                db::shard::writeState(dbConn,ID,arg);
            }
            break;
        default:
            break;
        }

        OPacket pkt;
        srv_pkt::WriteShard(&pkt,(SHARD_NOTIFY)type,ID,arg);
        broadcastGateway(&pkt);
    }
}

void Service::OnRemoveShard (const uint32_t connID)
{
    uint32_t shardID = 0;
    boost::mutex::scoped_lock glock(m_global_mutex);

    for (std::map<uint16_t,Shard>::iterator sit = m_shards.begin(); sit != m_shards.end(); ++sit)
    {
        if (sit->second.connID == connID)
        {
            sit->second.connID = 0;
            sit->second.state = 0;
            sit->second.usr_cnt = 0;

            shardID = sit->second.id;
            break;
        }
    }

    glock.unlock();

    if (shardID)
    {
        db::ScopedConnection dbConn(m_dbPool);

        typedef std::multimap<uint16_t,uint32_t>::iterator usr_shard_iter;

        std::pair<usr_shard_iter,usr_shard_iter> acc_range = m_active_accounts.equal_range(shardID);

        for (usr_shard_iter uit = acc_range.first; uit != acc_range.second; ++uit)
            db::account::writeState(dbConn,uit->second,false);

        db::shard::writeState(dbConn,shardID,0);

        OPacket pkt;
        srv_pkt::WriteShard(&pkt,SHARD_NOTIFY_STATE,shardID,0);
        broadcastGateway(&pkt);

        boost::lock_guard<boost::mutex> tlock(m_ticket_mutex);
        TIndexShard &idx = boost::multi_index::get<LoginTicket::tagShard>(m_tickets);

        idx.erase(connID);
    }
}

void Service::OnAgent (const uint16_t shardID, const uint32_t agentID, const std::string &host,
              const std::string &port)
{
    syslog(LOG_INFO,"New agent server %i, available for shard %i",agentID,shardID);

    boost::unique_lock<boost::mutex> lock(m_global_mutex);

    std::map<uint16_t,Shard>::iterator iter = m_shards.find(shardID);

    if (iter != m_shards.end())
    {
        Agent agent;
        agent.ID = agentID;
        agent.host = host;
        agent.port = atoi(port.c_str());

        iter->second.agents.push_back(agent);
    }
}

void Service::OnRemoveAgent (const uint16_t shardID, const uint32_t agentID)
{
    syslog(LOG_INFO,"Removing agent server %i for shard %i",agentID,shardID);

    boost::unique_lock<boost::mutex> lock(m_global_mutex);

    std::map<uint16_t,Shard>::iterator iter = m_shards.find(shardID);

    if (iter != m_shards.end())
    {
        std::vector<Agent>::iterator aiter;
        for ( aiter = iter->second.agents.begin(); aiter != iter->second.agents.end(); ++aiter)
        {
            if (aiter->ID == agentID)
            {
                iter->second.agents.erase(aiter);
                break;
            }
        }
    }
}

void Service::OnAccount (const uint32_t accountID, const uint32_t shardConnID)
{
    uint16_t shardID = getShardID(shardConnID);

    if (shardID)
    {
        db::ScopedConnection dbConn(m_dbPool);

        db::account::writeState(dbConn,accountID,true);

        {
            boost::lock_guard<boost::mutex> alock(m_ticket_mutex);

            TIndexAcc &idx = boost::multi_index::get<LoginTicket::tagAcc>(m_tickets);

            idx.erase(accountID);
        }

        boost::lock_guard<boost::mutex> lock(m_account_mutex);

        m_active_accounts.insert(std::make_pair(shardID,accountID));
    }
}

void Service::OnAccountBan (const uint32_t accountID, const int years)
{
    db::ScopedConnection dbConn(m_dbPool);

    db::account::issueBan(dbConn,accountID,years);
}

void Service::OnAccountSilk (const uint32_t accountID, const uint32_t silk)
{
    db::ScopedConnection dbConn(m_dbPool);

    db::account::writeSilk(dbConn,accountID,silk);
}

void Service::OnRemoveAccount (const uint32_t accountID, const uint32_t shardConnID)
{
    uint16_t shardID = getShardID(shardConnID);

    if (shardID)
    {
        typedef std::multimap<uint16_t,uint32_t>::iterator usr_shard_iter;

        boost::mutex::scoped_lock lock(m_account_mutex);

        std::pair<usr_shard_iter,usr_shard_iter> acc_range = m_active_accounts.equal_range(shardID);

        for (usr_shard_iter it = acc_range.first; it != acc_range.second; ++it)
        {
            if (it->second == accountID)
            {
                m_active_accounts.erase(it);
                break;
            }
        }
    }

    db::ScopedConnection dbConn(m_dbPool);

    db::account::writeState(dbConn,accountID,false);
}

void Service::OnTicket (const Ticket &ticket)
{
    boost::mutex::scoped_lock slock(m_global_mutex);

    std::map<uint16_t,Shard>::iterator i = m_shards.find(ticket.shardID);

    OPacket pkt;

    if (i == m_shards.end() || !i->second.state || !i->second.connID || i->second.agents.empty())
    {
        srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_C5);
        ticket.conn->send(&pkt);

        return;
    }

    Shard shard = i->second;

    slock.unlock();

    if (isIPBanned(ticket.IP))
    {
        using namespace boost::gregorian;
        using namespace boost::posix_time;

        uint8_t min = getRemainingBanTime(ticket.IP);

        if (min)
        {
            boost::gregorian::date date(boost::gregorian::day_clock::local_day());

            boost::posix_time::ptime curtime = boost::posix_time::second_clock::local_time();
            boost::posix_time::ptime nexttime = curtime + boost::posix_time::minutes(10);

            if (curtime > nexttime)
                date += boost::gregorian::days(1);

            tm pm_tm = to_tm(nexttime);

            std::string msg = "Your account is suspended/blocked. Expiration date " +
                              to_simple_string(date) + " " + to_simple_string(nexttime);

            srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_BLOCKED_MSG,msg,date.year(),
                date.month(),date.day(),pm_tm.tm_hour,pm_tm.tm_min);
            ticket.conn->send(&pkt);

            return;
        }
    }

    db::ScopedConnection dbConn(m_dbPool);

    db::account::LOGIN_RET ret = db::account::Authenticate(dbConn,ticket.usr,ticket.pw,ticket.IP);

    switch(ret)
    {
    case db::account::LOGIN_ACCEPTED:
        {
            /// Obtain account access level, security level and current silk.

            AccountProperty property = db::account::readAccount(dbConn,ticket.usr);

            uint32_t queueID = generateTicket(property.ID,shard.connID);

            if (queueID)
            {
                srv_pkt::LoginQueue(&pkt,queueID,ticket.usr,ticket.pw,ticket.IP,property);
                m_server->send(&pkt,shard.connID);

                pkt.Clear();

                size_t agentIdx = queueID % shard.agents.size();

                srv_pkt::WriteLogin(&pkt,ticket.ID,queueID,shard.agents[agentIdx]);
                ticket.conn->send(&pkt);
            }
            else
            {
                srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_USER_ALREDY_ON);
                ticket.conn->send(&pkt);
            }
        }
        break;
    case db::account::LOGIN_BANNED:
        {
            mysqlpp::DateTime dt = db::account::readBanExpirationDate(dbConn,ticket.usr);

            std::string msg = "Your account is suspended/blocked. Expiration date " + dt.str();

            srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_BLOCKED_MSG,msg,dt.year(),
                dt.month(),dt.day(),dt.hour(),dt.minute());
            ticket.conn->send(&pkt);
        }
        break;
    case db::account::LOGIN_ALREDY_LOGGED:
        {
            srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_USER_ALREDY_ON);
            ticket.conn->send(&pkt);
        }
        break;
    case db::account::LOGIN_REJECTED:
        {
            int attempts = addLoginAttempt(ticket.IP);

            srv_pkt::WriteLogin(&pkt,ticket.ID,srv_pkt::LOGIN_ERROR_WRONG_PW,attempts,MAX_ATTEMPTS);
            ticket.conn->send(&pkt);
        }
        break;
    default:
        break;
    }
}

void Service::OnBanTimeout (const boost::system::error_code &error)
{
    if (!error)
    {
        std::vector<std::string> ips;
        boost::mutex::scoped_lock lock(m_ip_mutex);

        for (std::map<std::string,uint8_t>::iterator it= m_banned_ips.begin(); it != m_banned_ips.end(); ++it)
        {
            --it->second;

            if (!it->second)
                ips.push_back(it->first);
        }

        lock.unlock();

        std::for_each(ips.begin(),ips.end(),boost::bind(&Service::removeIPBan,this,_1));

        m_ban_timer.expires_from_now(boost::posix_time::minutes(1));
        m_ban_timer.async_wait(boost::bind(&Service::OnBanTimeout,this,boost::asio::placeholders::error));
    }
}

void Service::OnAttemptTimeout (const boost::system::error_code &error)
{
    if (!error)
    {
        boost::mutex::scoped_lock lock(m_attempt_mutex);
        for (std::map<std::string,uint8_t>::iterator it = m_attempts.begin(); it != m_attempts.end();)
        {
            --it->second;

            if (!it->second)
                m_attempts.erase(it++);
            else
                ++it;
        }

        lock.unlock();

        m_attempt_timer.expires_from_now(boost::posix_time::minutes(1));
        m_attempt_timer.async_wait(boost::bind(&Service::OnAttemptTimeout,this,
                                               boost::asio::placeholders::error));
    }
}

void Service::OnTicketTimeout (const boost::system::error_code &error)
{
    if (!error)
    {
        std::vector<uint32_t> tickets;

        boost::unique_lock<boost::mutex> lock(m_ticket_mutex);

        LoginTicket ticket;
        for (ticket_set::iterator it = m_tickets.begin(); it != m_tickets.end();)
        {
            ticket = *it;
            --ticket.expireTime;

            if (!ticket.expireTime)
            {
                tickets.push_back(ticket.ID);
                m_tickets.erase(it++);
            }
            else
                ++it;
        }

        lock.unlock();

        m_ticket_timer.expires_from_now(boost::posix_time::minutes(1));
        m_ticket_timer.async_wait(boost::bind(&Service::OnTicketTimeout,this,
                                              boost::asio::placeholders::error()));

        /// TODO: NOTIFY SHARDS THE TICKET HAS EXPIRED!!
    }
}
