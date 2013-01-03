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

#ifndef DEF_PKT_TELEPORT_H
#define DEF_PKT_TELEPORT_H

namespace pkt
{
    enum TELEPORT_DESTINATION_ERROR
    {
        TELEPORT_DST_ERROR_INSUFFICIENT_GOLD = 0x1C07,
        TELEPORT_DST_ERROR_IN_USE = 0x1C10,
        TELEPORT_DST_UNREPAIRABLE = 0x1C11,
        TELEPORT_DST_UNREPRODUCEABLE = 0x1C12,
        TELEPORT_DST_ERROR_TO_FAR_FROM_DOCK = 0x1C13,
        TELEPORT_DST_ERROR_BEING_TELEPORTED = 0x1C14,
        TELEPORT_DST_ERROR_INVALID_LEVEL = 0x1C15,
        TELEPORT_DST_ERROR_MURDER_STATUS = 0x1C16,
        TELEPORT_DST_ERROR_INVALID_REQUEST = 0x1C17,
        TELEPORT_DST_ERROR_INVALID_JOB = 0x1C18,
        TELEPORT_DST_ERROR_LEVEL_ABOVE_20 = 0x1C20,
        TELEPORT_DST_ERROR_NO_RECALL_OR_DEATH_POINT = 0x1C21
    };
}

#endif // DEF_PKT_TELEPORT_H
