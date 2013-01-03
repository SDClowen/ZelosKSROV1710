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

#include "query/query_news.h"

#include <mysql++/mysql++.h>

namespace db {
namespace news {

    std::vector<Notice> Select (db::ScopedConnection &conn)
    {
        mysqlpp::Query query = conn->query("select * from news order by date limit 0,4");

        mysqlpp::StoreQueryResult res = query.store();

        std::vector<Notice> news;
        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            Notice notice;
            notice.id = res[i]["id"];
            notice.heading = static_cast<std::string>(res[i]["title"]);
            notice.message = static_cast<std::string>(res[i]["message"]);
            notice.day = 7;
            notice.month = 9;

            news.push_back(notice);
        }

        return news;
    }

}	}
