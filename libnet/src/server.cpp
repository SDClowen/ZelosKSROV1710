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

#include "net/server.h"

#include <boost/bind.hpp>

#include "net/connection.h"

IServer::IServer (boost::asio::io_service &io_service)
    :   m_port(0),
        m_counter(0),
        m_io_service(io_service),
        m_acceptor(m_io_service),
        m_locale(0),
        m_client_version(0),
        m_security_mode(0),
        m_connection_timeout(5),
        m_connection_max_count(10)
{
}

IServer::~IServer ()
{
}

uint8_t IServer::SecurityMode () const
{
    return m_security_mode;
}

uint32_t IServer::ConnectionTimeout () const
{
    return m_connection_timeout;
}

uint32_t IServer::ClientVersion () const
{
    return m_client_version;
}

uint8_t IServer::Locale() const
{
    return m_locale;
}

void IServer::RemoveConnection (const uint32_t connID, const uint8_t state)
{
    OnRemoveConnection(connID,state);

    boost::unique_lock<mutex_type> lock(m_conn_mutex);

    m_conn_list.erase(connID);
}

const std::string& IServer::IDS () const
{
    return m_IDS;
}

void IServer::configure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator conf_const_iterator;

    for (conf_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "Port")
            m_port = atoi(it->second.c_str());
        else if (it->first == "ConnectionTimeout")
            m_connection_timeout = atoi(it->second.c_str());
        else if (it->first == "ConnectionMax")
            m_connection_max_count = atoi(it->second.c_str());
        else if (it->first == "SecurityMode")
            m_security_mode = atoi(it->second.c_str());
        else if (it->first == "LogPath")
            m_LogPath = it->second;
    }

    OnConfigure(entries);
}

bool IServer::initialize ()
{
    try
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),m_port);

        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor.bind(endpoint);

        if(!OnInitialize())
            return false;
    }
    catch (const boost::system::system_error &error)
    {
        m_last_error = error.what();
        return false;
    }
    catch (const std::exception &error)
    {
        m_last_error = error.what();
        return false;
    }
    catch (...)
    {
        m_last_error = "Exception raised. Unhandled error.";
        return false;
    }

    return true;
}

bool IServer::Execute ()
{
    m_acceptor.listen();

    m_pending_conn = CreateConnection();

    m_acceptor.async_accept(m_pending_conn->socket(),boost::bind(&IServer::OnAccept,this,
        boost::asio::placeholders::error));

    return true;
}

void IServer::Stop ()
{
    m_io_service.post(boost::bind(&IServer::OnStop,this));
}

const std::string& IServer::GetLastError () const
{
    return m_last_error;
}

void IServer::send (OPacket *pkt, const uint32_t connID)
{
    boost::unique_lock<mutex_type> lock(m_conn_mutex);

    conn_iterator iter = m_conn_list.find(connID);

    if (iter != m_conn_list.end())
        iter->second->send(pkt);
}

void IServer::send (PacketGroup *group, const uint32_t connID)
{
    boost::unique_lock<mutex_type> lock(m_conn_mutex);

    conn_iterator iter = m_conn_list.find(connID);

    if (iter != m_conn_list.end())
        iter->second->send(group);
}

boost::shared_ptr<IConnection> IServer::findConnection (const uint32_t connectionID) const
{
    boost::shared_ptr<IConnection> conn;

    boost::unique_lock<mutex_type> lock(m_conn_mutex);

    conn_const_iterator iter = m_conn_list.find(connectionID);

    if (iter != m_conn_list.end())
        conn = iter->second;

    return conn;
}

void IServer::OnAccept (const boost::system::error_code &error)
{
    if (!error)
    {
        boost::unique_lock<mutex_type> lock(m_conn_mutex);

        m_conn_list.insert(std::make_pair(m_counter,m_pending_conn));

        lock.unlock();

        m_pending_conn->listen();

        m_pending_conn = CreateConnection();

        m_acceptor.async_accept(m_pending_conn->socket(), boost::bind(&IServer::OnAccept,this,
            boost::asio::placeholders::error));
    }
}

void IServer::OnStop ()
{
    boost::unique_lock<mutex_type> lock(m_conn_mutex);

    m_acceptor.close();

    for (conn_iterator it = m_conn_list.begin(); it != m_conn_list.end(); ++it)
        it->second->stop();

    m_conn_list.clear();

    lock.unlock();

    OnRelease();
}
