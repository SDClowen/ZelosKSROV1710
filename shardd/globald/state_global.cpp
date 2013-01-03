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

#include "state_global.h"

#include <utility>

#include <net/opcode/global_shard_srv.h>

#include "area.h"
#include "service.h"
#include "ticket.h"

namespace globald
{
    StateGlobal::StateGlobal (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_SHARD_CONFIG] = bind(&StateGlobal::OnConfig,this,_1);
        m_opcode_table[SRV_TICKET_QUEUE] = bind(&StateGlobal::OnTicket,this,_1);
    }

    StateGlobal::~StateGlobal ()
    {
    }

    void StateGlobal::Close ()
    {
    }

    int StateGlobal::OnConfig (const IPacket &packet)
    {
        uint16_t ID = packet.Read<uint16_t>();

        uint8_t locale = packet.Read<uint8_t>();

        uint8_t race = packet.Read<uint8_t>();

        uint32_t version = packet.Read<uint32_t>();

        std::string chFilter = packet.Read();

        size_t size = packet.Read<size_t>();

        std::string tmp;
        std::vector<std::string> strFilter;
        for (size_t i=0; i < size; ++i)
        {
            tmp = packet.Read();
            strFilter.push_back(tmp);
        }

        int area_count = packet.Read<uint16_t>();

        Area area;
        std::vector<Area> world;

        uint16_t zoneID = 0;

        int zone_count = 0;

        for (int i = 0; i < area_count; ++i)
        {
            area.ID = packet.Read<uint16_t>();

            area.Instanced = packet.Read<uint8_t>();

            zone_count = packet.Read<uint16_t>();

            for (int j = 0; j < zone_count; ++j)
            {
                zoneID = packet.Read<uint16_t>();

                area.Zones.push_back(zoneID);
            }

            world.push_back(area);

            area.Zones.clear();
        }

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->setShardID(ID);

        m_service->setLocale(locale);

        m_service->setRace(race);

        m_service->setVersion(version);

        m_service->setFilter(chFilter,strFilter);

        m_service->setWorld(world);

        m_service->runServer();

        return MSG_SUCCESS;
    }

    int StateGlobal::OnTicket (const IPacket &packet)
    {
        Ticket ticket;

        ticket.ID = packet.Read<uint32_t>();

        ticket.usr = packet.Read();

        ticket.pw = packet.Read();

        ticket.IP = packet.Read();

        ticket.AccountID = packet.Read<uint32_t>();

        ticket.access_lv  = packet.Read<uint8_t>();

        ticket.sec_lv = packet.Read<uint8_t>();

        ticket.silk = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addTicket(ticket);

        return MSG_SUCCESS;
    }
}
