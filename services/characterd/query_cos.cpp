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

#include "query_cos.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

#include <db/pool.h>

namespace db {
namespace cos {

uint64_t Create (ScopedConnection &conn, const sso::property::COS &attr)
{
    uint64_t refID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "insert into character_cos (State,HP,HGP,XP,RemainingTime,Settings) values ("
                << mysqlpp::quote << attr.state << "," << attr.HP << "," << attr.HGP << ","
                << attr.XP << "," << attr.Minutes << "," << attr.settings << ")";

        query.exec();

        refID = query.insert_id();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::Create - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::Create - %s",error.what());
    }

    return refID;
}

sso::property::COS Select (ScopedConnection &conn, const uint32_t cosID)
{
    sso::property::COS attr;
    attr.ID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from character_cos where ID = " << cosID;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
        {
            attr.ID = res[0]["ID"];
            attr.state = res[0]["State"];
            attr.HP = res[0]["HP"];
            attr.HGP = res[0]["HGP"];
            attr.XP = res[0]["XP"];
            attr.Minutes = res[0]["RemainingTime"];
            attr.settings = res[0]["Settings"];
        }

        query.reset();

        query << "select * from character_cos_name where refID = " << cosID;

        res = query.store();

        if (res.num_rows())
            attr.name = static_cast<std::string>(res[0]["Name"]);
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::Select - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::Select - %s",error.what());
    }

    return attr;
}

void HGP (ScopedConnection &conn, const uint32_t cosID, const uint16_t value)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update character_cos set HGP = " << value << " where ID = " << cosID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::HGP - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::HGP - %s",error.what());
    }
}

bool IsNameValid (ScopedConnection &conn, const std::string &name)
{
    if (!db::IsInputValid(name))
        return false;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        escName.reserve(2*name.length()+1);
        query.escape_string(&escName,name.c_str(),name.length());

        query << "select count(*) from character_cos_name where Name = " << mysqlpp::quote << escName;

        mysqlpp::StoreQueryResult res = query.store();

        return !res.num_rows();
    }
    catch (mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::IsNameValid - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::IsNameValid - %s",error.what());
    }

    return false;
}

void Name (ScopedConnection &conn, const uint32_t cosID, const std::string &name)
{
    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        escName.reserve(2*name.length()+1);
        query.escape_string(&escName,name.c_str(),name.length());

        query << "update character_cos_name set Name = " << mysqlpp::quote << escName
                << " where refID = " << cosID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::Name - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::Name - %s",error.what());
    }
}

void Settings (ScopedConnection &conn, const uint32_t cosID, const uint32_t settings)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update character_cos set Settings = " << settings << " where ID = " << cosID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::cos::Settings - %s, ErrorNum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::cos::Settings - %s",error.what());
    }
}

}	}
