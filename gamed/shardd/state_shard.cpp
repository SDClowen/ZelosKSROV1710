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

#include "shardd/state_shard.h"

#include <net/opcode/shard_area_srv.h>

#include "service.h"

namespace shard
{
    StateShard::StateShard (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_AREA_CONFIG] = boost::bind(&StateShard::OnConfig,this,_1);
        m_opcode_table[SRV_AREA_ACTION] = boost::bind(&StateShard::OnAction,this,_1);
    }

    StateShard::~StateShard ()
    {
    }

    void StateShard::Close ()
    {
    }

    int StateShard::OnConfig (const IPacket &packet)
    {
        WorldArea area;

        area.ID = packet.Read<uint16_t>();

        area.Instanced = packet.Read<uint8_t>();

        uint16_t num_zones = packet.Read<uint16_t>();

        for (size_t i = 0; i < num_zones; ++i)
            area.Zones.insert(packet.Read<uint16_t>());

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->runServer();

        return MSG_SUCCESS;
    }

    int StateShard::OnAction (const IPacket &packet)
    {
        uint8_t action = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        if (action)
            m_service->runSimulation();
        else
            m_service->stopSimulation();

        return MSG_SUCCESS;
    }
}
