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

#include "state_game.h"

#include <net/connection.h>
#include <net/opcode/shard_area_clt.h>

#include "service.h"

namespace srv
{
    StateGame::StateGame (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLT_AREA_CONFIG] = boost::bind(&StateGame::OnConfig,this,_1);
        m_opcode_table[CLT_AREA_STATE] = boost::bind(&StateGame::OnState,this,_1);
    }

    StateGame::~StateGame ()
    {
    }

    void StateGame::Close ()
    {
    }

    int StateGame::OnConfig (const IPacket &packet)
    {
        std::string host = packet.Read();

        std::string port = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addGShard(host,port,m_connection);

        return MSG_SUCCESS;
    }

    int StateGame::OnState (const IPacket &packet)
    {
        uint8_t state = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->setGShardState(m_connection->ID(),state);

        return MSG_SUCCESS;
    }
}
