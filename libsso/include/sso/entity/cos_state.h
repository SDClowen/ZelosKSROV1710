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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_COS_STATE_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_COS_STATE_H__

namespace sso
{
    enum COS_STATE
    {
        COS_STATE_SUMMONNED = 2,
        COS_STATE_UNSUMMONED,
        COS_STATE_DEAD
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_COS_STATE_H__
