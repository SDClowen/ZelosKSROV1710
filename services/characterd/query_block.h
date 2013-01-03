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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_BLOCK_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_BLOCK_H__

#include <stdint.h>
#include <string>
#include <vector>

#include <db/scoped_connection.h>

namespace db {
namespace block {

    /**
     *
     *	@brief Check if a character with the selected name exists.
     *	@param conn Database connection.
     *	@param name Character name to check.
     *	@retval True if exists, false otherwise.
     *
     **/

    bool Valid (ScopedConnection &conn, const std::string &name);

    /**
     *
     *	@brief Check if the name is in the character block list.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param name Name of the character to search.
     *	@retval True if exists, false otherwise.
     *
     **/

    bool Check (ScopedConnection &conn, const uint64_t charID, const std::string &name);

    /**
     *
     *	@brief Insert into character block list a new entry.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param name Name to insert into the block list.
     *
     **/

    void Insert (ScopedConnection &conn, const uint64_t charID, const std::string &name);

    /**
     *
     *	@brief Remove from character block list an entry.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param name Name to remove from block list.
     *
     **/

    void Remove (ScopedConnection &conn, const uint64_t charID, const std::string &name);

    /**
     *
     *	@brief Obtain character block list.
     *	@param Database connection.
     *	@param charID Selected character database ID.
     *	@retval List of the blocked character names.
     *
     **/

    std::vector<std::string> Select (ScopedConnection &conn, const uint64_t charID);

}	}


#endif // __ZELOS_SERVICES_CHARACTERD_QUERY_BLOCK_H__
