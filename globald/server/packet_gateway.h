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

#ifndef __ZELOS_GLOBALD_SERVER_PACKET_GATEWAY_H__
#define __ZELOS_GLOBALD_SERVER_PACKET_GATEWAY_H__

#include <stdint.h>
#include <string>

#include <map>
#include <boost/shared_ptr.hpp>

#include "news.h"
#include "shard.h"

class OPacket;

namespace srv_pkt
{
    enum LOGIN_ERROR
    {
        LOGIN_ERROR_WRONG_PW = 1,
        LOGIN_ERROR_BLOCKED_MSG,
        LOGIN_ERROR_USER_ALREDY_ON,
        LOGIN_ERROR_C5,
        LOGIN_ERROR_C6,
        LOGIN_ERROR_C7,
        LOGIN_ERROR_C8,
    };

    void WriteConfig (OPacket *pkt, const uint8_t locale, const uint32_t client_version,
        const std::map<uint16_t,Shard> &servers, const std::map<uint32_t,Notice> &news);

    void WriteShard (OPacket *pkt, const uint16_t ID);

    void WriteShard (OPacket *pkt, const SHARD_NOTIFY type, const uint16_t ID, const uint16_t arg);

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const uint32_t queueID,
        const Agent &agent);

    /**
     *
     *  @brief: Login error packet. (USER_ALREDY_ON,C5,C7,C8 are encrypted packets)
     *  @param error: All login errors except WRONG_PW AND BLOCKED_MSG.
     *
     **/

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error);

    /**
     *
     *  @brief: WRONG PW/ACCOUNT ERROR.
     *  @param error: it should be only LOGIN_ERROR_WRONG_PW.
     *  @param max: Max login attempts before the accounts gets blocked.
     *  @param count: Number of failed logins.
     *
     **/

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error,
                     const uint32_t count, const uint32_t max);

    void WriteLogin (OPacket *pkt, const uint32_t clientID, const LOGIN_ERROR error,
        const std::string &msg, const uint16_t year, const uint8_t month, const uint8_t day,
        const uint8_t hour, const uint8_t min);
}

#endif // __ZELOS_GLOBALD_SERVER_PACKET_GATEWAY_H__
