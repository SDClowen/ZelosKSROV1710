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

#include "net/client/connection.h"

#include "net/service.h"

#define MAX_PAYLOAD 4096
#define TIMEOUT_PING 5

namespace clt
{

Connection::Connection (const int type, IService *service, boost::asio::io_service &io_service)
    : IConnection(0,0,MAX_PAYLOAD,io_service),
    m_type(type),
    m_service(service),
    m_ping_timer(io_service)
{
}

Connection::~Connection ()
{
}

void Connection::KeepAlive ()
{
    m_ping_timer.expires_from_now(boost::posix_time::seconds(TIMEOUT_PING));
    m_ping_timer.async_wait(boost::bind(&Connection::OnKeepAlive,this,boost::asio::placeholders::error));
}

void Connection::OnListen ()
{
    if (m_service)
        m_service->setClientState(m_ID,m_type,true);
}

void Connection::OnCleanup ()
{
    if (m_service)
        m_service->setClientState(m_ID,m_type,false);

    m_ping_timer.cancel();
}

void Connection::OnError (const boost::system::error_code &error)
{
    if (m_service)
        m_service->displayClientError(m_ID,m_type,error);
    else
        throw boost::system::system_error(error);
}

void Connection::OnKeepAlive (const boost::system::error_code &error)
{
    if (!error)
    {
        OPacket pkt(0x2002);
        send(&pkt);

        m_ping_timer.expires_from_now(boost::posix_time::seconds(TIMEOUT_PING));
        m_ping_timer.async_wait(boost::bind(&Connection::OnKeepAlive,this,boost::asio::placeholders::error));
    }
}

}

