#include "state_name.h"

namespace named
{
	StateName::StateName (const boost::shared_ptr<IConnection> &conn)
		: ConnectionState<IConnection>::ConnectionState(conn)
	{
	}

	StateName::~StateName ()
	{
	}

	void StateName::Close ()
	{
	}
}
