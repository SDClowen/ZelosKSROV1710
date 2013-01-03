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

#ifndef __ZELOS_LIBSSO_SSO_SKILL_IMBUE_H__
#define __ZELOS_LIBSSO_SSO_SKILL_IMBUE_H__

#include <stdint.h>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace sso
{
    class Skill;

    struct Imbue
    {
        Imbue ()
            : castID(0), taskID(0)
        {}

        uint32_t castID;
        uint32_t taskID;

        boost::shared_ptr<Skill> skill;
    };
}

#endif //__ZELOS_LIBSSO_SSO_SKILL_IMBUE_H__
