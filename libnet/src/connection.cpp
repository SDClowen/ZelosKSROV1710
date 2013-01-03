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

#include "net/connection.h"

#include <utility>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>

#include <auth/authentication.h>
#include <packet/packet.h>
#include <packet/packet_group.h>

#include "net/connection_state.h"
#include "net/monitor.h"

IConnection::IConnection (const uint32_t connID, const uint32_t timeout, const uint32_t maxPayload,
                          boost::asio::io_service &io_service)
    :   m_ID(connID),
        m_current_state(0),
        m_read_offset(0),
        m_write_offset(0),
        m_idle(false),
        m_closed(false),
        m_io_service(io_service),
        m_strand(io_service),
        m_socket(io_service),
        m_resolver(io_service),
        m_idle_timer(io_service),
        m_timeout(timeout),
        m_max_payload(maxPayload)
{
    memset(m_buffer,0,2*IPacket::BODY_MAX_LENGTH);
}

IConnection::~IConnection ()
{
}

uint32_t IConnection::ID () const
{
    return m_ID;
}

IConnection::socket_type& IConnection::socket ()
{
    return m_socket;
}

std::string IConnection::localEndpoint () const
{
    return m_LocalIP;
}

std::string IConnection::remoteEndpoint () const
{
    return m_RemoteIP;
}

void IConnection::listen ()
{
    assert(m_Auth);

    /// Cache Local and Remote IP.

    boost::asio::ip::tcp::no_delay option(true);

    m_socket.set_option(option);

    m_LocalIP = m_socket.local_endpoint().address().to_string();

    m_RemoteIP = m_socket.remote_endpoint().address().to_string();

    m_socket.async_read_some(boost::asio::buffer(&m_buffer,2*IPacket::BODY_MAX_LENGTH),
                             boost::bind(&IConnection::OnRead,shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));

    if (m_timeout)
    {
        m_idle_timer.expires_from_now(boost::posix_time::seconds(m_timeout));
        m_idle_timer.async_wait(boost::bind(&IConnection::OnTimeout,
                                            shared_from_this(),
                                            boost::asio::placeholders::error));
    }

    OnListen();
}

void IConnection::connect (const std::string &host, const std::string &port)
{
    assert(m_Auth);

    boost::asio::ip::tcp::resolver::query query(host,port);

    m_resolver.async_resolve(query,boost::bind(&IConnection::OnResolve,
                                               shared_from_this(),
                                               boost::asio::placeholders::error,
                                               m_resolver.resolve(query)));
}

void IConnection::stop ()
{
    m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
}

void IConnection::setAuthentication (const boost::shared_ptr<Authentication> &auth)
{
    m_Auth = auth;
}

boost::shared_ptr<Authentication> IConnection::getAuthentication () const
{
    return m_Auth;
}


void IConnection::addMonitor (const boost::shared_ptr<IMonitor> &monitor)
{
    boost::lock_guard<boost::mutex> lock(m_monitor_mutex);

    std::vector<boost::shared_ptr<IMonitor> >::iterator it;

    for (it = m_Monitors.begin(); it != m_Monitors.end(); ++it)
    {
        if ((*it)->getType() == monitor->getType())
        {
            m_Monitors.erase(it);
            break;
        }
    }

    m_Monitors.push_back(monitor);
}

void IConnection::removeMonitor (int type)
{
    boost::lock_guard<boost::mutex> lock(m_monitor_mutex);

    std::vector<boost::shared_ptr<IMonitor> >::iterator it;

    for (it = m_Monitors.begin(); it != m_Monitors.end(); ++it)
    {
        if ((*it)->getType() == type)
        {
            m_Monitors.erase(it);
            break;
        }
    }
}

bool IConnection::setState (const uint8_t stateID)
{
    state_iterator i = m_state_table.find(stateID);

    if (i == m_state_table.end())
        return false;

    m_current_state = stateID;
    m_State = i->second;

    return true;
}

void IConnection::addState (const boost::shared_ptr<State> &state, const uint8_t stateID)
{
    m_state_table.insert(std::make_pair(stateID,state));
}

uint8_t IConnection::getCurrentState () const
{
    return m_current_state;
}

