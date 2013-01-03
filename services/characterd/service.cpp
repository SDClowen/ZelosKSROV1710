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

#include "service.h"

#include <auth/authentication_sro.h>
#include <net/client/connection.h>

#include "named/states.h"
#include "named/state_handshake.h"
#include "named/state_name.h"
#include "server/server.h"
#include "manager.h"

#define RECONNECT_TIME 5

enum CLIENT_SERVICES
{
    CLIENT_NAMED
};

Service::Service ()
    : m_name_state(false),
    m_name_timer(m_io_service),
    m_char_service(new Manager(m_io_service)),
    m_server(new Server(m_char_service,m_io_service))
{
}

Service::~Service ()
{
}

const std::string& Service::getLastError () const
{
    if (m_error.empty() && m_server)
        return m_server->GetLastError();

    return m_error;
}

const std::string& Service::getHost () const
{
    return m_host;
}

const std::string& Service::getPort () const
{
    return m_port;
}

boost::shared_ptr<Manager> Service::getCharService () const
{
    return m_char_service;
}

bool Service::initializeName ()
{
    m_name_client.reset(new clt::Connection(CLIENT_NAMED,this,m_io_service));

    if (!m_name_client)
        return false;

    boost::shared_ptr<SROAuth> auth;
    boost::shared_ptr<IConnection::State> state;

    auth.reset(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_name_client->setAuthentication(auth);

    state.reset(new named::StateName(this,m_name_client));

    if (!state)
        return false;

    m_name_client->addState(state,named::STATE_NAME);

    state.reset(new named::StateHandshake(this,m_name_client));

    if (!state)
        return false;

    m_name_client->addState(state,named::STATE_HANDSHAKE);

    m_name_client->setState(named::STATE_HANDSHAKE);

    return true;
}

bool Service::OnInitialize ()
{
    if (m_host.empty())
    {
        return false;
    }

    if (m_port.empty())
    {
        return false;
    }

    if (m_name_host.empty())
    {
        return false;
    }

    if (m_name_port.empty())
    {
        return false;
    }

    if (!m_char_service)
    {
        m_error = "Failed to initialize character service.";
        return false;
    }

    if (!m_char_service->initialize())
    {
        m_error = m_char_service->getError();
        return false;
    }

    if (!m_server || !m_server->initialize())
    {
        m_error = "Failed to initialize server.";
        return false;
    }

    if (!initializeName())
    {
        m_error = "Failed to initialize name client";
        return false;
    }

    syslog(LOG_INFO,"Connecting to name service.");

    m_name_client->connect(m_name_host,m_name_port);

    syslog(LOG_INFO,"Launching server.");

    m_server->Execute();

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    if (m_server)
        m_server->configure(entries);

    bool createCH = false, createEU = false;
    std::string dbHost, dbName, dbUsr, dbPw,  mediaPath;

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "Host")
            m_host = it->second;
        else if (it->first == "Port")
            m_port = it->second;
        else if (it->first == "NameHost")
            m_name_host = it->second;
        else if (it->first == "NamePort")
            m_name_port = it->second;
        else if (it->first == "DatabaseIP")
            dbHost = it->second;
        else if (it->first == "DatabaseName")
            dbName = it->second;
        else if (it->first == "DatabaseUser")
            dbUsr = it->second;
        else if (it->first == "DatabasePassword")
            dbPw = it->second;
        else if (it->first == "WorldAllowChinese")
            createCH = atoi(it->second.c_str());
        else if (it->first == "WorldAllowEuropean")
            createEU = atoi(it->second.c_str());
        else if (it->first == "MediaPath")
            mediaPath = it->second;
    }

    if (m_char_service)
    {
        m_char_service->setHost(dbHost);
        m_char_service->setDatabase(dbName);
        m_char_service->setUser(dbUsr,dbPw);
        m_char_service->setCharRules(createCH,createEU);
        m_char_service->setMediaPath(mediaPath);
    }
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    assert(type == CLIENT_NAMED);

    if (__sync_bool_compare_and_swap(&m_name_state,!state,state))
    {
        syslog(LOG_INFO,"Connection with name server %s.",state ? "established" : "terminated");

        if (!state)
        {
            m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_name_timer.async_wait(boost::bind(&Service::OnReconnect,this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    assert(type == CLIENT_NAMED);

    syslog(LOG_INFO,"Name client, %s",error.message().c_str());

    m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
    m_name_timer.async_wait(boost::bind(&Service::OnReconnect,this,
                                        boost::asio::placeholders::error));
}

void Service::OnRelease ()
{
    if (m_name_client)
        m_name_client->stop();

    if (m_server)
        m_server->Stop();

    if (m_char_service)
        m_char_service->release();
}

void Service::OnReconnect (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to name server.");

        if (initializeName())
        {
            m_name_client->connect(m_name_host,m_name_port);
        }
        else
        {
            m_name_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_name_timer.async_wait(boost::bind(&Service::OnReconnect,this,
                                                boost::asio::placeholders::error));
        }
    }
}
