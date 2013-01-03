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

#include "globald/state_global.h"

#include <net/connection.h>
#include <net/opcode/gateway_srv.h>
#include <net/opcode/global_gateway_srv.h>

#include "news.h"
#include "server/server.h"
#include "service.h"
#include "shard.h"

namespace clt
{

StateGlobal::StateGlobal (Service *service, const boost::shared_ptr<IConnection> &conn)
    : ConnectionState<IConnection>::ConnectionState(conn), m_service(service)
{
    using boost::bind;

    m_opcode_table[SRV_CONFIG] = bind(&StateGlobal::OnConfig,this,_1);
    m_opcode_table[SRV_ACCOUNT_PROCESS] = bind(&StateGlobal::OnAccountProcess,this,_1);
    m_opcode_table[SRV_NEWS_INSERT] = bind(&StateGlobal::OnNewsInsert,this,_1);
    m_opcode_table[SRV_NEWS_REMOVE] = bind(&StateGlobal::OnNewsRemove,this,_1);
    m_opcode_table[SRV_SHARD_INSERT] = bind(&StateGlobal::OnShardInsert,this,_1);
    m_opcode_table[SRV_SHARD_REMOVE] = bind(&StateGlobal::OnShardRemove,this,_1);
    m_opcode_table[SRV_SHARD_NOTIFY] = bind(&StateGlobal::OnShardNotify,this,_1);
    m_opcode_table[SRV_PATCH_INSERT] = bind(&StateGlobal::OnPatchInsert,this,_1);
    m_opcode_table[SRV_PATCH_REMOVE] = bind(&StateGlobal::OnPatchRemove,this,_1);
    m_opcode_table[SRV_PATCH_NOTIFY] = bind(&StateGlobal::OnPatchNotify,this,_1);
}

StateGlobal::~StateGlobal ()
{
}

void StateGlobal::Close ()
{
}

int StateGlobal::OnConfig (const IPacket &packet)
{
    uint8_t locale = packet.Read<uint8_t>();

    uint32_t client_version = packet.Read<uint32_t>();

    uint8_t serv_size = packet.Read<uint8_t>();

    Shard serv;
    std::vector<Shard> shard_list;

    for (int i = 0; i < serv_size; ++i)
    {
        serv.is_new = packet.Read<uint8_t>();
        serv.name = packet.Read();
        serv.id = packet.Read<uint16_t>();
        serv.usr_cnt = packet.Read<uint16_t>();
        serv.usr_max = packet.Read<uint16_t>();
        serv.state = packet.Read<uint8_t>();

        shard_list.push_back(serv);
    }

    uint8_t news_size = packet.Read<uint8_t>();

    Notice news;
    std::vector<Notice> news_list;

    for (int i = 0; i < news_size; ++i)
    {
        news.id = packet.Read<uint32_t>();
        news.heading = packet.Read();
        news.message = packet.Read();
        news.month = packet.Read<uint16_t>();
        news.day = packet.Read<uint16_t>();

        news_list.push_back(news);
    }

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    boost::shared_ptr<Server> server = m_service->getServer();

    server->SetLocale(locale);

    server->SetClientVersion(client_version);

    std::for_each(shard_list.begin(),shard_list.end(),boost::bind(&Service::addShard,m_service,_1));

    std::for_each(news_list.begin(),news_list.end(),boost::bind(&Service::addNews,m_service,_1));

    m_service->runServer();

    return MSG_SUCCESS;
}

int StateGlobal::OnAccountProcess (const IPacket &packet)
{
    /// FOWARD THE RESPONSE FROM THE MASTER SERVER TO THE RESPECTIVE CLIENT
    uint32_t clientID = packet.Read<uint32_t>();

    uint8_t ret = packet.Read<uint8_t>();

    OPacket pkt;
    pkt.WriteOpcode(SERVER_LOGIN);
    pkt.Write<uint8_t>(ret);

    while (!packet.EndOfStream())
        pkt.Write<uint8_t>(packet.Read<uint8_t>());

    m_service->getServer()->send(&pkt,clientID);

    return MSG_SUCCESS;
}

int StateGlobal::OnNewsInsert (const IPacket &packet)
{
    Notice news;
    news.id = packet.Read<uint32_t>();
    news.heading = packet.Read();
    news.message = packet.Read();
    news.month = packet.Read<uint16_t>();
    news.day = packet.Read<uint16_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->addNews(news);

    return MSG_SUCCESS;
}

int StateGlobal::OnNewsRemove (const IPacket &packet)
{
    uint32_t ID = packet.Read<uint32_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->removeNews(ID);

    return MSG_SUCCESS;
}

int StateGlobal::OnShardInsert (const IPacket &packet)
{
    Shard serv;
    serv.is_new = packet.Read<uint8_t>();
    serv.name = packet.Read();
    serv.id = packet.Read<uint16_t>();
    serv.usr_cnt = packet.Read<uint16_t>();
    serv.usr_max = packet.Read<uint16_t>();
    serv.state = packet.Read<uint8_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->addShard(serv);

    return MSG_SUCCESS;
}

int StateGlobal::OnShardRemove (const IPacket &packet)
{
    uint16_t ID = packet.Read<uint16_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->removeShard(ID);

    return MSG_SUCCESS;
}

int StateGlobal::OnShardNotify (const IPacket &packet)
{
    enum
    {
        SHARD_NOTIFY_USR_CNT,
        SHARD_NOTIFY_USR_MAX,
        SHARD_NOTIFY_STATE
    };

    uint8_t action = packet.Read<uint8_t>();

    uint16_t ID = packet.Read<uint16_t>();

    uint16_t arg = packet.Read<uint16_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    switch(action)
    {
    case SHARD_NOTIFY_USR_CNT:
    case SHARD_NOTIFY_USR_MAX:
    case SHARD_NOTIFY_STATE:
        m_service->updateShard(action,ID,arg);
        break;
    default:
        break;
    }

    return MSG_SUCCESS;
}

int StateGlobal::OnPatchInsert (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    return MSG_SUCCESS;
}

int StateGlobal::OnPatchRemove (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    return MSG_SUCCESS;
}

int StateGlobal::OnPatchNotify (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    return MSG_SUCCESS;
}

}
