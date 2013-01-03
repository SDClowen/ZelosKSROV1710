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

#ifndef __ZELOS_SHARDD_GLOBALD_PACKET_GLOBAL_H__
#define __ZELOS_SHARDD_GLOBALD_PACKET_GLOBAL_H__

#include <stdint.h>
#include <string>

class OPacket;

/**
 *
 *  @brief Packets sent from the shard server to the manager server.
 *
 **/

namespace clt_pkt
{
    enum ACCOUNT_NOTIFY
    {
        ACCOUNT_NOTIFY_STATE,
        ACCOUNT_NOTIFY_SILK,
        ACCOUNT_NOTIFY_BAN
    };

    enum SHARD_NOTIFY
    {
        SHARD_NOTIFY_USR_CNT,
        SHARD_NOTIFY_USR_MAX,
        SHARD_NOTIFY_STATE
    };

    void Config (OPacket *pkt, const std::string &name);

    void AgentInsert (OPacket *pkt, const uint16_t shardID, const uint32_t agentID, const std::string &host,
                      const std::string &port);

    void AgentRemove (OPacket *pkt, const uint16_t shardID, const uint32_t agentID);

    void AccountAuthenticated (OPacket *pkt, const uint32_t accountID, const uint32_t ticketID);

    void AccountNotify8 (OPacket *pkt, const ACCOUNT_NOTIFY type, const uint32_t AccountID,
                         const uint8_t arg);

    void AccountNotify32 (OPacket *pkt, const ACCOUNT_NOTIFY type, const uint32_t AccountID,
                          const uint32_t arg);

    void ShardNotify8 (OPacket *pkt, const SHARD_NOTIFY type, const uint32_t shardID,
                       const uint8_t arg);

    void ShardNotify16 (OPacket *pkt, const SHARD_NOTIFY type, const uint32_t shardID,
                        const uint16_t arg);
}

#endif // __ZELOS_SHARDD_GLOBALD_PACKET_GLOBAL_H__
