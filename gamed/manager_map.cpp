#include "manager_map.h"

#include <dirent.h>
#include <string.h>
#include <syslog.h>
#include <fstream>

#include <physic/navigation_mesh.h>
#include <sso/entity/player.h>

#include "zone.h"

#ifdef DEBUG_PRINT
#define ZONE_GRID_COUNT 1
#else
#define ZONE_GRID_COUNT 5
#endif

#define ZONE_GRID_SIZE (1920/ZONE_GRID_COUNT)

int nvm (const dirent *ep)
{
    return strstr(ep->d_name,".nvm") != NULL;
}

bool ZoneArea::belongs (const Coord &pos) const
{
	bool valid = false;

	if ( left == -1 && top == -1 && right == -1 && bottom == -1)
		valid = true;
	else
	{
		if ( pos.get_x() >= left && pos.get_x() <= right && pos.get_y() >= bottom && pos.get_y() <= top)
			valid = true;
	}

	return valid;
}

MapManager::MapManager (const std::string &path)
    : m_Path(path)
{
}

MapManager::~MapManager()
{
}

bool MapManager::LoadZones()
{
    std::string nvm_path = m_Path + "/navmesh";

    dirent **eps;
    int n;

    n = scandir(nvm_path.c_str(),&eps,nvm,alphasort);

    if (n > 0)
    {
        for (int cnt = 0; cnt < n; ++cnt)
			if (!LoadZone(eps[cnt]->d_name,phy::NavigationMesh::MODE_NAV_MESH))
                return false;
    }
    else
        return false;

	return loadZoneSafes();
}

bool MapManager::LoadZones (const std::set<int> &zones)
{
	std::string nvm_path = m_Path + "/navmesh";

	dirent **eps;
	int n;

	n = scandir(nvm_path.c_str(),&eps,nvm,alphasort);

	if (n > 0)
	{
		for (int cnt = 0; cnt < n; ++cnt)
		{
			int16_t navID = strtol(eps[cnt]->d_name+3,NULL,16);

			if (zones.find(navID) != zones.end())
			{
				if (!LoadZone(eps[cnt]->d_name,phy::NavigationMesh::MODE_NAV_MESH))
					return false;
			}
		}
	}
	else
		return false;

	return loadZoneSafes();
}

bool MapManager::LoadZone (const char *name, int mode)
{
	int16_t navID = strtol(name+3,NULL,16);

    std::string filename = m_Path + "/navmesh/" + name;

	boost::shared_ptr<phy::NavigationMesh> navmesh(new phy::NavigationMesh(navID));

	if (!navmesh || !navmesh->Read(filename,static_cast<phy::NavigationMesh::READ_MODE>(mode)))
        return false;

	if (mode & phy::NavigationMesh::MODE_NAV_MESH && !navmesh->buildNavigationMesh())
        return false;

	m_NavMeshes.insert(std::make_pair(navID,navmesh));

	uint32_t zoneID = 0;
	float minX=0,maxX=0,minZ=0,maxZ=0;
	boost::shared_ptr<Zone> zone;

	for (int x = 0; x < ZONE_GRID_COUNT; ++x)
	{
		minX = navmesh->minX() + x*ZONE_GRID_SIZE;
		maxX = (x+1)*ZONE_GRID_SIZE;
		
		for (int z = 0; z < ZONE_GRID_COUNT; ++z)
		{
			zoneID = MapManager::ZoneID(ZONE_GRID_COUNT*((navID & 0xFF)-1) + x,
										ZONE_GRID_COUNT*((navID >> 8)-1) + z);

			minZ = navmesh->minZ() + z*ZONE_GRID_SIZE;
			maxZ = (z+1)*ZONE_GRID_SIZE;

			zone.reset(new Zone(zoneID,minX,maxX,minZ,maxZ));

			if (!zone)
				return false;

			m_ZoneTable.insert(std::make_pair(zoneID,zone));
		}
	}

	return true;
}

