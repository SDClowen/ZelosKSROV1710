#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>

#include <net/utility.h>

#include "service.h"

#define DAEMON "databased"
#define PID_FILE "/var/run/databased.pid"

boost::shared_ptr<Service> service;

void signal_handler (int sig)
{
	switch(sig)
	{
	case SIGHUP:
		syslog(LOG_WARNING,"Received SIGHUP signal.");
		break;
	case SIGTERM:
		syslog(LOG_WARNING,"Received SIGTERM signal.");
		break;
	default:
		syslog(LOG_WARNING,"Unhandled signal (%d) %s",sig,strsignal(sig));
		break;
	}

	if (service)
		service->stop();
}

void print_usage ()
{
	std::cerr << "Usage: <filename>" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		print_usage();
		return 1;
	}

	std::string filename(argv[1]);

	std::map<std::string,std::string> config_entries;

	if (!read_config_file(filename,config_entries))
		return 1;

	signal(SIGHUP,signal_handler);
	signal(SIGTERM,signal_handler);
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,signal_handler);

#if defined DEBUG
	setlogmask(LOG_UPTO(LOG_DEBUG));
	openlog(DAEMON,LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
#else
	setlogmask(LOG_UPTO(LOG_INFO));
	openlog(DAEMON,LOG_CONS,LOG_USER);
#endif

	syslog(LOG_INFO,"%s daemon starting up",DAEMON);

	try
	{
		service.reset(new Service());

		if (!service)
			return 1;

		if (!service->initialize(config_entries) || !service->run())
		{
			syslog(LOG_INFO,"Server Exception: %s",service->getLastError().c_str());
			return 1;
		}
	}
	catch(const std::exception& e)
	{
		syslog(LOG_INFO,"Server Exception: %s",e.what());
		return 1;
	}
	catch(...)
	{
		syslog(LOG_INFO,"Server Exception: Unhandled error.");
		return 1;
	}

	return 0;
}
