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

#include "characterd/state_player.h"

#include <name/services.h>
#include <net/connection.h>
#include <net/opcode/agent_srv.h>
#include <net/opcode/chard_srv.h>

#include "server/connection.h"
#include "server/states.h"
#include "service.h"

namespace chard
{
    StatePlayer::StatePlayer (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_CHARD_DC] = boost::bind(&StatePlayer::OnDC,this,_1);
        m_opcode_table[SRV_CHARD_LOBBY] = boost::bind(&StatePlayer::OnLobby,this,_1);
        m_opcode_table[SRV_CHARD_SELECT] = boost::bind(&StatePlayer::OnSelect,this,_1);
    }

    StatePlayer::~StatePlayer ()
    {
    }

    void StatePlayer::Close ()
    {
        m_service->reconnectComponent(SERVICE_CHARACTER);
    }

    int StatePlayer::OnDC (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->killConnection(accountID);

        return MSG_SUCCESS;
    }

    int StatePlayer::OnLobby (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        boost::shared_ptr<IConnection> bconn = m_service->findConnection(accountID);

        boost::shared_ptr<agentd::Connection> conn =
                boost::shared_static_cast<agentd::Connection,IConnection>(bconn);

        if (conn)
        {
            const char *offset = (const char*)(packet.buffer()+IPacket::HEADER_LENGTH+4);

            OPacket pkt(SERVER_LOBBY);
            pkt.Write(offset,packet.size()-4);
            conn->send(&pkt);

            conn->setBlock(false);
        }

        return MSG_SUCCESS;
    }

    int StatePlayer::OnSelect (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        uint16_t zoneID = packet.Read<uint16_t>();

        m_service->setAccountArea(accountID,zoneID);

        boost::shared_ptr<IConnection> bconn = m_service->findConnection(accountID);

        boost::shared_ptr<agentd::Connection> conn =
                boost::shared_static_cast<agentd::Connection,IConnection>(bconn);

        if (conn)
        {
            const char *offset = (const char*)(packet.buffer()+IPacket::HEADER_LENGTH+4);

            OPacket pkt(SERVER_LOBBY_SELECT);
            pkt.Write(offset,packet.size()-4);
            conn->send(&pkt);

            conn->setBlock(false);

            conn->setState(srv::STATE_WORLD);
        }

        return MSG_SUCCESS;
    }
}
