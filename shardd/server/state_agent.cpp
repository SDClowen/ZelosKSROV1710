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

#include "state_agent.h"

#include <net/connection.h>
#include <net/opcode/shard_agent_clt.h>

#include "service.h"

namespace srv
{
    StateAgent::StateAgent (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLT_AGENT_CONFIG] = boost::bind(&StateAgent::OnConfig,this,_1);
        m_opcode_table[CLT_AGENT_STATE] = boost::bind(&StateAgent::OnState,this,_1);
        m_opcode_table[CLT_LOGIN_TICKET] = boost::bind(&StateAgent::OnTicket,this,_1);
        m_opcode_table[CLT_ACCOUNT_STATE] = boost::bind(&StateAgent::OnAccount,this,_1);
    }

    StateAgent::~StateAgent ()
    {
    }

    void StateAgent::Close ()
    {
    }

    int StateAgent::OnConfig (const IPacket &packet)
    {
        std::string host = packet.Read();

        std::string port = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addAgent(host,port,m_connection);

        return MSG_SUCCESS;
    }

    int StateAgent::OnState (const IPacket &packet)
    {
        uint8_t state = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->updateAgent(m_connection->ID(),state);

        return MSG_SUCCESS;
    }

    int StateAgent::OnTicket (const IPacket &packet)
    {
        uint32_t ticketID = packet.Read<uint32_t>();

        std::string usr = packet.Read();

        std::string pw = packet.Read();

        std::string host = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->certifyTicket(ticketID,usr,pw,host,m_connection);

        return MSG_SUCCESS;
    }

    int StateAgent::OnAccount (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        uint8_t state = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        if (state)
            m_service->addAccount(m_connection->ID(),accountID);
        else
            m_service->removeAccount(m_connection->ID(),accountID);

        return MSG_SUCCESS;
    }
}