bool MapManager::loadZoneSafes ()
{
	/// LOAD ZONE AREA DATA!
	std::string filename = m_Path + "/regioninfo.txt";
	std::ifstream file(filename.c_str());

	if (!file)
		return false;

	int x,y;
	std::string box;

	ZoneArea area;
	safe_iterator iter;
	std::vector<ZoneArea> areas;

	while (!file.eof())
	{
		std::string line;
		getline(file,line);

		if (*line.begin() == '\t')
			continue;

		if (*line.begin() == '#')
		{
			if (line.find("#TOWN") != std::string::npos)
				area.type = ZONE_TOWN;
			else
				area.type = ZONE_FIELD;

			continue;
		}

		if (area.type == ZONE_TOWN)
		{
			std::stringstream s(line);
			s >> x >> y >> box;

			int16_t zoneID = (y << 8) + x;

			if (m_NavMeshes.find(zoneID) != m_NavMeshes.end())
			{
				iter = m_ZoneSafes.find(zoneID);

				if (box == "RECT")
					s >> area.left >> area.top >> area.right >> area.bottom;
				else
					area.left = area.top = area.right = area.bottom = -1;

				if (iter != m_ZoneSafes.end())
				{
					iter->second.push_back(area);
				}
				else
				{
					areas.push_back(area);
					m_ZoneSafes.insert(std::make_pair(zoneID,areas));
					areas.clear();
				}
			}
		}
	}

	file.close();

	return true;
}

boost::shared_ptr<Zone> MapManager::findByPos (const Coord &pos) const
{
    boost::shared_ptr<Zone> zone;

	const_iterator iter = m_ZoneTable.find(MapManager::ZoneID(pos));

    if (iter != m_ZoneTable.end())
        zone = iter->second;
	else
	{
		syslog(LOG_INFO,"Pos = %f %f %f Zone = %i",pos.get_x(),pos.get_y(),pos.get_z(),pos.get_zone());
		syslog(LOG_INFO,"Zone %X dont exists",MapManager::ZoneID(pos));
	}

    return zone;
}

boost::shared_ptr<Zone> MapManager::findByID (const uint32_t zoneID) const
{
	boost::shared_ptr<Zone> zone;

	const_iterator iter = m_ZoneTable.find(zoneID);

	if (iter != m_ZoneTable.end())
		zone = iter->second;

	return zone;
}

bool MapManager::IsInsideTown (const Coord &pos) const
{
	safe_const_iterator siter = m_ZoneSafes.find(pos.get_zone());

	if (siter != m_ZoneSafes.end())
	{
		for (std::vector<ZoneArea>::const_iterator it = siter->second.begin(); it != siter->second.end(); ++it)
		{
			if (it->belongs(pos))
				return true;
		}
	}

	return false;
}

Coord MapManager::GenRadiusPosition(const Coord &center, unsigned short radius, bool skipSafe) const
{
	Coord genPos;
	Coord realPos;

	do
	{
		genPos.set_zone(center.get_zone());
		genPos.set_x(center.get_x() + (rand()%radius)*sin((rand()%360)*3.1415/180));
		genPos.set_y(center.get_y() + (rand()%radius)*sin((rand()%360)*3.1415/180));
	}
	while (!ResolveMotion(center,genPos,&realPos) && skipSafe && IsInsideTown(realPos));

	return realPos;
}

MapManager::iterator MapManager::begin()
{
    return m_ZoneTable.begin();
}

MapManager::iterator MapManager::end()
{
    return m_ZoneTable.end();
}

MapManager::const_iterator MapManager::begin() const
{
    return m_ZoneTable.begin();
}

MapManager::const_iterator MapManager::end() const
{
    return m_ZoneTable.end();
}

size_t MapManager::size () const
{
	return m_ZoneTable.size();
}

float MapManager::getHeightAt(const Coord &pos) const
{
	float h = getTerrainHeight(pos);

	if (pos.get_z() > h)
	{
		float hobj = getObjectHeight(pos);

		if (hobj != -1e4 && pos.get_z() >= hobj)
			h = hobj;
	}

	return h;
}

float MapManager::getTerrainHeight (const Coord &pos) const
{
	float height = -1e4;
	nav_const_iterator it = m_NavMeshes.find(pos.get_zone());

	if (it != m_NavMeshes.end())
	{
		float x = pos.get_x();
		float z = pos.get_y();

		assert(x >= it->second->minX() && x <= it->second->maxX());
		assert(z >= it->second->minZ() && z <= it->second->maxZ());

		x /= 20.0f;
		z /= 20.0f;

		height = it->second->getHeightAt(static_cast<int>(x),static_cast<int>(z));
	}

	return height;
}

