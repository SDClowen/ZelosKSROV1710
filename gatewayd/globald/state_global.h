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

#ifndef __ZELOS_GATEWAYD_GLOBALD_STATE_GLOBAL_H__
#define __ZELOS_GATEWAYD_GLOBALD_STATE_GLOBAL_H__

#include <net/connection_state.h>

class Service;
class IConnection;

namespace clt
{
    class StateGlobal : public ConnectionState<IConnection>
    {
    public:

        StateGlobal (Service *service, const boost::shared_ptr<IConnection> &conn);

        virtual ~StateGlobal ();

        virtual void Close ();

    protected:

        int OnConfig (const IPacket &packet);

        int OnAccountProcess (const IPacket &packet);

        int OnNewsInsert (const IPacket &packet);

        int OnNewsRemove (const IPacket &packet);

        int OnShardInsert (const IPacket &packet);

        int OnShardRemove (const IPacket &packet);

        int OnShardNotify (const IPacket &packet);

        int OnPatchInsert (const IPacket &packet);

        int OnPatchRemove (const IPacket &packet);

        int OnPatchNotify (const IPacket &packet);

    private:

        Service *m_service;
    };
}

#endif //__ZELOS_GATEWAYD_GLOBALD_STATE_GLOBAL_H__
