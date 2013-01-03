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

#include "query/query_shard.h"

#include <mysql++/mysql++.h>

namespace db {
namespace shard {

    std::map<uint16_t,Shard> readShards (db::ScopedConnection &conn)
    {
        std::map<uint16_t,Shard> v;

        mysqlpp::Query query = conn->query();

        query << "select * from shards";

        Shard shard;
        mysqlpp::StoreQueryResult res = query.store();

        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            shard.id = res[i]["refID"];
            shard.name = (std::string)res[i]["name"];
            shard.is_new = res[i]["new"];
            shard.state = res[i]["state"];
            shard.usr_max = res[i]["capacity"];

            v.insert(std::make_pair(shard.id,shard));
        }

        return v;
    }

    void writeState (db::ScopedConnection &conn, const uint32_t refID, const int state)
    {
        mysqlpp::Query query = conn->query();

        query << "update shards set state = " << state << " where refID = " << refID;

        query.exec();
    }

    void writeStateClear (db::ScopedConnection &conn)
    {
        mysqlpp::Query query = conn->query();

        query << "update shards set state = 0";

        query.exec();
    }

}   }

