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

#include "net/service.h"

IService::IService ()
    : m_thread_count(1)
{
}

IService::~IService ()
{
}

bool IService::initialize(const std::map<std::string, std::string> &entries)
{
    std::map<std::string, std::string>::const_iterator it = entries.find("NetworkPoolSize");

    if (it != entries.end())
        m_thread_count = atoi(it->second.c_str());

    OnConfigure(entries);

    if (!OnInitialize())
        return false;

    return true;
}

bool IService::run ()
{
    for (int i = 0; i < m_thread_count; ++i)
        m_threads.create_thread(boost::bind(&boost::asio::io_service::run,&m_io_service));

    m_threads.join_all();

    return true;
}

void IService::stop ()
{
    m_io_service.post(boost::bind(&IService::OnStop,this));
}

void IService::setClientState (const uint32_t connID, const int type, const bool state)
{
    m_io_service.post(boost::bind(&IService::OnClientState,this,connID,type,state));
}

void IService::displayClientError (const uint32_t connID, const int type,
                                   const boost::system::error_code &error)
{
    m_io_service.post(boost::bind(&IService::OnClientError,this,connID,type,error));
}

void IService::OnStop ()
{
    OnRelease();

    m_io_service.stop();
}
