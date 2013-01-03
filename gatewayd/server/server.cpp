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
#include "server/state_handshake.h"
#include "server/state_version.h"
#include "server/state_login.h"

Server::Server (Service *service, boost::asio::io_service &io_service)
    : IServer(io_service),
    m_service(service)
{
    m_IDS = "GatewayServer";

    m_client_id = "SR_Client";
    m_server_list_id = "ZelosOfficial";
}

Server::~Server ()
{

}

void Server::SetLocale(const uint8_t locale)
{
    m_locale = locale;
}

void Server::SetClientVersion (const uint32_t version)
{
    m_client_version = version;
}

void Server::SetClientID (const std::string &ID)
{
    m_client_id = ID;
}

void Server::SetServerListID (const std::string &ID)
{
    m_server_list_id = ID;
}

const std::string& Server::ClientID () const
{
    return m_client_id;
}

const std::string& Server::ServerListID () const
{
    return m_server_list_id;
}

bool Server::OnInitialize ()
{
    if (!m_security_mode)
    {
        m_last_error = "Invalid SecurityMode. Check your configuration file.";
        return false;
    }

    if (m_LogPath.empty())
    {
        m_last_error = "Log directory entry empty. Check your configuration file.";
        return false;
    }

    return true;
}

void Server::OnConfigure (const std::map<std::string,std::string> &_entries)
{
}

void Server::OnRemoveConnection (const uint32_t connID, const uint8_t state)
{
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

        boost::shared_ptr<ConnectionState<IConnection> > state;

        state.reset(new srv::StateHandshake(conn,m_IDS,m_client_id));

        conn->addState(state,srv::STATE_HANDSHAKE);

        state.reset(new srv::StateVersion(m_service,conn));

        conn->addState(state,srv::STATE_VERSION);

        state.reset(new srv::StateLogin(m_service,conn));

        conn->addState(state,srv::STATE_LOGIN);

        conn->setState(srv::STATE_HANDSHAKE);
    }

    return conn;
}

void Server::OnRelease()
{
}
