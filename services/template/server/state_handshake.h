#ifndef __PHOENIX_SERVICES_DATABASED_SERVER_STATE_HANDSHAKE_H__
#define __PHOENIX_SERVICES_DATABASED_SERVER_STATE_HANDSHAKE_H__

#include <net/server/state_handshake.h>

namespace srv
{
	class StateHandshake : public srv::IStateHandshake
	{
	public:

		StateHandshake (const std::string &srvID, const boost::shared_ptr<IConnection> &conn);

		virtual ~StateHandshake ();

	private:

		bool OnClientID (const std::string &ID);
	};
}

#endif // __PHOENIX_SERVICES_DATABASED_SERVER_STATE_HANDSHAKE_H__
