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

#ifndef __ZELOS_AGENTD_SERVER_STATE_LOBBY_H__
#define __ZELOS_AGENTD_SERVER_STATE_LOBBY_H__

#include <net/connection_state.h>

class IConnection;
class Service;

namespace srv
{
    class StateLobby : public ConnectionState<IConnection>
    {
    public:

        StateLobby (Service *service, const boost::shared_ptr<IConnection> &conn);

        virtual ~StateLobby ();

        void Close ();

        void setAccountID (const uint32_t accID);

        void setBlock (bool block);

    private:

        int OnCommand (const IPacket &packet, const uint16_t opcode);

    private:

        Service *m_service;

        bool m_block;
        uint32_t m_accountId;
    };
}

#endif // __ZELOS_AGENTD_SERVER_STATE_LOBBY_H__
