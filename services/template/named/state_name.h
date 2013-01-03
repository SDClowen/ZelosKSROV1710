#ifndef __PHOENIX_SERVICES_DATABASED_NAMED_STATE_NAME_H__
#define __PHOENIX_SERVICES_DATABASED_NAMED_STATE_NAME_H__

#include <net/connection_state.h>

class IConnection;

namespace named
{
	class StateName : public ConnectionState<IConnection>
	{
	public:

		StateName (const boost::shared_ptr<IConnection> &conn);

		virtual ~StateName ();

		void Close ();
	};
}

#endif // __PHOENIX_SERVICES_DATABASED_NAMED_STATE_NAME_H__
