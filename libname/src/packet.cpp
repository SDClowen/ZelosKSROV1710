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

#include "name/packet.h"

#include <net/opcode/name_clt.h>
#include <packet/packet.h>

namespace clt_pkt
{
    void RegisterService (OPacket *pkt, const SYS_SERVICE service, const std::string &host,
                          const std::string &port)
    {
        pkt->WriteOpcode(CLT_REGISTER_SERVICE);
        pkt->Write<uint8_t>(service);
        pkt->Write(host);
        pkt->Write(port);
    }

    void Query (OPacket *pkt, const SYS_SERVICE service)
    {
        pkt->WriteOpcode(CLT_QUERY);
        pkt->Write<uint8_t>(service);
    }

    void UnregisterService (OPacket *pkt)
    {
        pkt->WriteOpcode(CLT_UNREGISTER_SERVICE);
    }
}
