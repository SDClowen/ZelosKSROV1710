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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_MASTERY_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_MASTERY_H__

#include <stdint.h>
#include <vector>

#include <db/scoped_connection.h>

namespace db {
namespace mastery {

    /**
     *
     *  @brief Save character mastery information into the database.
     *  @param conn: Database Connection.
     *  @param charID: Selected character database ID.
     *  @param masteryID: Mastery reference ID to be inserted.
     *	@param lv: Mastery current level.
     *
     */

    void Create (ScopedConnection &conn, const uint64_t charID, const uint32_t mastID, const uint8_t lv);

    /**
     *
     *	@brief Update mastery level.
     *  @param conn: Database Connection.
     *  @param charID: Selected character database ID.
     *  @param masteryID: Mastery reference ID.
     *	@param lv: Mastery current level.
     *
     */

    void Update (ScopedConnection &conn, const uint64_t charID, const uint32_t mastID, const uint8_t lv);

    /**
     *
     *	@brief Obtain character mastery tree.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@retval Character masteries.
     *
     **/

    std::vector<std::pair<uint32_t,uint8_t> > Select (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Delete character mastery data.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *
     **/

    void Delete (ScopedConnection &conn, const uint64_t charID);

}   }

#endif //__ZELOS_SERVICES_CHARACTERD_QUERY_MASTERY_H__
