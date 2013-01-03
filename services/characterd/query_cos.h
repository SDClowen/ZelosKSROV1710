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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_COS_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_COS_H__

#include <db/scoped_connection.h>
#include <sso/property/item_cos.h>

namespace db {
namespace cos {

    uint64_t Create (ScopedConnection &conn, const sso::property::COS &attr);

    sso::property::COS Select (ScopedConnection &conn, const uint32_t cosID);

    /**
     *
     *	@brief Update COS HGP
     *	@param conn: Database connection.
     *	@param cosID: COS refID.
     *	@param val: HGP value.
     *
     **/

    void HGP (ScopedConnection &conn, const uint32_t cosID, const uint16_t val);

    /**
     *
     *	@brief Check if the name is valid and not already taken.
     *	@param conn: Database connection.
     *	@param name: Name to check if its available and valid.
     *	@retval True if its valid and available, false otherwise.
     *
     **/

    bool IsNameValid (ScopedConnection &conn, const std::string &name);

    /**
     *
     *	@brief Update COS name.
     *	@param conn: Database connection.
     *	@param cosID: COS refID.
     *	@param name: COS name.
     *
     **/

    void Name (ScopedConnection &conn, const uint32_t cosID, const std::string &name);

    /**
     *
     *	@brief Update COS settings.
     *	@param conn: Database connection.
     *	@param cosID: COS refID.
     *	@param settings: COS settings (COS type specific).
     *
     **/

    void Settings (ScopedConnection &conn, const uint32_t cosID, const uint32_t settings);

}	}

#endif // __ZELOS_SERVICES_CHARACTERD_QUERY_COS_H__