void IConnection::send (OPacket *packet, bool encrypt)
{
    if (!__sync_fetch_and_add(&m_closed,0))
    {
        packet->WriteSize(encrypt);

        std::vector<uint8_t> buffer;
        m_Auth->encode(packet,encrypt,buffer);

        try
        {
            boost::lock_guard<boost::mutex> lock(m_mutex);

            m_write_buffers.push_back(buffer);

            if (!m_write_offset)
            {
                std::vector<boost::asio::const_buffer> buffers;

                std::list<std::vector<uint8_t> >::const_reverse_iterator it;
                for (it = m_write_buffers.rbegin(); it != m_write_buffers.rend(); ++it)
                    buffers.push_back(boost::asio::buffer(*it));

                m_socket.async_write_some(buffers,boost::bind(&IConnection::OnWrite,shared_from_this(),
                                                      boost::asio::placeholders::bytes_transferred,
                                                      boost::asio::placeholders::error));
            }

            m_write_offset += buffer.size();

            boost::lock_guard<boost::mutex> mlock(m_monitor_mutex);

            std::for_each(m_Monitors.begin(),m_Monitors.end(),
                          boost::bind(&IMonitor::Notify,_1,buffer.data(),buffer.size()));
        }
        catch (const boost::lock_error &error)
        {
            syslog(LOG_ERR,"srv::IConnection::Send - %s, Error code = %i",error.what(),
                   error.native_error());
        }
    }
}

void IConnection::send (PacketGroup *group, bool encrypt)
{
    if (!__sync_fetch_and_add(&m_closed,0))
    {
        size_t totalSize = 0;

        OPacket *packet;
        std::vector<uint8_t> buffer;
        std::list<std::vector<uint8_t> > packets;

        for (std::vector<OPacket>::iterator it = group->packets.begin(); it != group->packets.end(); ++it)
        {
            it->WriteSize(encrypt);

            packet = &(*it);

            m_Auth->encode(packet,encrypt,buffer);

            packets.push_back(buffer);

            totalSize += buffer.size();

            buffer.clear();
        }

        try
        {
            boost::lock_guard<boost::mutex> lock(m_mutex);

            m_write_buffers.splice(m_write_buffers.end(),packets);

            if (!m_write_offset)
            {
                std::vector<boost::asio::const_buffer> buffers;

                std::list<std::vector<uint8_t> >::const_reverse_iterator it;
                for (it = m_write_buffers.rbegin(); it != m_write_buffers.rend(); ++it)
                    buffers.push_back(boost::asio::buffer(*it));

                m_socket.async_write_some(buffers,boost::bind(&IConnection::OnWrite,shared_from_this(),
                                                      boost::asio::placeholders::bytes_transferred,
                                                      boost::asio::placeholders::error));
            }

            m_write_offset += totalSize;

            boost::lock_guard<boost::mutex> mlock(m_monitor_mutex);

            for ( std::list<std::vector<uint8_t> >::const_iterator liter = packets.begin();
                liter != packets.end(); ++liter)
            {
                std::for_each(m_Monitors.begin(),m_Monitors.end(),
                              boost::bind(&IMonitor::Notify,_1,liter->data(),liter->size()));
            }
        }
        catch (const boost::lock_error &error)
        {
            syslog(LOG_ERR,"srv::IConnection::Send - %s, Error code = %i",error.what(),
                   error.native_error());
        }
    }
}

