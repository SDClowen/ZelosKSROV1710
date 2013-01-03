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

#ifndef __ZELOS_LIBNET_OPCODE_GATEWAY_SRV_H__
#define __ZELOS_LIBNET_OPCODE_GATEWAY_SRV_H__

enum
{
    SERVER_VERSION = 0xA100,
    SERVER_GAME_SERVER_LIST = 0xA101,
    SERVER_LOGIN = 0xA102,
    SERVER_CAPTCHA = 0x2322,
    SERVER_CAPTCHA_VALIDATE = 0xA323,
};

#endif // __ZELOS_LIBNET_OPCODE_GATEWAY_SRV_H__
