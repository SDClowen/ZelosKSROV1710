#include "service.h"

#include <auth/authentication_phoenix.h>
#include <net/client/connection.h>

#include "named/states.h"
#include "named/state_handshake.h"
#include "named/state_name.h"
#include "server/server.h"

Service::Service ()
	: m_name_client(new clt::Connection(m_io_service)),
	m_server(new Server(m_io_service))
{
}

Service::~Service ()
{
}

const std::string& Service::getLastError () const
{
	if (m_error.empty() && m_server)
		return m_server->GetLastError();

	return m_error;
}

bool Service::initializeName ()
{
	if (!m_name_client)
		return false;

	boost::shared_ptr<PhoenixAuth> auth;
	boost::shared_ptr<IConnection::State> state;

	auth.reset(new PhoenixAuth(AUTH_CLIENT,0));

	if (!auth)
		return false;

	m_name_client->setAuthentication(auth);

	state.reset(new named::StateName(m_name_client));

	if (!state)
		return false;

	m_name_client->addState(state,named::STATE_NAME);

	state.reset(new named::StateHandshake(m_name_client));

	if (!state)
		return false;

	m_name_client->addState(state,named::STATE_HANDSHAKE);

	m_name_client->setState(named::STATE_HANDSHAKE);

	return true;
}

bool Service::OnInitialize ()
{
	if (m_host.empty())
	{
		return false;
	}

	if (m_port.empty())
	{
		return false;
	}

	if (m_name_host.empty())
	{
		return false;
	}

	if (m_name_port.empty())
	{
		return false;
	}

	if (!m_server || !m_server->initialize())
	{
		m_error = "Failed to initialize server.";
		return false;
	}

	if (!initializeName())
	{
		m_error = "Failed to initialize name client";
		return false;
	}

	m_name_client->connect(m_name_host,m_name_port);

	m_server->Execute();

	return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
	typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

	if (m_server)
		m_server->configure(entries);

	for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
	{
		if (it->first == "Host")
			m_host = it->second;
		else if (it->first == "Port")
			m_port = it->second;
		else if (it->first == "NameHost")
			m_name_host = it->second;
		else if (it->first == "NamePort")
			m_name_port = it->second;
	}
}

void Service::OnRelease ()
{
	if (m_name_client)
		m_name_client->stop();

	if (m_server)
		m_server->Stop();
}
