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

#ifndef __ZELOS_LIBNAME_NAME_PACKET_H__
#define __ZELOS_LIBNAME_NAME_PACKET_H__

#include <string>

#include <name/services.h>

class OPacket;

namespace clt_pkt
{
    /**
     *
     *	@brief Build a packet to register a service at name server.
     *	@param pkt: Message stream to write data.
     *	@param host: Service host address.
     *	@param port: Service listening port.
     *
     **/

    void RegisterService (OPacket *pkt, const SYS_SERVICE service, const std::string &host,
                          const std::string &port);

    /**
     *
     *	@brief Build a packet to query name server for a desired service.
     *	@param pkt: Message stream to write data.
     *	@param service: Service type to query.
     *
     **/

    void Query (OPacket *pkt, const SYS_SERVICE service);

    /**
     *
     *	@brief Build a packet to unregister a service at name server.
     *	@param pkt: Message stream to write data.
     *
     **/

    void UnregisterService (OPacket *pkt);
}

#endif // __ZELOS_LIBNAME_NAME_PACKET_H__
