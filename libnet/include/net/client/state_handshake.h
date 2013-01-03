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

#ifndef __ZELOS_LIBNET_NET_CLIENT_STATE_HANDSHAKE_H__
#define __ZELOS_LIBNET_NET_CLIENT_STATE_HANDSHAKE_H__

#include <net/connection_state.h>

class IConnection;

namespace clt
{

class IStateHandshake : public ConnectionState<IConnection>
{
public:

    enum { KEY_LENGTH = 8 };

    IStateHandshake (const std::string &srvID, const std::string &cltID, const boost::shared_ptr<IConnection> &conn);

    virtual ~IStateHandshake ();

    virtual void Close ();

private:

    void SendID ();

    virtual void OnConfig () = 0;

    int OnHandshake (const IPacket &packet);

    int OnHandshake_Clean (const IPacket &packet);

    int OnHandshake_Security (const IPacket &packet);

    int OnHandshake_Signature (const IPacket &packet);

    int OnWhoIs (const IPacket &packet);

protected:

    std::string m_SrvID;
    std::string m_CltID;

    uint32_t m_S;
    uint8_t m_base_key[KEY_LENGTH];
    uint8_t m_srv_signature[KEY_LENGTH];
};

}

#endif // __ZELOS_LIBNET_NET_CLIENT_STATE_HANDSHAKE_H__
