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

#include "state_name.h"

#include <net/opcode/name_srv.h>

#include "service.h"

namespace named
{
    StateName::StateName (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[SRV_QUERY] = boost::bind(&StateName::OnQuery,this,_1);
    }

    StateName::~StateName ()
    {
    }

    void StateName::Close ()
    {
    }

    int StateName::OnQuery (const IPacket &packet)
    {
        uint8_t type = packet.Read<uint8_t>();

        std::string host = packet.Read();

        std::string port = packet.Read();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->addComponent(type,host,port);

        return MSG_SUCCESS;
    }
}
