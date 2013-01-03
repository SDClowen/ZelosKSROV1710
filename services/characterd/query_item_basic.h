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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_BASIC_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_BASIC_H__

#include <stdint.h>
#include <utility>
#include <vector>

#include <db/scoped_connection.h>

namespace db {
namespace item_basic {

    /**
     *
     *	@brief Obtain starter items list.
     *	@param conn Database connection.
     *	@retval List of starter items.
     *
     **/

    std::vector<std::pair<uint32_t,uint16_t> > Select (ScopedConnection &conn);

}	}

#endif // __ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_BASIC_H__
