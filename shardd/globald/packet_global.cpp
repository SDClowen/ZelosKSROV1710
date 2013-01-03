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

#include "globald/packet_global.h"

#include <packet/packet.h>
#include <net/opcode/global_shard_clt.h>

namespace clt_pkt
{
    void Config (OPacket *pkt, const std::string &name)
    {
        pkt->WriteOpcode(CLT_SHARD_CONFIG);
        pkt->Write(name);
    }

    void ShardNotify8 (OPacket *pkt, const SHARD_NOTIFY type, const uint32_t ID, const uint8_t arg)
    {
        pkt->WriteOpcode(CLT_SHARD_NOTIFY);
        pkt->Write<uint8_t>(type);
        pkt->Write<uint16_t>(ID);
        pkt->Write<uint8_t>(arg);
    }

    void ShardNotify16 (OPacket *pkt, const SHARD_NOTIFY type, const uint32_t ID, const uint16_t arg)
    {
        pkt->WriteOpcode(CLT_SHARD_NOTIFY);
        pkt->Write<uint8_t>(type);
        pkt->Write<uint16_t>(ID);
        pkt->Write<uint16_t>(arg);
    }

    void AgentInsert (OPacket *pkt, const uint16_t shardID, const uint32_t agentID,
                      const std::string &host, const std::string &port)
    {
        pkt->WriteOpcode(CLT_SHARD_AGENT_ADD);
        pkt->Write<uint16_t>(shardID);
        pkt->Write<uint32_t>(agentID);
        pkt->Write(host);
        pkt->Write(port);
    }

    void AgentRemove (OPacket *pkt, const uint16_t shardID, const uint32_t agentID)
    {
        pkt->WriteOpcode(CLT_SHARD_AGENT_DEL);
        pkt->Write<uint16_t>(shardID);
        pkt->Write<uint32_t>(agentID);
    }

    void AccountAuthenticated (OPacket *pkt, const uint32_t accountID, const uint32_t ticketID)
    {
        pkt->WriteOpcode(CLT_ACCOUNT_NOTIFY);
        pkt->Write<uint8_t>(clt_pkt::ACCOUNT_NOTIFY_STATE);
        pkt->Write<uint32_t>(accountID);
        pkt->Write<uint8_t>(true);
        pkt->Write<uint32_t>(ticketID);
    }

    void AccountNotify8 (OPacket *pkt, const ACCOUNT_NOTIFY type, const uint32_t AccountID,
                         const uint8_t arg)
    {
        pkt->WriteOpcode(CLT_ACCOUNT_NOTIFY);
        pkt->Write<uint8_t>(type);
        pkt->Write<uint32_t>(AccountID);
        pkt->Write<uint8_t>(arg);
    }

    void AccountNotify32 (OPacket *pkt, const ACCOUNT_NOTIFY type, const uint32_t AccountID,
                          const uint32_t arg)
    {
        pkt->WriteOpcode(CLT_ACCOUNT_NOTIFY);
        pkt->Write<uint8_t>(type);
        pkt->Write<uint32_t>(AccountID);
        pkt->Write<uint32_t>(arg);
    }
}
