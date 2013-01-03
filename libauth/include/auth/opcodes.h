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

#ifndef __ZELOS_LIBAUTH_OPCODES_HPP__
#define __ZELOS_LIBAUTH_OPCODES_HPP__

enum
{
    SERVER_AUTHENTICATION = 0x5000,
    SERVER_WHOIS = 0x2001,
    SERVER_INFORMATION = 0x600D,

    CLIENT_AUTHENTICATION_VALID = 0x9000
};

#endif //__ZELOS_LIBAUTH_OPCODES_HPP__
