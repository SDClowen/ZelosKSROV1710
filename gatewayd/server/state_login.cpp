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

#include <net/opcode/gateway_clt.h>
#include <net/server/connection.h>

#include "server/server.h"
#include "service.h"

namespace srv
{
    StateLogin::StateLogin (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLIENT_NEWS] = boost::bind(&StateLogin::OnNews,this,_1);
        m_opcode_table[CLIENT_SERVERS_LIST] = boost::bind(&StateLogin::OnServers,this,_1);
        m_opcode_table[CLIENT_LOGIN] = boost::bind(&StateLogin::OnLogin,this,_1);
    }

    StateLogin::~StateLogin ()
    {
    }

    void StateLogin::Close ()
    {
    }

    int StateLogin::OnNews (const IPacket &packet)
    {
        uint8_t locale = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        if (locale != m_service->getLocale())
             return MSG_ERROR_ARG;

        m_service->sendNews(m_connection);

        return MSG_SUCCESS;
    }

    int StateLogin::OnServers (const IPacket &packet)
    {
        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->sendShards(m_connection);

        return MSG_SUCCESS;
    }

    int StateLogin::OnLogin (const IPacket &packet)
    {
        uint8_t locale = packet.Read<uint8_t>();

        std::string user = packet.Read();

        std::string pass = packet.Read();

        std::string mobile = packet.Read();

        uint16_t servID = packet.Read<uint16_t>();

        uint8_t channel = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        if (locale != m_service->getLocale())
        {
            syslog(LOG_INFO,"Trying to log with a different locale = %i",locale);
            return MSG_ERROR_ARG;
        }

        std::string IP = m_connection->remoteEndpoint();

        m_service->requestTicket(user,pass,servID,IP,m_connection);

        return MSG_SUCCESS;
    }
}
