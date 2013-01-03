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

#include "certd/states.h"
#include "certd/state_certify.h"
#include "certd/state_handshake.h"
#include "globald/states.h"
#include "globald/state_global.h"
#include "globald/state_handshake.h"
#include "server/server.h"
#include "server/packet.h"

#define RECONNECT_TIME 5

enum CLIENT_SERVICE
{
    CLIENT_CERTD,
    CLIENT_GLOBALD
};

Service::Service ()
    : m_srv_state(false),
    m_server(new Server(this,m_io_service)),
    m_cert_timer(m_io_service),
    m_global_state(false),
    m_global_timer(m_io_service),
    m_service_strand(m_io_service)
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

void Service::runServer ()
{
    m_io_service.post(boost::bind(&Service::OnServer,this));
}

void Service::runGlobal ()
{
    m_io_service.post(boost::bind(&Service::OnGlobal,this));
}

void Service::addService (const NameService &service, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(m_service_strand.wrap(boost::bind(&Service::OnService,this,service,conn)));
}

void Service::queryService (const uint8_t type, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Service::OnQueryService,this,type,conn));
}

void Service::removeService (const uint32_t connID)
{
    m_io_service.post(m_service_strand.wrap(boost::bind(&Service::OnRemoveService,this,connID)));
}

void Service::removeConnection (const uint32_t connID)
{
    m_io_service.post(m_service_strand.wrap(boost::bind(&Service::OnRemoveService,this,connID)));

    m_io_service.post(boost::bind(&Service::OnRemoveQuery,this,connID));
}

bool Service::initializeCert ()
{
    m_cert_client.reset(new clt::Connection(CLIENT_CERTD,this,m_io_service));

    if (!m_cert_client)
        return false;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_cert_client->setAuthentication(auth);

    boost::shared_ptr<ConnectionState<IConnection> > state;

    state.reset(new certd::StateHandshake(this,"CertifyServer","NameServer",m_cert_client));

    m_cert_client->addState(state,certd::STATE_HANDSHAKE);

    state.reset(new certd::StateCertify(m_cert_client));

    m_cert_client->addState(state,certd::STATE_CERTIFY);

    m_cert_client->setState(certd::STATE_HANDSHAKE);

    return true;
}

bool Service::initializeGlobal ()
{
    m_global_client.reset(new clt::Connection(CLIENT_GLOBALD,this,m_io_service));

    if (!m_global_client)
        return false;

    boost::shared_ptr<SROAuth> auth(new SROAuth(AUTH_CLIENT,0));

    if (!auth)
        return false;

    m_global_client->setAuthentication(auth);

    boost::shared_ptr<ConnectionState<IConnection> > state;

    state.reset(new globald::StateHandshake(this,"ManagerServer","NameServer",m_global_client));

    m_global_client->addState(state,globald::STATE_HANDSHAKE);

    state.reset(new globald::StateGlobal(m_global_client));

    m_global_client->addState(state,globald::STATE_GLOBAL);

    m_global_client->setState(globald::STATE_HANDSHAKE);

    return true;
}

bool Service::OnInitialize ()
{
    if (!m_server || !m_server->initialize())
        return false;

    if (!initializeCert())
    {
        m_error = "Failed to initialize certify client.";
        return false;
    }

    if (!initializeGlobal())
    {
        m_error = "Failed to initialize global client.";
        return false;
    }

    m_cert_client->connect(m_cert_host,m_cert_port);

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    typedef std::map<std::string,std::string>::const_iterator cfg_const_iterator;

    if (m_server)
        m_server->configure(entries);

    for (cfg_const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->first == "CertifyHost")
            m_cert_host = it->second;
        else if (it->first == "CertifyPort")
            m_cert_port = it->second;
        else if (it->first == "GlobalHost")
            m_global_host = it->second;
        else if (it->first == "GlobalPort")
            m_global_port = it->second;
    }
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
    assert(type == CLIENT_CERTD || type == CLIENT_GLOBALD);

    if (type == CLIENT_CERTD)
    {
        syslog(LOG_INFO,"Connection with certify server has been %s.",state ? "established" : "terminated");

        if (!state)
        {
            m_cert_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_cert_timer.async_wait(boost::bind(&Service::OnReconnectCert,
                                                this,
                                                boost::asio::placeholders::error));
        }
    }
    else
    {
        if (__sync_bool_compare_and_swap(&m_global_state,!state,state))
        {
            syslog(LOG_INFO,"Connection with global server has been %s.",state ? "established" : "terminated");

            if (!state)
            {
                m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
                m_global_timer.async_wait(boost::bind(&Service::OnReconnectGlobal,
                                                      this,
                                                      boost::asio::placeholders::error));
            }
        }
    }
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
    assert(type == CLIENT_CERTD || type == CLIENT_GLOBALD);

    if (type == CLIENT_CERTD)
    {
        syslog(LOG_INFO,"Certify Client, %s",error.message().c_str());

        m_cert_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_cert_timer.async_wait(boost::bind(&Service::OnReconnectCert,
                                            this,
                                            boost::asio::placeholders::error));

    }
    else
    {
        syslog(LOG_INFO,"Global Client, %s",error.message().c_str());

        m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
        m_global_timer.async_wait(boost::bind(&Service::OnReconnectGlobal,
                                              this,
                                              boost::asio::placeholders::error));
    }
}

