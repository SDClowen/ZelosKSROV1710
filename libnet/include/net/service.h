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

#ifndef __ZELOS_LIBNET_SERVICE_H__
#define __ZELOS_LIBNET_SERVICE_H__

#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class IService
{
public:

    IService ();

    virtual ~IService ();

    bool initialize (const std::map<std::string,std::string> &entries);

    bool run ();

    void stop ();

    void setClientState (const uint32_t connID, const int type, const bool state);

    void displayClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error);

    virtual const std::string& getLastError () const = 0;

private:

    void OnStop ();

    virtual bool OnInitialize () = 0;

    virtual void OnConfigure (const std::map<std::string,std::string> &entries) = 0;

    virtual void OnClientState (const uint32_t connID, const int type, const bool state) = 0;

    virtual void OnClientError (const uint32_t connID, const int type,
                                const boost::system::error_code &error) = 0;

    virtual void OnRelease () = 0;

protected:

    boost::asio::io_service m_io_service;

    int m_thread_count;
    boost::thread_group m_threads;
};

#endif // SERVICE_H
