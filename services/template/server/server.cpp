#include "server.h"

#include <auth/authentication_phoenix.h>
#include <net/server/connection.h>

#include "server/states.h"

Server::Server (boost::asio::io_service &io_service)
	: IServer(io_service)
{
}

Server::~Server ()
{
}

bool Server::OnInitialize ()
{
	return true;
}

void Server::OnConfigure (const std::map<std::string, std::string> &entries)
{
}

void Server::OnRemoveConnection (const uint32_t connID)
{
}

void Server::OnRelease ()
{
}

boost::shared_ptr<IConnection> Server::CreateConnection ()
{
	boost::shared_ptr<srv::Connection> conn;

	boost::shared_ptr<PhoenixAuth> auth(new PhoenixAuth(AUTH_SERVER,m_security_mode));

	if (!auth)
		return conn;

	conn.reset(new srv::Connection(++m_counter,m_connection_timeout,m_io_service,this));

	if (conn)
	{
		/// TODO: ADD CONNECTION STATES
	}
}
