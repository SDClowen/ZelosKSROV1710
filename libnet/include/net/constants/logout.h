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

#ifndef DEF_PKT_LOGOUT_H
#define DEF_PKT_LOGOUT_H

namespace pkt
{
    enum LOGOUT_ERROR_TYPE
    {
        LOGOUT_ERROR_ATTACK_STATE = 0x0801,	/// UIIT_MSG_LOGOUT_ERR_CANT_LOGOUT_IN_BATTLE_STATE (*)
        LOGOUT_ERROR_TELEPORTING = 0x0802,	/// UIIT_MSG_LOGOUT_ERR_CANT_LOGOUT_WHILE_TELEPORT_WORKING (*)
    };
}

#endif // DEF_PKT_LOGOUT_H