float MapManager::getObjectHeight (const Coord &pos) const
{
	float height = -1e4;
	nav_const_iterator it = m_NavMeshes.find(pos.get_zone());

	if (it != m_NavMeshes.end())
		height = it->second->getObjectHeight(pos.get_x(),pos.get_y(),m_modelMgr);

	return height;
}

bool MapManager::ResolveMotion (const Coord &src, const Coord &dest, Coord *end) const
{
    assert(end);

    bool flag = true;
    Coord current = src;

    while(flag)
    {
		nav_const_iterator iter = m_NavMeshes.find(current.get_zone());

		if (iter == m_NavMeshes.end())
            return false;

        assert(current.get_x() <= iter->second->maxX() && current.get_x() >= iter->second->minX());
        assert(current.get_y() <= iter->second->maxZ() && current.get_y() >= iter->second->minZ());

        int8_t zdx = (dest.get_zone() & 0xFF) - (current.get_zone() & 0xFF);
		int8_t zdz = (dest.get_zone() >> 8) - (current.get_zone() >> 8);

		Coord relative(zdx*1920+dest.get_x(),zdz*1920+dest.get_y(),dest.get_z(),current.get_zone());

		flag = iter->second->resolveMotion(current,relative,m_modelMgr,end);

        current = *end;
    }

    end->set_zone(current.get_zone());

    return true;
}

RSOLVE_OBJECT_RESULT MapManager::ResolveMotion (const Coord &curPos, const Coord &nextPos,
												const std::vector<MapManager::ICharPtr> &objects,
												callback_function callback, bool calcPos,
												float curSpeed, float curAngle, uint32_t dt)
{
	typedef std::vector<ICharPtr>::const_iterator nconst_iterator;

	uint32_t srcID = MapManager::ZoneID(curPos);

	boost::shared_ptr<Zone> zone = findByID(srcID);

	if (!zone)
	{
		for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (*it)
				(*it)->setDestination(Coord());
		}

		return RET_INVALID_ZONE;
	}

	Coord pos;

	if (calcPos)
	{
		float step = curSpeed*static_cast<float>(dt)/1000.0f;
		float angle = curAngle/10430.38208;  /// CONVERT ANGLE TO RADIANS
		float sx = step*cos(angle);
		float sz = step*sin(angle);

		float x = curPos.get_x()+sx;
		float z = curPos.get_y()+sz;

		/// CHECK THAT THE MOVEMENT IS NOT BEYOND THE DESTINATION POINT
		if (curPos.get_zone() == nextPos.get_zone())
		{
			if (sx > 0)
			{
				if (x > nextPos.get_x())
					x = nextPos.get_x();
			}
			else
			{
				if (x < nextPos.get_x())
					x = nextPos.get_x();
			}

			if (sz > 0)
			{
				if (z > nextPos.get_y())
					z = nextPos.get_y();
			}
			else
			{
				if (z < nextPos.get_y())
					z = nextPos.get_y();
			}
		}

		pos = Coord(x,z,curPos.get_z(),curPos.get_zone());
	}
	else
		pos = nextPos;

	Coord dest;

	if (!ResolveMotion(curPos,pos,&dest))
		return RET_INVALID_ZONE;

	for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if (*it)
			(*it)->setPosition(dest);
	}

	RSOLVE_OBJECT_RESULT ret = RET_SUCCESS;

	uint32_t dstID = MapManager::ZoneID(dest);

	if (srcID != dstID)
	{
		/// NOTE: THIS WONT WORK FOR INSTANCES!!!
//		syslog(LOG_INFO,"SrcID = %X  -  DstID = %X",srcID,dstID);
//		syslog(LOG_INFO,"Src = %f %f %f %i",curpos.get_x(),curpos.get_y(),curpos.get_z(),curpos.get_zone());
//		syslog(LOG_INFO,"Dst = %f %f %f %i",dest.get_x(),dest.get_y(),dest.get_z(),dest.get_zone());
//
//		syslog(LOG_INFO,"MinX = %f, MaxX = %f",zone->minX(),zone->maxX());
//		syslog(LOG_INFO,"MinZ = %f, MaxZ = %f",zone->minZ(),zone->maxZ());

		GridResult result;

		/// OBTAIN THE X,Y OF THE CENTER ZONE
		result.xz = MapManager::XZoneID(srcID);
		result.zz = MapManager::YZoneID(srcID);

		/// CALCULATE ADJACENT AREAS,ZONE AND POSITION
		result.dx = MapManager::XZoneID(dstID)-MapManager::XZoneID(srcID);

		result.adjx.min_x = result.adjx.max_x = result.xz + 2*result.dx;
		result.adjx.min_y = result.zz - 1;
		result.adjx.max_y = result.zz + 1;

		/// CALCULATE THE X OF THE NEW CENTER
		result.xz = MapManager::XZoneID(dstID);

		result.dz = MapManager::YZoneID(dstID)-MapManager::YZoneID(srcID);

		result.adjz.min_y = result.adjz.max_y = result.zz + 2*result.dz;
		result.adjz.min_x = result.xz - 1;
		result.adjz.max_x = result.xz + 1;

		/// CALCULATE THE Y OF THE NEW CENTER
		result.zz = MapManager::YZoneID(dstID);

		/// INSERT PLAYER INTO THE NEW ZONE
		boost::shared_ptr<Zone> dzone = findByID(dstID);

		if (!dzone)
		{
			for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
			{
				if (*it)
					(*it)->setDestination(Coord());
			}

			return RET_INVALID_ZONE;
		}

		/// REGISTER TO ADJACENT AREAS
		for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (*it)
			{
				if ((*it)->getType() == ET_PLAYER)
					zone->RemovePlayer((*it)->GUID());
				else
					zone->RemoveNPC((*it)->GUID());
			}
		}

		callback(objects,&result);

		for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (*it)
			{
				(*it)->setChannelID(dstID);

				if ((*it)->getType() == ET_PLAYER)
				{
					boost::shared_ptr<sso::Player> player =
							boost::shared_dynamic_cast<sso::Player,sso::ICharacter>(*it);

					if (player)
						dzone->InsertPlayer(player);
					else
						dzone->InsertNPC(*it);
				}
				else
				{
					dzone->InsertNPC(*it);
				}
			}
		}
	}

	if (dest == nextPos)
	{
		for (nconst_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (*it)
				(*it)->setDestination(Coord());
		}

		ret = RET_SUCCESS_DESTINATION;
	}

	return ret;
}

