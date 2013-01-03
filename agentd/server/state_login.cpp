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

#include "server/state_login.h"

#include <net/connection.h>
#include <net/opcode/agent_clt.h>

#include "service.h"
#include "ticket.h"

namespace srv
{
    StateLogin::StateLogin (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLIENT_LOGIN] = boost::bind(&StateLogin::OnLogin,this,_1);
    }

    StateLogin::~StateLogin ()
    {
    }

    void StateLogin::Close ()
    {
    }

    int StateLogin::OnLogin (const IPacket &packet)
    {
        Ticket ticket;

        ticket.ID = packet.Read<uint32_t>();

        ticket.usr = packet.Read();

        ticket.pw = packet.Read();

        uint8_t locale = packet.Read<uint8_t>();

        uint8_t mac[6];

        for (int i = 0; i < 6; ++i)
            mac[i] = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        ticket.conn = m_connection;

        ticket.host = m_connection->remoteEndpoint();

        if (locale != m_service->getLocale())
        {
            syslog(LOG_INFO,"Trying to log with a different locale %i",locale);
            return MSG_ERROR_ARG;
        }

        m_service->addTicket(ticket);

        return MSG_SUCCESS;
    }
}
