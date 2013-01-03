#include "coordinates.h"

#include <cmath>
#include <cassert>

int ZoneID (unsigned char xsector,unsigned char ysector)
{
	int id = xsector;

	if (ysector >= 128 )
		id -= 256*ysector;
	else
		id += 256*ysector;

	return id;
}

Coord::Coord ()
	: m_x(0), m_y(0), m_z(0), m_zone(0)
{
}

Coord::Coord (float x, float y, float z, int16_t zone)
	: m_x(x), m_y(y), m_z(z), m_zone(zone)
{
}

float Coord::global_x () const
{
	return ((m_zone & 0xFF) - 135)*192 + 0.1*m_x;
}

float Coord::global_y () const
{
	return ((m_zone >> 8) - 92)*192 + 0.1*m_y;
}

bool Coord::operator == (const Coord &coord) const
{
	return m_zone == coord.m_zone && m_x == coord.m_x && m_y == coord.m_y;
//	return m_zone == coord.m_zone && (std::abs(m_x-coord.m_x) <= 1e-2) && (std::abs(m_y-coord.m_y) <= 1e-2);
}

bool Coord::operator != (const Coord &coord) const
{
	return !(*this == coord);
}

Coord Coord::operator - (const Coord &coord) const
{
	return Coord(m_x - coord.m_x,m_y-coord.m_y, m_z-coord.m_z,m_zone);
}

Coord Coord::operator + (const Coord &coord) const
{
	return Coord(m_x + coord.m_x,m_y + coord.m_y, m_z + coord.m_z,m_zone);
}

Coord Coord::operator * (const float scalar) const
{
	return Coord(m_x*scalar,m_y*scalar,m_z*scalar,m_zone);
}

float Coord::modulus () const
{
	return std::sqrt(m_x*m_x+m_y*m_y);
}

void Coord::normalize ()
{
	float mod = modulus();

	if (mod)
	{
		m_x /= mod;
		m_y /= mod;
	}
}

float distance (const Coord& src, const Coord& dest)
{
	float dx = ((dest.get_zone() & 0xFF)-(src.get_zone() & 0xFF))*1920 + dest.get_x() - src.get_x();
	float dy = ((dest.get_zone() >> 8)-(src.get_zone() >> 8))*1920 + dest.get_y() - src.get_y();

	return sqrt(dx*dx+dy*dy);
}
