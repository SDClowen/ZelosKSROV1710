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

#ifndef __ZELOS_LIBNET_SERVER_INTERFACE_HPP__
#define __ZELOS_LIBNET_SERVER_INTERFACE_HPP__

#include <map>
#include <string>
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/thread/shared_mutex.hpp>

class OPacket;
class PacketGroup;

class IConnection;

class IServer
{
public:

    typedef boost::shared_mutex mutex_type;

    typedef std::map<uint32_t,boost::shared_ptr<IConnection> >::iterator conn_iterator;
    typedef std::map<uint32_t,boost::shared_ptr<IConnection> >::const_iterator conn_const_iterator;

    IServer (boost::asio::io_service &io_service);

    virtual ~IServer ();

    void configure (const std::map<std::string,std::string> &entries);

    bool initialize ();

    bool Execute ();

    void Stop ();

    const std::string& GetLastError () const;

    uint8_t SecurityMode () const;

    uint32_t ConnectionTimeout () const;

    uint32_t ClientVersion () const;

    uint8_t Locale () const;

    void RemoveConnection (const uint32_t connID, const uint8_t state);

    const std::string& IDS () const;

    void send (OPacket *pkt, const uint32_t connID);

    void send (PacketGroup *group, const uint32_t connID);

    boost::shared_ptr<IConnection> findConnection (const uint32_t connectionID) const;

private:

    virtual bool OnInitialize () = 0;

    virtual void OnConfigure (const std::map<std::string,std::string> &entries) = 0;

    virtual void OnRemoveConnection (const uint32_t connID, const uint8_t state) = 0;

    virtual void OnRelease () = 0;

    virtual boost::shared_ptr<IConnection> CreateConnection () = 0;

private:

    void OnStop ();

    void OnAccept (const boost::system::error_code &error);

protected:

    uint32_t m_port;
    uint32_t m_counter;
    std::string m_last_error;

    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;

    boost::shared_ptr<IConnection> m_pending_conn;
    std::map<uint32_t,boost::shared_ptr<IConnection> > m_conn_list;

    uint8_t m_locale;
    uint32_t m_client_version;
    uint8_t m_security_mode;
    uint32_t m_connection_timeout;
    uint32_t m_connection_max_count;

    std::string m_IDS;
    std::string m_LogPath;

    mutable mutex_type m_conn_mutex;
};

#endif //__ZELOS_LIBNET_SERVER_INTERFACE_HPP__
