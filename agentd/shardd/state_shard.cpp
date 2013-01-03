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

#include <net/opcode/shard_agent_srv.h>

#include "service.h"

namespace shard
{
    StateShard::StateShard (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_AGENT_CONFIG] = boost::bind(&StateShard::OnConfig,this,_1);
        m_opcode_table[SRV_AGENT_AREA] = boost::bind(&StateShard::OnWorldArea,this,_1);
        m_opcode_table[SRV_AGENT_ACTION] = boost::bind(&StateShard::OnAction,this,_1);
        m_opcode_table[SRV_LOGIN_TICKET] = boost::bind(&StateShard::OnTicket,this,_1);
    }

    StateShard::~StateShard ()
    {
    }

    void StateShard::Close ()
    {
    }

    int StateShard::OnConfig (const IPacket &packet)
    {
        uint8_t locale = packet.Read<uint8_t>();

        uint8_t race = packet.Read<uint8_t>();

        uint32_t version = packet.Read<uint32_t>();

        uint8_t maxAreas = packet.Read<uint8_t>();

        std::string chFilter = packet.Read();

        uint32_t size = packet.Read<uint32_t>();

        std::vector<std::string> strFilter;

        for (size_t i = 0; i < size; ++i)
            strFilter.push_back(packet.Read());

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->setLocale(locale);

        m_service->setRace(race);

        m_service->setVersion(version);

        m_service->setMaxAreas(maxAreas);

        m_service->setFilter(chFilter,strFilter);

        return MSG_SUCCESS;
    }

    int StateShard::OnWorldArea (const IPacket &packet)
    {
        Area area;

        area.ID = packet.Read<uint16_t>();
        area.host = packet.Read();
        area.port = packet.Read();

        size_t size = packet.Read<uint16_t>();

        for (size_t i = 0; i < size; ++i)
            area.Zones.insert(packet.Read<uint16_t>());

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addArea(area);

        return MSG_SUCCESS;
    }

    int StateShard::OnAction (const IPacket &packet)
    {
        uint8_t action = packet.Read<uint8_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        if (action)
            m_service->runServer();
        else
            m_service->stopServer();

        return MSG_SUCCESS;
    }

    int StateShard::OnTicket (const IPacket &packet)
    {
        uint8_t ret = packet.Read<uint8_t>();

        if (ret > ANSWER_ERROR)
            return MSG_ERROR_ARG;

        if (ret == ANSWER_ACCEPT)
        {
            uint32_t ticketID = packet.Read<uint32_t>();

            uint32_t accountID = packet.Read<uint32_t>();

            uint8_t secLv = packet.Read<uint8_t>();

            if (!packet.EndOfStream())
                return MSG_ERROR_SIZE;

            m_service->processTicket(ticketID,accountID,secLv);
        }
        else
        {
            uint16_t error_code = packet.Read<uint16_t>();

            uint32_t ticketID = packet.Read<uint32_t>();

            if (!packet.EndOfStream())
                return MSG_ERROR_SIZE;

            m_service->processTicket(ticketID,error_code);
        }

        return MSG_SUCCESS;
    }
}
