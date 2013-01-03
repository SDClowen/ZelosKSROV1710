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

#ifndef __ZELOS_GAMED_NAMED_STATE_NAME_H__
#define __ZELOS_GAMED_NAMED_STATE_NAME_H__

#include <net/connection_state.h>

class IConnection;
class Service;

namespace named
{
    class StateName : public ConnectionState<IConnection>
    {
    public:

        StateName (Service *service, const boost::shared_ptr<IConnection> &conn);

        virtual ~StateName ();

        void Close ();

    private:

        int OnQuery (const IPacket &packet);

    private:

        Service *m_service;
    };
}

#endif // __ZELOS_GAMED_NAMED_STATE_NAME_H__
