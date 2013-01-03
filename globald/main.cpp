/*********************************************************************************
 *
 * This file is part of ZelosOnline.
 *
 * ZelosOnline is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ZelosOnline is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright  2013  Rafael Dominguez (npcdoom)
 *
 *********************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>

#include "service.h"

#define DAEMON "globald"
#define PID_FILE "/var/run/globald.pid"

boost::shared_ptr<Service> service;

void print_usage ()
{
    std::cerr << "Usage: <filename>" << std::endl;
}

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

bool read_config_file (const std::string &filename, std::map<std::string,std::string> &entries);

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

    syslog(LOG_INFO,"%s daemon starting up",DAEMON);

#if defined DEBUG
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog(DAEMON,LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
#else
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog(DAEMON,LOG_CONS,LOG_USER);
#endif

    try
    {
        srand(time(NULL));

        service.reset(new Service());

        if (!service)
            return 1;

        if (!service->initialize(config_entries) || !service->run())
        {
            syslog(LOG_INFO,"Server Error: %s",service->getLastError().c_str());
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

bool read_config_file (const std::string &filename, std::map<std::string,std::string> &entries)
{
    std::ifstream file(filename.c_str());

    if (!file)
    {
        std::cerr << "File " << filename << " dont exists." << std::endl;
        return false;
    }

    size_t i = 0;
    std::string line;

    while (!file.eof())
    {
        std::getline(file,line);

        if (!line.empty())
        {
            ///A LINE BEGGINING WITH A # IS A COMMENT

            if (*line.begin() != '#')
            {
                ///TRIM WHITESPACES
                line.erase(std::remove(line.begin(),line.end(),' '),line.end());

                size_t pos = line.find_first_of('=');

                if (pos == std::string::npos)
                {
                    std::cerr << "Invalid option in line " << i << ", " << line << std::endl;
                    return false;
                }

                entries.insert(std::make_pair(line.substr(0,pos),line.substr(pos+1)));
            }
        }

        ++i;
    }

    return true;
}
