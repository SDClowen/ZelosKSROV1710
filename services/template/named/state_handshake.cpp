#include "state_handshake.h"

#include <net/client/connection.h>
#include <name/services.h>
#include <name/packet.h>

#include "states.h"

namespace named
{
	StateHandshake::StateHandshake (const boost::shared_ptr<IConnection> &conn)
		: clt::IStateHandshake("NameServer","ServiceServer",conn)
	{
	}

	StateHandshake::~StateHandshake ()
	{
	}

	void StateHandshake::OnConfig ()
	{
		boost::shared_static_cast<clt::Connection,IConnection>(m_connection)->KeepAlive();

		m_connection->setState(named::STATE_NAME);
	}
}
