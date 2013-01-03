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

#include "server/state_shard.h"

#include <net/opcode/global_shard_clt.h>
#include <net/server/connection.h>

#include "server/packet_gateway.h"
#include "service.h"

namespace srv
{
    StateShard::StateShard (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        using boost::bind;

        m_opcode_table[CLT_SHARD_CONFIG] = bind(&StateShard::OnConfig,this,_1);
        m_opcode_table[CLT_SHARD_NOTIFY] = bind(&StateShard::OnShardNotify,this,_1);
        m_opcode_table[CLT_SHARD_AGENT_ADD] = bind(&StateShard::OnAgentInsert,this,_1);
        m_opcode_table[CLT_SHARD_AGENT_DEL] = bind(&StateShard::OnAgentRemove,this,_1);
        m_opcode_table[CLT_ACCOUNT_NOTIFY] = bind(&StateShard::OnAccountNotify,this,_1);
    }

    StateShard::~StateShard ()
    {
    }

    void StateShard::Close ()
    {
    }

    int StateShard::OnConfig (const IPacket &packet)
    {
        std::string name = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addShard(name,m_connection);

        return MSG_SUCCESS;
    }

    int StateShard::OnShardNotify (const IPacket &packet)
    {
        enum SHARD_NOTIFY
        {
            SHARD_NOTIFY_USR_CNT,
            SHARD_NOTIFY_USR_MAX,
            SHARD_NOTIFY_STATE
        };

        uint8_t type = packet.Read<uint8_t>();

        uint16_t ID = packet.Read<uint16_t>();

        uint16_t arg = 0;

        switch(type)
        {
        case SHARD_NOTIFY_USR_CNT:
        case SHARD_NOTIFY_USR_MAX:
            arg = packet.Read<uint16_t>();
            break;
        case SHARD_NOTIFY_STATE:
            arg = packet.Read<uint8_t>();
            break;
        default:
            break;
        }

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->updateShard(type,ID,arg);

        return MSG_SUCCESS;
    }

    int StateShard::OnAgentInsert (const IPacket &packet)
    {
        uint16_t shardID = packet.Read<uint16_t>();

        uint32_t agentID = packet.Read<uint32_t>();

        std::string host = packet.Read();

        std::string port = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addAgent(shardID,agentID,host,port);

        return MSG_SUCCESS;
    }

    int StateShard::OnAgentRemove (const IPacket &packet)
    {
        uint16_t shardID = packet.Read<uint16_t>();

        uint32_t agentID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->removeAgent(shardID,agentID);

        return MSG_SUCCESS;
    }

    int StateShard::OnAccountNotify (const IPacket &packet)
    {
        enum ACCOUNT_NOTIFY
        {
            ACCOUNT_NOTIFY_STATE,
            ACCOUNT_NOTIFY_SILK,
            ACCOUNT_NOTIFY_BAN,
        };

        uint8_t type = packet.Read<uint8_t>();

        uint32_t accountID = packet.Read<uint32_t>();

        switch(type)
        {
        case ACCOUNT_NOTIFY_STATE:
            {
                uint8_t state = packet.Read<uint8_t>();

                if (!packet.EndOfStream())
                    return MSG_ERROR_SIZE;

                if (state)
                    m_service->addAccount(accountID,m_connection->ID());
                else
                    m_service->removeAccount(accountID,m_connection->ID());
            }
            break;
        case ACCOUNT_NOTIFY_BAN:
            {
                uint8_t years = packet.Read<uint8_t>();

                if (!packet.EndOfStream())
                    return MSG_ERROR_SIZE;

                m_service->setAccountBan(accountID,years);
            }
            break;
        case ACCOUNT_NOTIFY_SILK:
            {
                uint32_t arg = packet.Read<uint32_t>();

                if (!packet.EndOfStream())
                    return MSG_ERROR_SIZE;

                m_service->setAccountSilk(accountID,arg);
            }
            break;
        default:
            break;
        }

        return MSG_SUCCESS;
    }
}
