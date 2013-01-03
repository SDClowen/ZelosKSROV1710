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

#ifndef MONITOR_LOGGER_H
#define MONITOR_LOGGER_H

#include "monitor.h"

#include <fstream>
#include <string>

class Logger : public IMonitor
{
public:

    Logger ()
    {
//        if (!m_Logfile.is_open())
//        {
//            char s[30];
//            char filename[200];
//            struct tm tim;
//            time_t now;
//            now = time(NULL);
//            tim = *(localtime(&now));
//            strftime(s,30,"%H%M%S%d%m%Y",&tim);
//
//			sprintf(filename,"%s/conn_%u_%s",path.c_str(),m_ID,s);
//
//            m_Logfile.open(filename,std::ios::binary | std::ios::app);
//        }
    }

    void Notify (const uint8_t *buffer, const size_t len)
    {
    }

private:

    std::string m_filename;

    std::ofstream m_Logfile;
};

#endif // MONITOR_LOGGER_H
