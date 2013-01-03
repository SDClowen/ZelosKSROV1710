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

#include "state_handshake.h"

#include <net/connection.h>

#include "server/states.h"

namespace srv
{

    StateHandshake::StateHandshake (const boost::shared_ptr<IConnection> &conn, const std::string &srvID)
        : srv::IStateHandshake(srvID,conn)
    {
    }

    StateHandshake::~StateHandshake ()
    {
    }

    bool StateHandshake::OnClientID (const std::string &ID)
    {
        m_connection->setState(srv::STATE_REGISTER);

        return true;
    }

}
