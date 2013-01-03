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

#include "state_certify.h"

#include <net/connection.h>
#include <net/opcode/cert_clt.h>

#include "service.h"

StateCertify::StateCertify (Service *service, const boost::shared_ptr<IConnection> &conn)
    : ConnectionState<IConnection>::ConnectionState(conn),
    m_service(service)
{
    m_opcode_table[CLIENT_CERTIFY_DAEMON] = boost::bind(&StateCertify::OnCertify,this,_1);
}

StateCertify::~StateCertify ()
{
}

void StateCertify::Close ()
{
}

int StateCertify::OnCertify (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->certify(m_connection->ID());

    return MSG_SUCCESS;
}
