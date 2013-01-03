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

#include "state_register.h"

#include <net/connection.h>
#include <net/opcode/name_clt.h>

#include "name_service.h"
#include "service.h"

namespace srv
{

StateRegister::StateRegister (Service *service, const boost::shared_ptr<IConnection> &conn)
    : ConnectionState<IConnection>::ConnectionState(conn),
    m_service(service)
{
    m_opcode_table[CLT_REGISTER_SERVICE] = boost::bind(&StateRegister::OnRegister,this,_1);
    m_opcode_table[CLT_UNREGISTER_SERVICE] = boost::bind(&StateRegister::OnUnregister,this,_1);
    m_opcode_table[CLT_QUERY] = boost::bind(&StateRegister::OnQuery,this,_1);
}

StateRegister::~StateRegister ()
{
}

void StateRegister::Close ()
{
    m_service->removeConnection(m_connection->ID());
}

int StateRegister::OnRegister (const IPacket &packet)
{
    NameService service;

    service.type = packet.Read<uint8_t>();

    service.host = packet.Read();

    service.port = packet.Read();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    service.connID = m_connection->ID();

    m_service->addService(service,m_connection);

    return MSG_SUCCESS;
}

int StateRegister::OnUnregister (const IPacket &packet)
{
    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->removeService(m_connection->ID());

    return MSG_SUCCESS;
}

int StateRegister::OnQuery (const IPacket &packet)
{
    uint8_t type = packet.Read<uint8_t>();

    if (!packet.EndOfStream())
        return MSG_ERROR_SIZE;

    m_service->queryService(type,m_connection);

    return MSG_SUCCESS;
}

}