void IConnection::OnResolve (const boost::system::error_code &error,
                          resolver_iterator_type endpoint_iterator)
{
    if (!error)
    {
        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
        m_socket.async_connect(endpoint, boost::bind(&IConnection::OnConnect, shared_from_this(),
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
        OnError(error);
}

void IConnection::OnConnect (const boost::system::error_code &error,
                          resolver_iterator_type endpoint_iterator)
{
    if (!error)
    {
        listen();
    }
    else if (endpoint_iterator != resolver_iterator_type())
    {
      // The connection failed. Try the next endpoint in the list.
      m_socket.close();
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
      m_socket.async_connect(endpoint, boost::bind(&IConnection::OnConnect, shared_from_this(),
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
        OnError(error);
}

void IConnection::OnRead (const boost::system::error_code &error, std::size_t bytes_transferred)
{
    enum { CLIENT_PING = 0x2002 };

    if (__sync_fetch_and_add(&m_closed,0))
        return;

    if (!error)
    {
        m_read_offset += bytes_transferred;

        size_t offset = 0;

        while (m_read_offset >= IPacket::HEADER_LENGTH )
        {
            bool hasSecurity = false;
            size_t real_size = 0;
            size_t buffer_size = 0;

            DECODE_ERROR ret = m_Auth->readHeader(m_buffer+offset,m_read_offset,m_max_payload,
                                                  &buffer_size,&real_size,&hasSecurity);

            if (ret != ERR_DECODE_SUCCESS)
            {
                m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
                return;
            }

            IPacket packet(m_buffer+offset,buffer_size);

            ret = m_Auth->decode(&packet,buffer_size,real_size,hasSecurity);

            if (ret != ERR_DECODE_SUCCESS)
            {
                m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
                return;
            }

            __sync_lock_test_and_set(&m_idle,false);

            if (packet.opcode() != CLIENT_PING)
            {
                m_io_service.post(m_strand.wrap(boost::bind(&IConnection::OnProcess,
                                                            shared_from_this(),
                                                            packet)));
            }

            m_read_offset -= buffer_size;
            offset += buffer_size;
        }

        if (m_read_offset)
            memmove(m_buffer,m_buffer+offset,m_read_offset);

        try
        {
            boost::lock_guard<boost::mutex> lock(m_mutex);

            m_socket.async_read_some(boost::asio::buffer(m_buffer+m_read_offset,
                                    2*IPacket::BODY_MAX_LENGTH-m_read_offset),
                                    boost::bind(&IConnection::OnRead,
                                                shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        catch (const boost::lock_error &err)
        {
            syslog(LOG_ERR,"srv::IConnection::OnRead - %s, Error code = %i",err.what(),err.native_error());
        }
    }
    else
    {
        if (error != boost::asio::error::operation_aborted)
            m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
    }
}

void IConnection::OnProcess (const IPacket packet)
{
    if(__sync_fetch_and_add(&m_closed,0))
        return;

    syslog(LOG_INFO,"ConnID = %i, Opcode = %X",m_ID,packet.opcode());

    assert(m_State);

    if (m_State)
        m_State->Parse(packet);

    boost::lock_guard<boost::mutex> mlock(m_monitor_mutex);

    std::for_each(m_Monitors.begin(),m_Monitors.end(),
                  boost::bind(&IMonitor::Notify,_1,packet.buffer(),packet.size()));
}

void IConnection::OnWrite (std::size_t bytes_transferred, const boost::system::error_code& error)
{
    if (!error)
    {
        try
        {
            if (!__sync_fetch_and_add(&m_closed,0))
            {
                boost::lock_guard<boost::mutex> lock(m_mutex);

                std::list<std::vector<uint8_t> >::iterator iter = m_write_buffers.begin();

                while (iter != m_write_buffers.end() && iter->size() <= bytes_transferred)
                {
                    m_write_offset -= iter->size();
                    bytes_transferred -= iter->size();

                    iter = m_write_buffers.erase(iter);
                }

                if (m_write_offset)
                {
                    size_t offset = 0;
                    std::list<boost::asio::const_buffer> buffers;

                    for (; iter != m_write_buffers.end(); ++iter)
                    {
                        offset = bytes_transferred ? iter->size()-bytes_transferred : 0;

                        buffers.push_back(boost::asio::buffer(iter->data()+bytes_transferred,
                                                              iter->size()-offset));

                        bytes_transferred = 0;
                    }

                    m_socket.async_write_some(buffers,boost::bind(&IConnection::OnWrite,
                                                            shared_from_this(),
                                                            boost::asio::placeholders::bytes_transferred,
                                                            boost::asio::placeholders::error));
                }
            }
        }
        catch (const boost::lock_error &err)
        {
            syslog(LOG_ERR,"srv::IConnection::OnWrite - %s, Error code = %i",err.what(),
                   err.native_error());
        }
    }
    else
    {
        m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
    }
}

void IConnection::OnTimeout (const boost::system::error_code &error)
{
    if (!error)
    {
        if (__sync_fetch_and_add(&m_idle,0))
        {
            m_io_service.post(boost::bind(&IConnection::OnStop,shared_from_this()));
        }
        else
        {
            __sync_lock_test_and_set(&m_idle,true);

            m_idle_timer.expires_from_now(boost::posix_time::seconds(m_timeout));
            m_idle_timer.async_wait(boost::bind(&IConnection::OnTimeout,
                                                shared_from_this(),
                                                boost::asio::placeholders::error));
        }
    }
}

void IConnection::OnStop ()
{
    try
    {
        if (!__sync_fetch_and_add(&m_closed,0))
        {
            __sync_lock_test_and_set(&m_closed,true);

            OnCleanup();

            boost::unique_lock<boost::mutex> lock(m_mutex);

            try
            {
                m_socket.cancel();

                m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

                m_socket.close();

                m_idle_timer.cancel();
            }
            catch (...)
            {
            }

            m_state_table.clear();

            if (m_State)
            {
                boost::shared_ptr<State> state = m_State;

                m_State.reset();

                lock.unlock();

                state->Close();
            }
        }
    }
    catch (const boost::lock_error &error)
    {
        syslog(LOG_ERR,"srv::IConnection::Stop - %s, Error code = %i",error.what(),error.native_error());
    }
}

