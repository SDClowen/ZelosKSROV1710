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

#include "query_block.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace block {

bool Valid (ScopedConnection &conn, const std::string &name)
{
    bool ret = false;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;

        query.escape_string(&escName,name.c_str(),name.length());

        query << "select * from characters where Name = " << mysqlpp::quote << escName;

        mysqlpp::StoreQueryResult res = query.store();

        ret = res.num_rows() > 0;
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::block::Valid - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::block::Valid - %s",error.what());
    }

    return ret;
}

bool Check (ScopedConnection &conn, const uint64_t charID, const std::string &name)
{
    bool ret = false;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;

        query.escape_string(&escName,name.c_str(),name.length());

        query << "select * from block_chat where CharID = " << charID
                << " and CharName = " << mysqlpp::quote << escName;

        mysqlpp::StoreQueryResult res = query.store();

        ret = res.num_rows() > 0;
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::block::Check - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::block::Check - %s",error.what());
    }

    return ret;
}

void Insert (ScopedConnection &conn, const uint64_t charID, const std::string &name)
{
    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        query.escape_string(&escName,name.c_str(),name.length());

        query << "insert into block_chat values ("
                << mysqlpp::quote << charID << "," << mysqlpp::quote << escName << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::block::Insert - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::block::Insert - %s",error.what());
    }
}

void Remove (ScopedConnection &conn, const uint64_t charID, const std::string &name)
{
    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        query.escape_string(&escName,name.c_str(),name.length());

        query << "delete from block_chat where CharID = " << charID
                << " and CharName = " << mysqlpp::quote << escName;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::block::Remove - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::block::Remove - %s",error.what());
    }
}

std::vector<std::string> Select (ScopedConnection &conn, const uint64_t charID)
{
    std::vector<std::string> v;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from block_chat where CharID = " << charID;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t i = 0; i < res.num_rows(); ++i)
            v.push_back(static_cast<std::string>(res[i]["CharName"]));
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::block::Select - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::block::Select - %s",error.what());
    }

    return v;
}

}   }
