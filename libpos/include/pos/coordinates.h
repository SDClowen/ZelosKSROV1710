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

#ifndef COORD_H
#define COORD_H

#include <stdint.h>
#include <cassert>

int ZoneID (unsigned char xsector,unsigned char ysector);

class Coord
{
public:

    Coord ();

    Coord (float x, float y, float z, int16_t zone);

    inline void set_x (float x) { m_x = x; }

    inline void set_y (float y) { m_y = y; }

    inline void set_z (float z) { m_z = z; }

    inline void set_zone (int16_t zone) { m_zone = zone; }

    inline int get_zone () const { return m_zone; }

    inline float get_x () const { return m_x; }

    inline float get_y () const { return m_y; }

    inline float get_z () const { return m_z; }

    inline bool IsDungeon () const { return m_zone & 0x8000; }

    /**
     *
     *  @brief Convert X local coordinates to game coordinates.
     *
     **/

    float global_x () const;

    /**
     *
     *  @brief Convert Y local coordinates to game coordinates.
     *
     **/

    float global_y () const;

    bool operator == (const Coord &coord) const;

    bool operator != (const Coord &coord) const;

    Coord operator - (const Coord &coord) const;

    Coord operator + (const Coord &coord) const;

    Coord operator * (const float scalar) const;

    float modulus () const;

    void normalize ();

private:

    float m_x;
    float m_y;
    float m_z;
    int16_t m_zone;
};

float distance (const Coord& src, const Coord& dest);

#endif // COORD_H
