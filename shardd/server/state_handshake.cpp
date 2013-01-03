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
#include "service.h"

namespace srv
{
    StateHandshake::StateHandshake (Service *service, const std::string &srvID,
                                    const boost::shared_ptr<IConnection> &conn)
        : srv::IStateHandshake(srvID,conn),
        m_service(service)
    {
    }

    StateHandshake::~StateHandshake ()
    {
    }

    bool StateHandshake::OnClientID (const std::string &cltID)
    {
        if (cltID == "AgentServer")
        {
            m_connection->setState(srv::STATE_AGENT);
        }
        else if (cltID == "GameServer")
        {
            m_connection->setState(srv::STATE_GSHARD);
        }
        else
        {
            syslog(LOG_INFO,"Invalid client ID = %s",cltID.c_str());
            return false;
        }

        return true;
    }
}
