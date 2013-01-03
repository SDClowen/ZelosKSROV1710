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

#include "server/state_handshake.h"

#include <net/server/connection.h>

#include "server/states.h"

namespace srv
{

StateHandshake::StateHandshake (const boost::shared_ptr<IConnection> &conn, const std::string &srvID,
                                const std::string &clientID)
    : srv::IStateHandshake(srvID,conn),
    m_client_id(clientID)
{
}

StateHandshake::~StateHandshake ()
{
}

bool StateHandshake::OnClientID (const std::string &ID)
{
    if (ID != m_client_id)
        return false;

    m_connection->setState(srv::STATE_VERSION);

    return true;
}

}
