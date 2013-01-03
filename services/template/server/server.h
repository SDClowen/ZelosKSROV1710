#ifndef __PHOENIX_SERVICES_DATABASED_SERVER_SERVER_H__
#define __PHOENIX_SERVICES_DATABASED_SERVER_SERVER_H__

#include <net/server.h>

class Server : public IServer
{
public:

	Server (boost::asio::io_service &io_service);

	virtual ~Server ();

private:

	bool OnInitialize ();

	void OnConfigure (const std::map<std::string, std::string> &entries);

	void OnRemoveConnection (const uint32_t connID);

	void OnRelease ();

	boost::shared_ptr<IConnection> CreateConnection ();
};

#endif // __PHOENIX_SERVICES_DATABASED_SERVER_SERVER_H__
