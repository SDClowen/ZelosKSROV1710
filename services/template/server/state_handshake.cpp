#include "state_handshake.h"

#include <net/connection.h>

#include "server/states.h"

namespace srv
{
	StateHandshake::StateHandshake (const std::string &srvID, const boost::shared_ptr<IConnection> &conn)
		: srv::IStateHandshake(srvID,conn)
	{
	}

	StateHandshake::~StateHandshake ()
	{
	}

	bool StateHandshake::OnClientID (const std::string &ID)
	{
		m_connection->setState(srv::STATE_COMMAND);

		return true;
	}
}
