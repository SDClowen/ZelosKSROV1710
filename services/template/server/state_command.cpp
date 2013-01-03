#include "state_command.h"

namespace srv
{
	StateCommand::StateCommand (const boost::shared_ptr<IConnection> &conn)
		: ConnectionState<IConnection>::ConnectionState(conn)
	{
	}

	StateCommand::~StateCommand ()
	{
	}

	void StateCommand::Close ()
	{
	}
}
