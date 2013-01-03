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

#include "state_command.h"

#include <net/constants/lobby.h>
#include <net/opcode/chard_clt.h>

#include "manager.h"

namespace srv
{
    StateCommand::StateCommand (const boost::shared_ptr<Manager> &service,
                                const boost::shared_ptr<IConnection> &conn)
        : ConnectionState<IConnection>::ConnectionState(conn),
        m_service(service)
    {
        m_opcode_table[CLT_CHARD_LOBBY] = boost::bind(&StateCommand::OnLobby,this,_1);
        m_opcode_table[CLT_CHARD_SELECT] = boost::bind(&StateCommand::OnSelect,this,_1);
        m_opcode_table[CLT_CHARD_CHARACTER] = boost::bind(&StateCommand::OnCharacter,this,_1);
    }

    StateCommand::~StateCommand ()
    {
    }

    void StateCommand::Close ()
    {
    }

    int StateCommand::OnLobby (const IPacket &packet)
    {
        uint8_t action = packet.Read<uint8_t>();

        int ret;

        switch(action)
        {
        case pkt::LOBBY_ACTION_CREATE:
            ret = OnLobby_Create(packet);
            break;
        case pkt::LOBBY_ACTION_LIST:
            ret = OnLobby_List(packet);
            break;
        case pkt::LOBBY_ACTION_ERASE:
            ret = OnLobby_Erase(packet);
            break;
        case pkt::LOBBY_ACTION_VALIDATE_NAME:
            ret = OnLobby_ValidateName(packet);
            break;
        case pkt::LOBBY_ACTION_RESTORE:
            ret = OnLobby_Restore(packet);
            break;
        default:
            ret = MSG_ERROR_ARG;
            break;
        }

        return ret;
    }

    int StateCommand::OnLobby_Create (const IPacket &packet)
    {
        std::string name = packet.Read();

        uint32_t refCharID = packet.Read<uint32_t>();

        uint8_t scale = packet.Read<uint8_t>();

        uint32_t items[MAX_BASE_ITEMS];

        for (int i = 0; i < MAX_BASE_ITEMS; ++i)
            items[i] = packet.Read<uint32_t>();

        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Create(name,refCharID,scale,items,accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnLobby_List (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->List(accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnLobby_Erase (const IPacket &packet)
    {
        std::string name = packet.Read();

        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Erase(name,accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnLobby_ValidateName (const IPacket &packet)
    {
        std::string name = packet.Read();

        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Name(name,accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnLobby_Restore (const IPacket &packet)
    {
        std::string name = packet.Read();

        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Restore(name,accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnSelect (const IPacket &packet)
    {
        std::string name = packet.Read();

        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Select(name,accountID,m_connection);

        return MSG_SUCCESS;
    }

    int StateCommand::OnCharacter (const IPacket &packet)
    {
        uint32_t accountID = packet.Read<uint32_t>();

        if (!packet.EndOfStream())
            return MSG_ERROR_SIZE;

        m_service->Fetch(accountID,m_connection);

        return MSG_SUCCESS;
    }
}
