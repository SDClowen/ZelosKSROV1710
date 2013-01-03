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

#ifndef __ZELOS_SHARDD_SERVICE_H__
#define __ZELOS_SHARDD_SERVICE_H__

#include <net/service.h>

#include "agent.h"
#include "area.h"
#include "gshard.h"
#include "ticket.h"

namespace clt
{
    class Connection;
}

class IConnection;
class OPacket;
class Server;

class Service : public IService
{
public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    void runGlobal ();

    void runServer ();

    void setShardID (const uint16_t ID);

    void setLocale (const uint8_t locale);

    void setVersion (const uint32_t version);

    void setRace (const uint8_t race);

    void setFilter (const std::string &chFilter, const std::vector<std::string> &strFilter);

    /**
     *
     *	@brief Insert a new available agent server to the list.
     *	@param host: Host address.
     *	@param port: Listening port.
     *	@param conn: Agent server connection.
     *
     **/

    void addAgent (const std::string &host, const std::string &port,
                   const boost::shared_ptr<IConnection> &conn);

    void updateAgent (const uint32_t connID, const uint8_t state);

    void broadcastAgents (OPacket *pkt);

    void removeAgent (const uint32_t connID);

    /**
     *
     *	@brief Insert a new available game shard to the list.
     *	@param host: Host address.
     *	@param port: Listening port.
     *	@param conn: Game shard connection.
     *
     **/

    void addGShard (const std::string &host, const std::string &port,
                  const boost::shared_ptr<IConnection> &conn);

    void setGShardState (const uint32_t connID, const uint8_t state);

    void broadcastGShards (OPacket *pkt);

    void removeGShard (const uint32_t connID);

    /**
     *
     *	@brief Set shard world distribution.
     *	@param world: World distribution sorted by area ID.
     *
     **/

    void setWorld (const std::vector<Area> &world);

    /**
     *
     *	@brief Add a new area into the world.
     *
     *	@param area: Area to add to the world.
     *
     **/

    void addWorldArea (const Area &area);

    /**
     *
     *	@brief Remove an area from the world, if the area is currently active it sends a close
     *	notification.
     *
     *	@param areaID: Area ID to remove from the world.
     *
     **/

    void removeWorldArea (const int areaID);

    void addTicket (const Ticket &ticket);

    /**
     *
     *	@brief Certify if a login ticket credentials are valid.
     *	@param connID: Agent server connection ID.
     *	@param ticketID: Ticket reference ID.
     *	@param usr: Username.
     *	@param pw: Username password.
     *	@param host: IP where the ticket originated.
     *
     **/

    void certifyTicket (const uint32_t ticketID, const std::string &usr, const std::string &pw,
                        const std::string &host, const boost::shared_ptr<IConnection> &conn);

    void addAccount (const uint32_t connID, const uint32_t accountID);

    void removeAccount (const uint32_t connID, const uint32_t accountID);

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

    bool useSlot (const uint32_t accountID, const uint8_t accLv, const uint8_t secLv);

private:

    void OnCertReconnect (const boost::system::error_code &error);

    void OnGlobal ();

    void OnGlobalReconnect (const boost::system::error_code &error);

    void OnServer ();

    void OnCheckState ();

    void OnAgent (const std::string &host, const std::string &port,
                  const boost::shared_ptr<IConnection> &conn);

    void OnAgentState (const uint32_t connID, const uint8_t state);

    void OnRemoveAgent (const uint32_t connID);

    void OnGShard (const std::string &host, const std::string &port,
                 const boost::shared_ptr<IConnection> &conn);

    void OnGShardState (const uint32_t connID, const uint8_t state);

    void OnRemoveGShard (const uint32_t connID);

    void OnTicket (const Ticket &ticket);

    void OnCertifyTicket (const uint32_t ticketID, const std::string &usr, const std::string &pw,
                          const std::string &host, const boost::shared_ptr<IConnection> &conn);

    void OnAccount (const uint32_t connID, const uint32_t accountID);

    void OnRemoveAccount (const uint32_t connID, const uint32_t accountID);

private:

    uint16_t m_ID;
    uint8_t m_locale;
    uint8_t m_race;
    uint32_t m_version;
    std::string m_error;
    std::string m_name;
    std::string m_chFilter;
    std::vector<std::string> m_strFilter;

    uint8_t m_state;		/// Current shard state (0 - closed, 1 - open)
    uint16_t m_user_count;
    uint16_t m_user_max;

    std::string m_cert_host;
    std::string m_cert_port;
    boost::asio::deadline_timer m_cert_timer;
    boost::shared_ptr<clt::Connection> m_cert_client;

    std::string m_global_host;
    std::string m_global_port;
    boost::asio::deadline_timer m_global_timer;
    boost::shared_ptr<clt::Connection> m_global_client;

    boost::shared_ptr<Server> m_server;

    boost::mutex m_world_mutex;
    std::vector<Area> m_world;	/// WORLD PARTITION

    boost::mutex m_gshards_mutex;
    std::vector<GShard> m_gshards;

    boost::mutex m_agsrv_mutex;
    std::vector<Agent> m_agent_srvs;

    boost::mutex m_ticket_mutex;
    std::map<uint32_t,Ticket> m_tickets;
};

#endif // __ZELOS_SHARDD_SERVICE_H__
