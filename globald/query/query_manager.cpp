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

#include "query/query_manager.h"

#include <mysql++/mysql++.h>

namespace db {
namespace manager {

ServConfig readConfig (db::ScopedConnection &conn, const std::string &name)
{
    ServConfig cfg;
    cfg.ch_char = true;
    cfg.eu_char = false;

    mysqlpp::Query query = conn->query();

    query << "select * from serv_config where serv_name = " << mysqlpp::quote << name;

    mysqlpp::StoreQueryResult res = query.store();

    if (res.num_rows())
    {
        cfg.ch_char = res[0]["ch_char"];
        cfg.eu_char = res[0]["eu_char"];
    }

    return cfg;
}

FilterList readNameFilter (db::ScopedConnection &conn)
{
    FilterList ret;

    mysqlpp::Query query = conn->query();

    query << "select * from char_filter";

    mysqlpp::StoreQueryResult res = query.store();

    if (res.num_rows())
        ret.first = (std::string)res[0]["ch"];

    query.reset();

    query << "select * from name_filter";

    res = query.store();

    for (size_t i = 0; i < res.num_rows(); ++i)
        ret.second.push_back((std::string)res[0]["str"]);

    return ret;
}

}   }

