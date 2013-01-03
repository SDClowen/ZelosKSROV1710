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

#include "server.h"

#include <auth/authentication_sro.h>
#include <net/server/connection.h>

#include "server/states.h"
#include "server/state_agent.h"
#include "server/state_game.h"
#include "server/state_handshake.h"
#include "service.h"

Server::Server (Service *service, boost::asio::io_service &io_service)
    : IServer(io_service),
    m_service(service)
{
    m_IDS = "GameServer";
}

Server::~Server ()
{
}

void Server::setCloseCallback (const boost::function<void ()> &fn)
{
    m_close_cb = fn;
}

bool Server::OnInitialize ()
{
    return true;
}

void Server::OnConfigure (const std::map<std::string, std::string> &entries)
{
}

void Server::OnRemoveConnection (const uint32_t connID, const uint8_t state)
{
    if (state == srv::STATE_AGENT)
        m_service->removeAgent(connID);
}

void Server::OnRelease ()
{
    if (m_close_cb)
        m_close_cb();
}

boost::shared_ptr<IConnection> Server::CreateConnection ()
{
    boost::shared_ptr<srv::Connection> conn;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_SERVER,m_security_mode));

    if (!auth)
        return conn;

    conn.reset(new srv::Connection(++m_counter,m_connection_timeout,
                                   m_io_service,shared_from_this()));

    if (conn)
    {
        conn->setAuthentication(auth);

        boost::shared_ptr<IConnection::State> state;

        state.reset(new srv::StateAgent(m_service,conn));

        conn->addState(state,srv::STATE_AGENT);

        state.reset(new srv::StateGame(m_service,conn));

        conn->addState(state,srv::STATE_GAME);

        state.reset(new srv::StateHandshake(m_service,m_IDS,conn));

        conn->addState(state,srv::STATE_HANDSHAKE);

        conn->setState(srv::STATE_HANDSHAKE);
    }

    return conn;
}
