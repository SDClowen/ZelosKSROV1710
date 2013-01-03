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

#include "server/state_lobby.h"

#include <net/connection.h>
#include <net/opcode/agent_clt.h>
#include <net/opcode/chard_clt.h>

#include "service.h"

namespace srv
{
    StateLobby::StateLobby (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service),
        m_block(false),
        m_accountId(0)
    {
        m_opcode_table[CLIENT_LOBBY] = boost::bind(&StateLobby::OnCommand,this,_1,
                                                   static_cast<uint16_t>(CLT_CHARD_LOBBY));

        m_opcode_table[CLIENT_LOBBY_SELECT] = boost::bind(&StateLobby::OnCommand,this,_1,
                                                          static_cast<uint16_t>(CLT_CHARD_SELECT));
    }

    StateLobby::~StateLobby ()
    {
    }

    void StateLobby::Close ()
    {
    }

    void StateLobby::setAccountID (const uint32_t accID)
    {
        m_accountId = accID;
    }

    void StateLobby::setBlock (bool block)
    {
        __sync_bool_compare_and_swap(&m_block,!block,block);
    }

    int StateLobby::OnCommand (const IPacket &packet, const uint16_t opcode)
    {
        if (m_accountId && __sync_bool_compare_and_swap(&m_block,false,true))
        {
            const char *offset = (const char*)(packet.buffer()+IPacket::HEADER_LENGTH);

            OPacket pkt(opcode);
            pkt.Write(offset,packet.size());
            pkt.Write(m_accountId);

            boost::shared_ptr<IConnection> conn = m_service->getCharClient();

            if (conn)
                conn->send(&pkt);
        }

        return MSG_SUCCESS;
    }
}
