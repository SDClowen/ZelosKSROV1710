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

#include <net/connection.h>

#include "packet_certify.h"
#include "server.h"

Service::Service ()
    : m_server(new Server(this,m_io_service))
{
}

Service::~Service ()
{
}

const std::string& Service::getLastError () const
{
    return m_server->GetLastError();
}

void Service::certify (const uint32_t connID)
{
    m_io_service.post(boost::bind(&Service::OnCertify,this,connID));
}

bool Service::OnInitialize ()
{
    if (!m_server || !m_server->initialize())
        return false;

    m_server->Execute();

    return true;
}

void Service::OnConfigure (const std::map<std::string, std::string> &entries)
{
    if (m_server)
        m_server->configure(entries);
}

void Service::OnClientState (const uint32_t connID, const int type, const bool state)
{
}

void Service::OnClientError (const uint32_t connID, const int type,
                             const boost::system::error_code &error)
{
}

void Service::OnRelease ()
{
    m_server->Stop();
}

void Service::OnCertify (const uint32_t connID)
{
    boost::shared_ptr<IConnection> conn = m_server->findConnection(connID);

    if (conn)
    {
        OPacket pkt;
        srv_pkt::CertifyDaemon(&pkt,ANSWER_ACCEPT);
        conn->send(&pkt);
    }
}
