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

#include "query_buff.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace buff {

void Insert (ScopedConnection &conn, const uint64_t charID, const uint32_t refSkillID,
    const uint32_t time)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "insert into character_buff values (" << charID << "," << refSkillID << "," << time << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::buff::Insert - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::buff::Insert - %s",error.what());
    }
}

std::vector<std::pair<uint32_t,uint32_t> > Select (ScopedConnection &conn, const uint64_t charID)
{
    std::vector<std::pair<uint32_t,uint32_t> > v;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from character_buff where charID = " << charID;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t i = 0; i < res.num_rows(); ++i)
            v.push_back(std::make_pair(res[i]["refSkillID"],res[i]["Time"]));
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::buff::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::buff::Select - %s",error.what());
    }

    return v;
}

void Clear (ScopedConnection &conn, const uint64_t charID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from character_buff where charID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::buff::Clear - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::buff::Clear - %s",error.what());
    }
}

}   }
