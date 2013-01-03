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

#ifndef __ZELOS_AGENTD_NAMED_STATE_HANDSHAKE_H__
#define __ZELOS_AGENTD_NAMED_STATE_HANDSHAKE_H__

#include <net/client/state_handshake.h>

namespace named
{
    class StateHandshake : public clt::IStateHandshake
    {
    public:

        StateHandshake (const boost::shared_ptr<IConnection> &conn);

        virtual ~StateHandshake ();

    private:

        void OnConfig ();
    };
}

#endif //__ZELOS_AGENTD_NAMED_STATE_HANDSHAKE_H__
