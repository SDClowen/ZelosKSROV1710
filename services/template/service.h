#ifndef __PHOENIX_SERVICES_DATABASED_SERVICE_H__
#define __PHOENIX_SERVICES_DATABASED_SERVICE_H__

#include <net/service.h>

namespace clt
{
	class Connection;
}

class Server;

class Service : public IService
{
public:

	Service ();

	virtual ~Service ();

	const std::string& getLastError () const;

private:

	bool initializeName ();

private:

	bool OnInitialize ();

	void OnConfigure (const std::map<std::string, std::string> &entries);

	void OnRelease ();

private:

	std::string m_error;

	std::string m_host;
	std::string m_port;

	std::string m_name_host;
	std::string m_name_port;
	boost::shared_ptr<clt::Connection> m_name_client;

	boost::shared_ptr<Server> m_server;
};

#endif // __PHOENIX_SERVICES_DATABASED_SERVICE_H__
