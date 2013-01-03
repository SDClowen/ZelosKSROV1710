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

#include <net/client/connection.h>
#include <name/services.h>
#include <name/packet.h>

#include "service.h"
#include "states.h"

namespace named
{
    StateHandshake::StateHandshake (Service *service, const boost::shared_ptr<IConnection> &conn)
        : clt::IStateHandshake("NameServer","ServiceServer",conn),
        m_service(service)
    {
    }

    StateHandshake::~StateHandshake ()
    {
    }

    void StateHandshake::OnConfig ()
    {
        boost::shared_static_cast<clt::Connection,IConnection>(m_connection)->KeepAlive();

        m_connection->setState(named::STATE_NAME);

        OPacket pkt;
        clt_pkt::RegisterService(&pkt,SERVICE_CHARACTER,m_service->getHost(),m_service->getPort());
        m_connection->send(&pkt);
    }
}
