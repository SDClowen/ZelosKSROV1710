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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_SKILL_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_SKILL_H__

#include <set>
#include <stdint.h>

#include <db/scoped_connection.h>

namespace db {
namespace skill {

    /**
     *
     *	@brief Add a new skill for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param refSkillID Selected skill to insert (Must be inside refskilldata table).
     *
     **/

    void Insert (ScopedConnection &conn, const uint64_t charID, const uint32_t refSkillID);

    /**
     *
     *	@brief Delete all skills for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *
     **/

    void Clear (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Delete a skill for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param refSkillID Selected skill to delete (Must be inside refskilldata table).
     *
     **/

    void Delete (ScopedConnection &conn, uint64_t charID, const uint32_t refSkillID);

    /**
     *
     *	@brief Obtain all the available skills for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *
     **/

    std::set<uint32_t> Select (ScopedConnection &conn, const uint64_t charID);

}   }

#endif //__ZELOS_SERVICES_CHARACTERD_QUERY_SKILL_H__
