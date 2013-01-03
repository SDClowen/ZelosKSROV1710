#ifndef __PHOENIX_SERVICES_DATABASED_NAMED_STATE_HANDSHAKE_H__
#define __PHOENIX_SERVICES_DATABASED_NAMED_STATE_HANDSHAKE_H__

#include <net/client/state_handshake.h>

namespace named
{
	class StateHandshake : public clt::IStateHandshake
	{
	public:

		StateHandshake (const boost::shared_ptr<IConnection> &conn);

		virtual ~StateHandshake ();

	private:

		void OnConfig ();
	};
}

#endif //__PHOENIX_SERVICES_DATABASED_NAMED_STATE_HANDSHAKE_H__
