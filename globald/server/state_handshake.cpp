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

#include "server/state_handshake.h"

#include <net/server/connection.h>

#include "server/states.h"

namespace srv
{
    StateHandshake::StateHandshake (const boost::shared_ptr<IConnection> &conn, const std::string &srvID)
        : srv::IStateHandshake(srvID,conn)
    {
        m_state_table.insert(std::make_pair("EmblemServer",srv::STATE_EMBLEM));
        m_state_table.insert(std::make_pair("GatewayServer",srv::STATE_GATEWAY));
        m_state_table.insert(std::make_pair("GlobalAgent",srv::STATE_GLOBAL_AGENT));
        m_state_table.insert(std::make_pair("ShardManager",srv::STATE_SHARD));
        m_state_table.insert(std::make_pair("DownloadManager",srv::STATE_DOWNLOAD));
        m_state_table.insert(std::make_pair("NameServer",srv::STATE_NAME));
    }

    StateHandshake::~StateHandshake ()
    {
    }

    bool StateHandshake::OnClientID (const std::string &ID)
    {
        std::map<std::string,uint8_t>::const_iterator i = m_state_table.find(ID);

        if (i == m_state_table.end())
            return false;

        m_connection->setState(i->second);

        return true;
    }
}
