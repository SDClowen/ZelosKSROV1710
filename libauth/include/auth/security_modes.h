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

#ifndef SECURITY_MODES_H
#define SECURITY_MODES_H

enum SECURITY_MODES
{
    SECURITY_NONE = 0x01,
    SECURITY_ENCRYPTION = 0x02,
    SECURITY_CRC = 0x04,
    SECURITY_DH = 0x08,
    SECURITY_DH_AGREEMENT = 0x10
};

#endif // SECURITY_MODES_H
