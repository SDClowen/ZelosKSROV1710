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

#include "query_skill.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace skill {

void Insert (ScopedConnection &conn, const uint64_t CharID, const uint32_t SkillID)
{
    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();
        query << "insert into skills values (" << quote << CharID << ","
                << quote << (mysqlpp::sql_int)SkillID << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::skill::Insert - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::skill::Insert - %s",error.what());
    }
}

void Clear (ScopedConnection &conn, const uint64_t CharID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from skills where CharID =" << mysqlpp::quote << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::skill::Clear - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::skill::Clear - %s",error.what());
    }
}

void Delete (ScopedConnection &conn, const uint64_t CharID, const uint32_t SkillID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from skills where CharID = " << mysqlpp::quote << CharID
                << " and ID = " << SkillID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::skill::Delete - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::skill::Delete - %s",error.what());
    }
}


std::set<uint32_t> Select (ScopedConnection &conn, const uint64_t CharID)
{
    std::set<uint32_t> list;

    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "select * from skills where CharID =" << quote << CharID;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t j=0; j<res.num_rows(); ++j)
                list.insert(res[j]["ID"]);
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::skill::Select - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::skill::Select - %s",error.what());
    }

    return list;
}

}   }