void Service::OnRelease ()
{
    if (m_cert_client)
        m_cert_client->stop();

    if (m_global_client)
        m_global_client->stop();

    if (m_server)
        m_server->Stop();
}

void Service::OnServer ()
{
    if (__sync_bool_compare_and_swap(&m_srv_state,false,true))
    {
        syslog(LOG_INFO,"Launching server.");

        m_server->Execute();
    }
}

void Service::OnGlobal ()
{
    if (!__sync_fetch_and_add(&m_global_state,0))
        m_global_client->connect(m_global_host,m_global_port);
}

void Service::OnService (const NameService &service, const boost::shared_ptr<IConnection> &conn)
{
    bool add = true;
    for (service_const_iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        if (iter->type == service.type)
        {
            add = false;
            break;
        }
    }

    if (add)
    {
        syslog(LOG_INFO,"Registering service %i - ID = %i",service.type,service.connID);
        m_services.push_back(service);

        m_io_service.post(boost::bind(&Service::OnCheckQuery,this,service));
    }

    OPacket pkt;
    srv_pkt::Register(&pkt,add ? ANSWER_ACCEPT : ANSWER_ERROR);
    conn->send(&pkt);
}

void Service::OnQueryService (const uint8_t type, const boost::shared_ptr<IConnection> &conn)
{
    NameService service;

    for (service_const_iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        if (iter->type == type)
        {
            service = *iter;
            break;
        }
    }

    if (service.type)
    {
        OPacket pkt;
        srv_pkt::Query(&pkt,&service);
        conn->send(&pkt);
    }
    else
    {
        boost::lock_guard<boost::mutex> lock(m_query_mutex);
        m_queries.push_back(std::make_pair(type,conn));
    }
}

void Service::OnCheckQuery (const NameService &service)
{
    boost::lock_guard<boost::mutex> qlock(m_query_mutex);

    OPacket pkt;
    srv_pkt::Query(&pkt,&service);

    for (std::vector<Query>::iterator it = m_queries.begin(); it != m_queries.end();)
    {
        if (it->first == service.type)
        {
            it->second->send(&pkt);

            it = m_queries.erase(it);
        }
        else
            ++it;
    }
}

void Service::OnRemoveQuery (const uint32_t connID)
{
    boost::lock_guard<boost::mutex> lock(m_query_mutex);

    for (std::vector<Query>::iterator it = m_queries.begin(); it != m_queries.end();)
    {
        if (it->second->ID() == connID)
            it = m_queries.erase(it);
        else
            ++it;
    }
}

void Service::OnRemoveService (const uint32_t connID)
{
    for (service_iterator iter = m_services.begin(); iter != m_services.end(); ++iter)
    {
        if (iter->connID == connID)
        {
            syslog(LOG_INFO,"Unregistering service %i - ID = %i",iter->type,connID);

            m_services.erase(iter);
            break;
        }
    }
}

void Service::OnReconnectCert (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to certify server.");

        if (initializeCert())
        {
            m_cert_client->connect(m_cert_host,m_cert_port);
        }
        else
        {
            m_cert_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_cert_timer.async_wait(boost::bind(&Service::OnReconnectCert,
                                                this,
                                                boost::asio::placeholders::error));
        }
    }
}

void Service::OnReconnectGlobal (const boost::system::error_code &error)
{
    if (!error)
    {
        syslog(LOG_INFO,"Trying to connect to global server.");

        if (initializeGlobal())
        {
            m_global_client->connect(m_global_host,m_global_port);
        }
        else
        {
            m_global_timer.expires_from_now(boost::posix_time::seconds(RECONNECT_TIME));
            m_global_timer.async_wait(boost::bind(&Service::OnReconnectGlobal,
                                                  this,
                                                  boost::asio::placeholders::error));
        }
    }
}

