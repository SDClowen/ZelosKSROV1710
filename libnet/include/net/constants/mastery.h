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

#ifndef DEF_PKT_MASTERY_H
#define DEF_PKT_MASTERY_H

namespace pkt
{
    enum MASTERY_ERROR
    {
        MASTERY_ERROR_INSUFFICIENT_SP = 0x3802,		/// UIIT_STT_SKILL_POINT_INSUFFICIENCY_MASTERY (*)
        MASTERY_ERROR_LIMIT = 0x3804,				/// UIIT_STT_SKILL_LEARN_MASTERY_LIMIT (*)
        MASTERY_ERROR_TREE_LIMIT = 0x3805			/// UIIT_STT_SKILL_LEARN_MASTERY_TOTAL_LIMIT (*)
    };
}

#endif // DEF_PKT_MASTERY_H
