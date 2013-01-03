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

#include "state_agent.h"

#include <net/opcode/area_agent_clt.h>

#include "service.h"

namespace srv
{
    StateAgent::StateAgent (Service *service, const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLT_LOAD_CHARACTER] = boost::bind(&StateAgent::OnLoadCharacter,this,_1);
    }

    StateAgent::~StateAgent ()
    {
    }

    void StateAgent::Close ()
    {
    }

    int StateAgent::OnLoadCharacter(const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->loadCharacter(accountID,m_connection);

        return MSG_SUCCESS;
    }
}
