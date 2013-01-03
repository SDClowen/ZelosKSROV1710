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

#ifndef __ZELOS_GATEWAYD_SERVICE_H__
#define __ZELOS_GATEWAYD_SERVICE_H__

#include <boost/shared_ptr.hpp>

#include <net/service.h>
#include <net/service_client.h>

#include "news.h"
#include "shard.h"

namespace clt
{
    class Connection;
}

class Server;
class IConnection;
class OPacket;

class Service : public IService
{
public:

    Service();

    virtual ~Service ();

    virtual const std::string& getLastError() const;

    uint8_t getLocale () const;

    const std::string& ServerListID () const;

    void addShard (const Shard &shard);

    void updateShard (const uint8_t type, const uint16_t shardID, const uint16_t arg);

    void removeShard (const uint32_t shardID);

    void sendShards (const boost::shared_ptr<IConnection> &conn);

    void addNews (const Notice& news);

    void removeNews (const uint32_t newsID);

    void sendNews (const boost::shared_ptr<IConnection> &conn);

    void requestTicket (const std::string &usr, const std::string &pw,
                        const uint16_t shardID, const std::string &IP,
                        const boost::shared_ptr<IConnection> &conn);

    void sendGlobal (OPacket *pkt);

    void runServer ();

    boost::shared_ptr<Server> getServer () const;

    boost::shared_ptr<clt::Connection> getClient () const;


private:

    bool initializeGlobal ();

    bool initializeServer ();

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string,std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type,
                        const boost::system::error_code &error);

    void OnRelease ();

private:

    void OnServerRun ();

    void OnGlobalReconnect (const boost::system::error_code &error);

    void OnNews (const boost::shared_ptr<IConnection> &conn);

    void OnNewsInsert (const Notice &news);

    void OnNewsRemove (const uint32_t newsID);

    void OnShards (const boost::shared_ptr<IConnection> &conn);

    void OnShardInsert (const Shard &shard);

    void OnShardUpdate (const uint8_t type, const uint16_t shardID, const uint16_t arg);

    void OnShardRemove (const uint32_t shardID);

    void OnTicketRequest (const std::string &usr, const std::string &pw,
                          const uint16_t shardID, const std::string &IP,
                          const boost::shared_ptr<IConnection> &conn);

private:

    ServiceClt m_global;

    bool m_srv_state;
    boost::shared_ptr<Server> m_server;

    boost::asio::strand m_news_strand;
    boost::asio::strand m_shard_strand;

    std::map<uint32_t,Notice> m_news;
    std::map<uint16_t,Shard> m_shards;
};

#endif // __ZELOS_GATEWAYD_SERVICE_H__
