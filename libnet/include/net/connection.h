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

#ifndef __ZELOS_LIBNET_CONNECTION_HPP__
#define __ZELOS_LIBNET_CONNECTION_HPP__

#include <stdint.h>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>

#include <packet/packet.h>

#include "net/connection_state.h"

class Authentication;

class OPacket;
class PacketGroup;

class IMonitor;

class IConnection : public boost::enable_shared_from_this<IConnection>
{
public:

    typedef ConnectionState<IConnection> State;

    typedef boost::asio::ip::tcp::socket socket_type;
    typedef boost::asio::ip::tcp::resolver resolver_type;
    typedef boost::asio::ip::tcp::resolver::iterator resolver_iterator_type;

    typedef std::map<uint8_t,boost::shared_ptr<State> >::iterator state_iterator;
    typedef std::map<uint8_t,boost::shared_ptr<State> >::const_iterator state_const_iterator;

    typedef std::list<std::vector<uint8_t> >::iterator buffer_iterator;
    typedef std::list<std::vector<uint8_t> >::const_iterator buffer_const_iterator;

    IConnection (const uint32_t connID, const uint32_t timeout, const uint32_t maxPayload,
                 boost::asio::io_service &io_service);

    virtual ~IConnection ();

    uint32_t ID () const;

    socket_type& socket ();

    std::string localEndpoint () const;

    std::string remoteEndpoint () const;

    void listen ();

    void connect (const std::string &host, const std::string &port);

    void stop ();

    void setAuthentication (const boost::shared_ptr<Authentication> &auth);

    boost::shared_ptr<Authentication> getAuthentication () const;

    void addMonitor (const boost::shared_ptr<IMonitor> &monitor);

    void removeMonitor (const int type);

    bool setState (const uint8_t ID);

    void addState (const boost::shared_ptr<State> &state, const uint8_t stateID);

    uint8_t getCurrentState () const;

    void send (OPacket *pkt, bool encrypt = false);

    void send (PacketGroup *group, bool encrypt = false);

private:

    void OnResolve (const boost::system::error_code &error,
        resolver_iterator_type endpoint_iterator);

    void OnConnect (const boost::system::error_code &error,
        resolver_iterator_type endpoint_iterator);

    void OnRead (const boost::system::error_code& error, std::size_t bytes_transferred);

    void OnProcess (const IPacket packet);

    void OnWrite (std::size_t bytes_transferred, const boost::system::error_code& error);

    void OnTimeout (const boost::system::error_code &error);

    void OnStop ();

    virtual void OnListen () = 0;

    virtual void OnCleanup () = 0;

    virtual void OnError (const boost::system::error_code &error) = 0;

protected:

    uint32_t m_ID;
    uint8_t m_current_state;

    size_t m_read_offset;
    uint8_t m_buffer[2*IPacket::BODY_MAX_LENGTH];

    size_t m_write_offset;
    std::list<std::vector<uint8_t> > m_write_buffers;

    boost::shared_ptr<Authentication> m_Auth;

    boost::shared_ptr<State> m_State;
    std::map<uint8_t,boost::shared_ptr<State> > m_state_table;

    volatile int m_idle;
    volatile int m_closed;

    boost::asio::io_service &m_io_service;
    boost::asio::io_service::strand m_strand;

    socket_type m_socket;
    resolver_type m_resolver;
    boost::asio::deadline_timer m_idle_timer;

    boost::mutex m_mutex;

    std::string m_LocalIP;
    std::string m_RemoteIP;

    uint32_t m_timeout;
    uint32_t m_max_payload;

    boost::mutex m_monitor_mutex;
    std::vector<boost::shared_ptr<IMonitor> > m_Monitors;
};

#endif // __ZELOS_LIBNET_CONNECTION_HPP__
