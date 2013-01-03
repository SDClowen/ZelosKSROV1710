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

#include "query_hotkey.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace hotkey {

void Insert (ScopedConnection &conn, const uint64_t CharID, const sso::Hotkey& hotkey)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "insert into hotkeys values ("
            << mysqlpp::quote << CharID << ","
            << mysqlpp::quote << hotkey.pos << ","
            << mysqlpp::quote << hotkey.type << ","
            << mysqlpp::quote << (mysqlpp::sql_int)hotkey.id << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::hotkey::Insert - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::hotkey::Insert - %s",error.what());
    }
}

void Clear (ScopedConnection &conn, const uint64_t CharID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from hotkeys where CharID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::hotkey::Clear - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::hotkey::Clear - %s",error.what());
    }
}

void Remove (ScopedConnection &conn, const uint64_t CharID, const uint8_t pos)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from hotkeys where CharID = " << CharID
            << " and pos = " << mysqlpp::quote << pos;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::hotkey::Remove - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::hotkey::Remove - %s",error.what());
    }
}

std::vector<sso::Hotkey> Select (ScopedConnection &conn, const uint64_t CharID)
{
    std::vector<sso::Hotkey> v;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from hotkeys where CharID = " << CharID;

        mysqlpp::StoreQueryResult res = query.store();

        sso::Hotkey hotkey;
        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            hotkey.pos = res[i]["pos"];
            hotkey.type = res[i]["type"];
            hotkey.id = (mysqlpp::sql_int)res[i]["id"];

            v.push_back(hotkey);
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::hotkey::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::hotkey::Select - %s",error.what());
    }

    return v;
}

}   }
