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

#include "query_item_basic.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace item_basic {

std::vector<std::pair<uint32_t,uint16_t> > Select (ScopedConnection &conn)
{
    std::vector<std::pair<uint32_t,uint16_t> > items;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from refcharacter_basic_items";

        mysqlpp::StoreQueryResult res = query.store();

        std::pair<uint32_t,uint16_t> item;

        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            if (res[i]["Active"])
            {
                item.first = res[i]["refItemID"];
                item.second = res[i]["Quantity"];

                items.push_back(item);
            }
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item_basic - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item_basic - %s",error.what());
    }

    return items;
}

}	}
