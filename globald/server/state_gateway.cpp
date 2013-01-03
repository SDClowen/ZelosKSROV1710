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

#include "server/state_gateway.h"

#include <net/connection.h>
#include <net/opcode/global_gateway_clt.h>

#include "service.h"

namespace srv
{
    StateGateway::StateGateway (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        using boost::bind;

        m_opcode_table[CLT_CONFIG] = bind(&StateGateway::OnConfig,this,_1);
        m_opcode_table[CLT_LOGIN_QUEUE] = bind(&StateGateway::OnQueue,this,_1);
    }

    StateGateway::~StateGateway ()
    {
    }

    void StateGateway::Close ()
    {
    }

    int StateGateway::OnConfig (const IPacket &packet)
    {
        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addGateway(m_connection);

        return MSG_SUCCESS;
    }

    int StateGateway::OnQueue (const IPacket &packet)
    {
        Ticket ticket;

        ticket.ID = packet.Read<uint32_t>();

        ticket.shardID = packet.Read<uint32_t>();

        ticket.usr = packet.Read();

        ticket.pw = packet.Read();

        ticket.IP = packet.Read();

        ticket.conn = m_connection;

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addTicket(ticket);

        return MSG_SUCCESS;
    }
}
