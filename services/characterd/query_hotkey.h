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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_HOTKEY_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_HOTKEY_H__

#include <vector>

#include <db/scoped_connection.h>
#include <sso/hotkey.h>

namespace db {
namespace hotkey {

    /**
     *
     *	@brief Insert a new hotkey for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param hotkey Hotkey to insert.
     *
     **/

    void Insert (ScopedConnection &conn, const uint64_t charID, const sso::Hotkey &hotkey);

    /**
     *
     *	@brief Remove all hotkeys for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *
     **/

    void Clear (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Remove a hotkey from the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param pos Hotkey position to remove.
     *
     **/

    void Remove (ScopedConnection &conn, const uint64_t charID, const uint8_t pos);

    /**
     *
     *	@brief Obtain all the hotkeys for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@retval List of available hotkeys.
     *
     **/

    std::vector<sso::Hotkey> Select (ScopedConnection &conn, const uint64_t charID);

}   }

#endif //__ZELOS_SERVICES_CHARACTERD_QUERY_HOTKEY_H__
