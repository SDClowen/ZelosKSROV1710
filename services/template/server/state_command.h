#ifndef __PHOENIX_SERVICES_DATABASED_STATE_COMMAND_H__
#define __PHOENIX_SERVICES_DATABASED_STATE_COMMAND_H__

#include <net/connection_state.h>

class IConnection;

namespace srv
{
	class StateCommand : public ConnectionState<IConnection>
	{
	public:

		StateCommand (const boost::shared_ptr<IConnection> &conn);

		virtual ~StateCommand ();

		void Close ();
	};
}

#endif // __PHOENIX_SERVICES_DATABASED_STATE_COMMAND_H__
