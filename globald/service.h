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

#ifndef __ZELOS_GLOBALD_SERVICE_H__
#define __ZELOS_GLOBALD_SERVICE_H__

#include <set>
#include <boost/bimap.hpp>

#include <net/service.h>

#include "area.h"
#include "login_ticket.h"
#include "news.h"
#include "shard.h"
#include "ticket.h"

class IConnection;
class Server;
class OPacket;

namespace clt
{
    class Connection;
}

namespace db
{
    class Pool;
}

class Service : public IService
{
public:

    typedef std::map<uint16_t,Shard> ::iterator shard_iterator;
    typedef std::map<uint16_t,Shard>::const_iterator shard_const_iterator;

    Service();

    virtual ~Service ();

    const std::string& getLastError () const;

    void runServer ();

    void addGateway (const boost::shared_ptr<IConnection> &conn);

    void broadcastGateway (OPacket *pkt);

    void removeGateway (const uint32_t connID);

    void addShard (const std::string &name, const boost::shared_ptr<IConnection> &conn);

    uint16_t setShardConnID (const std::string &name, uint32_t connID);

    uint16_t getShardID (const uint32_t connID) const;

    void updateShard (const uint8_t type, const uint16_t ID, const uint16_t arg);

    void removeShard (const uint32_t connID);

    void addAgent (const uint16_t shardID, const uint32_t agentID, const std::string &host,
                   const std::string &port);

    void removeAgent (const uint16_t shardID, const uint32_t agentID);

    void addAccount (const uint32_t accountID, const uint32_t shardConnID);

    void setAccountBan (const uint32_t accountID, const int years);

    void setAccountSilk (const uint32_t accountID, const uint32_t silk);

    void removeAccount (const uint32_t accountID, const uint32_t shardConnID);

    void addTicket (const Ticket &ticket);

    /**
     *
     *	@brief Add an IP to the banned list.
     *	@param IP: Desired IP to ban.
     *
     **/

    void addIPBan (const std::string &IP);

    /**
     *
     *	@brief Remove an IP from the banned list.
     *	@param IP: Desired IP to remove from the list.
     *
     **/

    void removeIPBan (const std::string &IP);

    /**
     *
     *	@brief Check if an IP has been blocked.
     *	@param IP: IP to check for.
     *	@return True if the IP has been banned, false otherwise.
     *
     **/

    bool isIPBanned (const std::string &IP) const;

    /**
     *
     *	@brief Get IP remaining time[mins] to lift the ban.
     *	@param IP: Client IP to inspect.
     *	@return Minutes remaining.
     *
     **/

    uint8_t getRemainingBanTime (const std::string &IP) const;

    uint8_t addLoginAttempt (const std::string &IP);

    /**
     *
     *	@brief Create a new random login ticket (unique) and add it to the pending list.
     *	@return login ticket number.
     *
     **/

    uint32_t generateTicket (const uint32_t accountID, const uint16_t shardID);

    /**
     *
     *	@brief Remove a login ticket from the pending list.
     *	@param ticketID: Ticket to remove from the list.
     *
     **/

    void removeTicket (const uint32_t ticketID);

private:

    bool initializeCert ();

    bool initializeServer ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

private:

    bool readWorld ();

private:

    void OnCertReconnect (const boost::system::error_code &error);

    void OnServer ();

    void OnGateway (const boost::shared_ptr<IConnection> &conn);

    void OnRemoveGateway (const uint32_t connID);

    void OnShard (const std::string &name, const boost::shared_ptr<IConnection> &conn);

    void OnNotifyShard (const uint8_t type, const uint16_t ID, const uint16_t arg);

    void OnRemoveShard (const uint32_t connID);

    void OnAgent (const uint16_t shardID, const uint32_t agentID, const std::string &host,
                  const std::string &port);

    void OnRemoveAgent (const uint16_t shardID, const uint32_t agentID);

    void OnAccount (const uint32_t accountID, const uint32_t shardConnID);

    void OnAccountBan (const uint32_t accountID, const int years);

    void OnAccountSilk (const uint32_t accountID, const uint32_t silk);

    void OnRemoveAccount (const uint32_t accountID, const uint32_t shardConnID);

    void OnTicket (const Ticket &ticket);

    void OnBanTimeout (const boost::system::error_code &error);

    void OnAttemptTimeout (const boost::system::error_code &error);

    void OnTicketTimeout (const boost::system::error_code &error);

private:

    std::string m_error;

    int m_locale;
    int m_version;

    bool m_srv_state;
    boost::shared_ptr<Server> m_server;

    bool m_cert_state;
    std::string m_host;
    std::string m_port;
    boost::asio::deadline_timer m_cert_timer;
    boost::shared_ptr<clt::Connection> m_cert_client;

    std::string m_config_path;

    std::map<int,Area> m_world;

    boost::shared_ptr<db::Pool> m_dbPool;

    std::string m_db_ip;
    std::string m_db_name;
    std::string m_db_user;
    std::string m_db_password;

    boost::mutex m_ticket_mutex;
    ticket_set m_tickets;

    mutable boost::mutex m_global_mutex;
    std::map<uint32_t,Notice> m_news;
    std::map<uint16_t,Shard> m_shards;

    boost::mutex m_attempt_mutex;
    std::map<std::string,uint8_t> m_attempts;		/// NUMBER OF ATTEMPS DONE BY IP DECREASE BY 1 EVERY 15 MIN

    mutable boost::mutex m_ip_mutex;
    std::map<std::string,uint8_t> m_banned_ips;

    boost::mutex m_account_mutex;
    std::multimap<uint16_t,uint32_t> m_active_accounts;

    boost::mutex m_gw_mutex;
    std::vector<boost::shared_ptr<IConnection> > m_gws;

    boost::asio::deadline_timer m_ban_timer;
    boost::asio::deadline_timer m_attempt_timer;
    boost::asio::deadline_timer m_ticket_timer;
};

#endif // __ZELOS_GLOBALD_SERVICE_H__
