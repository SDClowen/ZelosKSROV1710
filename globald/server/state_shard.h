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

#ifndef __ZELOS_GLOBALD_SERVER_STATE_SHARD_HPP__
#define __ZELOS_GLOBALD_SERVER_STATE_SHARD_HPP__

#include <net/connection_state.h>

class Service;
class IConnection;

namespace srv
{
    class StateShard : public ConnectionState<IConnection>
    {
    public:

        StateShard (Service *service, const boost::shared_ptr<IConnection> &conn);

        virtual ~StateShard ();

        virtual void Close ();

    private:

        int OnConfig (const IPacket &packet);

        int OnShardNotify (const IPacket &packet);

        int OnAgentInsert (const IPacket &packet);

        int OnAgentRemove (const IPacket &packet);

        int OnAccountNotify (const IPacket &packet);

    private:

        Service *m_service;
    };
}

#endif //__ZELOS_GLOBALD_SERVER_STATE_SHARD_HPP__