void MapManager::setModelManager (const boost::shared_ptr<phy::ModelManager> &modelMgr)
{
	m_modelMgr = modelMgr;
}

uint32_t MapManager::ZoneID (const uint16_t x, const uint16_t y)
{
	return (y << 16) + x;
}

uint32_t MapManager::ZoneID (const Coord &pos)
{
	uint16_t dx = ZONE_GRID_COUNT*((pos.get_zone() & 0xFF)-1) + (pos.get_x()/ZONE_GRID_SIZE);
	uint16_t dy = ZONE_GRID_COUNT*((pos.get_zone() >> 8)-1) + (pos.get_y()/ZONE_GRID_SIZE);

	return (dy << 16) + dx;
}

uint16_t MapManager::XZoneID (const uint32_t zoneID)
{
	return (zoneID & 0xFFFF);
}

uint16_t MapManager::YZoneID (const uint32_t zoneID)
{
	return (zoneID >> 16);
}

AdjacentZone MapManager::GetAdjacentZone (int32_t gridID)
{
	uint16_t SectorX = static_cast<uint16_t>(gridID & 0xFFFF);
	uint16_t SectorY = static_cast<uint16_t>(gridID >> 16);

	AdjacentZone area;
	area.min_x = SectorX - ZONE_GRID_COUNT;
	area.max_x = SectorX + ZONE_GRID_COUNT;
	area.min_y = SectorY - ZONE_GRID_COUNT;
	area.max_y = SectorY + ZONE_GRID_COUNT;

	return area;
}

AdjacentZone MapManager::GetAdjacentArea (int32_t gridID)
{
	uint16_t SectorX = static_cast<uint16_t>(gridID & 0xFFFF);
	uint16_t SectorY = static_cast<uint16_t>(gridID >> 16);

	AdjacentZone area;
	area.min_x = SectorX;
	area.max_x = SectorX;
	area.min_y = SectorY;
	area.max_y = SectorY;

	--area.min_x;
	--area.min_y;
	++area.max_x;
	++area.max_y;

	return area;
}

