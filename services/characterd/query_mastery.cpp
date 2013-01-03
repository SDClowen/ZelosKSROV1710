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

#include "query_mastery.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace mastery {

void Create (ScopedConnection &conn, const uint64_t charID, const uint32_t mastID, const uint8_t lv)
{
    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "insert into masteries values (" << charID << "," << mastID << ","
                << quote << lv << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::mastery::Create - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::mastery::Create - %s",error.what());
    }
}

void Update (ScopedConnection &conn, const uint64_t charID, const uint32_t mastID, const uint8_t lv)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update masteries set Level = " << mysqlpp::quote << lv
                << " where CharID =" << charID << " and ID = " << (mysqlpp::sql_int)mastID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::mastery::Update- %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::mastery::Update - %s",error.what());
    }
}

std::vector<std::pair<uint32_t,uint8_t> > Select (ScopedConnection &conn, const uint64_t charID)
{
    std::vector<std::pair<uint32_t,uint8_t> > masteries;

    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "select * from masteries where CharID = " << charID;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t j = 0; j < res.num_rows(); ++j)
            masteries.push_back(std::make_pair(static_cast<uint32_t>(res[j]["ID"]),
                                               static_cast<uint8_t>(res[j]["Level"])));
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::mastery::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::mastery::Select - %s",error.what());
    }

    return masteries;
}

void Delete (ScopedConnection &conn, const uint64_t charID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from masteries where CharID =" << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::mastery::Delete - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::mastery::Delete - %s",error.what());
    }
}

}   }
