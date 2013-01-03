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

#ifndef __ZELOS_LIBPHYSIC_PHYSIC_NAVIGATION_ENTITY_H__
#define __ZELOS_LIBPHYSIC_PHYSIC_NAVIGATION_ENTITY_H__

#include <stdint.h>
#include <sys/types.h>

#include <vector>

#include <physic/line.h>
#include <physic/rectangle.h>

namespace phy
{
    struct NavigationEntity
    {
        Point TransformIntoObjectCoordinates(const Point &point) const;

        Line TransformIntoObjectCoordinates (const Line &line) const;

        Line TransformIntoWorldCoordinates (const Line &line) const;

        uint32_t refID;         /// Reference ID (listed data/navmesh/object.ifo file)
        float x;
        float y;
        float z;
        uint16_t uk2;
        float angle;            /// Angle in radians (Entity orientation in world space)
        uint16_t uk4;
        uint16_t uk5;
        uint16_t uk6;
        int16_t zoneID;
        std::vector<std::pair<uint32_t,uint16_t> > extra;

        Rect rect;
    };
}

#endif //__ZELOS_LIBPHYSIC_PHYSIC_NAVIGATION_ENTITY_H__
