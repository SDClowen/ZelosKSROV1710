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

#include "server/state_version.h"

#include <auth/packet_srv_authentication.h>
#include <net/opcode/common_clt.h>
#include <net/server/connection.h>

#include "server/server.h"
#include "server/states.h"
#include "service.h"

namespace srv
{
    StateVersion::StateVersion (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLIENT_VERSION_INFO] = boost::bind(&StateVersion::OnVersionInfo,this,_1);
    }

    StateVersion::~StateVersion ()
    {
    }

    void StateVersion::Close ()
    {
    }

    int StateVersion::OnVersionInfo (const IPacket &packet)
    {
        uint8_t locale = packet.Read<uint8_t>();

        std::string ID = packet.Read();

        uint32_t version = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        boost::shared_ptr<Server> server = m_service->getServer();

        if (!server)
            return MSG_ERROR;

        if (locale != server->Locale() || ID != server->ClientID())
        {
            syslog(LOG_INFO,"Trying to log with a different locale");
            return MSG_ERROR_ARG;
        }

        if (version == server->ClientVersion())
        {
            m_connection->setState(srv::STATE_LOGIN);

            OPacket pkt;

            srv_pkt::WriteVersionE(&pkt);
            m_connection->send(&pkt);

            pkt.Clear();

            srv_pkt::WriteVersionF(&pkt);   ///PERFORM UPDATE PACKET!!
            m_connection->send(&pkt);
        }
        else if (version < server->ClientVersion())
        {
            OPacket pkt(0x600D);
            pkt.Write<uint8_t>(1);
            pkt.Write<uint16_t>(1);
            pkt.Write<uint16_t>(0xA100);
            m_connection->send(&pkt);

            pkt.Clear();

            pkt.WriteOpcode(0x600D);
            pkt.Write<uint8_t>(0);
            pkt.Write<uint8_t>(2);
            pkt.Write<uint8_t>(5);
            m_connection->send(&pkt);
    //		///REDIRECT TO UPDATE SERVER!!!
    //		m_connection->Stop();
        }
        else
        {
            /// TOO NEW
            OPacket pkt(0x600D);
            pkt.Write<uint8_t>(1);
            pkt.Write<uint16_t>(1);
            pkt.Write<uint16_t>(0xA100);
            m_connection->send(&pkt);

            pkt.Clear();

            pkt.WriteOpcode(0x600D);
            pkt.Write<uint8_t>(0);
            pkt.Write<uint8_t>(2);
            pkt.Write<uint8_t>(1);
            m_connection->send(&pkt);
        }

        return MSG_SUCCESS;
    }
}
