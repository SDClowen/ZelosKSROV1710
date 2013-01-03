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

#ifndef DEF_PKT_SKILL_H
#define DEF_PKT_SKILL_H

namespace pkt
{
    enum INC_SKILL_ERROR
    {
        INC_SKILL_ERROR_INSUFFICIENT_INT = 0x3404,	/// UIIT_STT_SKILL_POINT_INSUFFICIENCY (*)
        INC_SKILL_ERROR_INSUFFICIENT_STR = 0x3405,	/// UIIT_STT_SKILL_LEARN_INT_INSUFFICIENCY
        INC_SKILL_ERROR_INSUFFICIENT_SP = 0x340A	/// UIIT_STT_SKILL_LEARN_STR_INSUFFICIENCY
    };
}

#endif // DEF_PKT_SKILL_H
