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

#ifndef __ZELOS_LIBSERVER_NET_SERVER_STATE_HANDSHAKE__H__
#define __ZELOS_LIBSERVER_NET_SERVER_STATE_HANDSHAKE_H__

#include <net/connection_state.h>

/// HANDSHAKE PROCESS AND VALIDATION

class IConnection;

namespace srv
{

    class IStateHandshake : public ConnectionState<IConnection>
    {
    public:

        IStateHandshake (const std::string &srvID, const boost::shared_ptr<IConnection> &conn);

        virtual ~IStateHandshake ();

        virtual void Close ();

        void SetBaseKey (const uint32_t key[2]);

        void SetSharedData (uint32_t a, uint32_t P, uint32_t G, uint32_t A);

    protected:

        int OnHandshake (const IPacket &packet);

        int OnHandshakeValidated (const IPacket &packet);

        int OnWhoIs (const IPacket &packet);

        virtual bool OnClientID (const std::string &ID) = 0;

    private:

        bool m_authenticated;
        std::string m_srvID;

        uint32_t m_a;   /// PRIVATE EXPONENT
        uint32_t m_p;   /// PUBLIC PRIME
        uint32_t m_g;   /// PUBLIC BASE
        uint32_t m_A;   /// CACHED VALUE FOR MODEXP(P,a,G)

        uint32_t m_base_key[2];
    };
}

#endif  //__ZELOS_LIBSERVER_NET_SERVER_STATE_HANDSHAKE_H__
