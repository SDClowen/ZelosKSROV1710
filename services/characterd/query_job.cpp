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

#include "query_job.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace job {

bool Check (ScopedConnection &conn, const std::string& alias)
{
    bool ret = true;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escAlias;
        query.escape_string(&escAlias,alias.c_str(),alias.length());

        query << "select * from job where alias =" << mysqlpp::quote << escAlias;

        mysqlpp::StoreQueryResult res = query.store();

        ret = res.num_rows();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Check - %s, Errornum  = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Check - %s",error.what());
    }

    return ret;
}

void Create (ScopedConnection &conn, const uint64_t CharID, const sso::Job &job)
{
    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        std::string escAlias;
        query.escape_string(&escAlias,job.Alias.c_str(),job.Alias.length());

        query << "insert into job values ("
                << quote << CharID << ","
                << quote << escAlias << ","
                << quote << job.Type << ","
                << quote << job.Level << ","
                << quote << job.Experience << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Create - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Create - %s",error.what());
    }
}

void Delete (ScopedConnection &conn, const uint64_t CharID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from job where CharID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Delete - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Delete - %s",error.what());
    }
}

sso::Job Select (ScopedConnection &conn, const uint64_t CharID)
{
    sso::Job job;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from job where CharID = " << mysqlpp::quote << CharID;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
        {
            job.Alias = static_cast<std::string>(res[0]["alias"]);
            job.Type = res[0]["type"];
            job.Level = res[0]["level"];
            job.Experience = res[0]["exp"];
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Select - %s",error.what());
    }

    return job;
}

void Alias (ScopedConnection &conn, const uint64_t CharID, const std::string &alias)
{
    try
    {
        mysqlpp::Query query = conn->query();

        std::string escAlias;
        query.escape_string(&escAlias,alias.c_str(),alias.length());

        query << "update job set alias = " << mysqlpp::quote << escAlias << " where CharID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Alias - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Alias - %s",error.what());
    }
}

std::vector<sso::Job> Ranking (ScopedConnection &conn, const uint8_t type)
{
    std::vector<sso::Job> list;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from job where type = " << mysqlpp::quote << type
                << " and exp > 0 order by level desc, exp desc limit 0, 50";

        mysqlpp::StoreQueryResult res = query.store();

        sso::Job job;
        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            job.Alias = static_cast<std::string>(res[i]["alias"]);
            job.Level = res[i]["level"];
            job.Experience = res[i]["exp"];

            list.push_back(job);
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Ranking - %s, Errornum  = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Ranking - %s",error.what());
    }

    return list;
}

std::vector<sso::Job> Weekly (ScopedConnection &conn, const uint8_t type)
{
    std::vector<sso::Job> list;

    try
    {
        /*mysqlpp::Query query = conn.query();

        query << "select * from job where type = " << mysqlpp::quote << type
                << " and last_contribution > 0 order by last_contribution desc limit 0,50";

        mysqlpp::StoreQueryResult res = query.store();

        Job job;
        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            job.set_alias(static_cast<std::string>(res[i]["alias"]));
            job.set_level(res[i]["level"]);
            job.set_contribution(res[i]["last_contribution"]);

            list.push_back(job);
        }*/
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Weekly - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Weekly - %s",error.what());
    }

    return list;
}

uint32_t Payment (ScopedConnection &conn, const uint64_t CharID)
{
    uint32_t amount = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from job_payment where CharID = " << CharID;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t i = 0; i < res.num_rows(); ++i)
            amount += static_cast<uint32_t>(res[i]["amount"]);
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Payment - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Payment - %s",error.what());
    }

    return amount;
}

boost::unordered_map<uint32_t,sso::Market> readProducts (ScopedConnection &conn)
{
    boost::unordered_map<uint32_t,sso::Market> v;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from market";

        mysqlpp::StoreQueryResult res = query.store();

        sso::MarketStock stock;
        boost::unordered_map<uint32_t,sso::Market>::iterator iter;

        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            iter = v.find(res[i]["refCharID"]);

            stock.refItemID = res[i]["refItemID"];
            stock.MinProfit = res[i]["MinProfit"];
            stock.QuantitySold = res[i]["QuantitySold"];

            if (iter == v.end())
            {
                sso::Market market;
                market.refCharID = res[i]["refCharID"];

                std::pair<boost::unordered_map<uint32_t,sso::Market>::iterator,bool> ret;

                ret = v.insert(std::make_pair(market.refCharID,market));

                if (ret.second)
                    iter = ret.first;
            }

            if (iter != v.end())
                iter->second.Chart.insert(std::make_pair(stock.refItemID,stock));
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Market - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Market - %s",error.what());
    }

    return v;
}

void writeProducts (ScopedConnection &conn, uint32_t refCharID, int32_t refItemID, uint32_t Quantity)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update market set QuantitySold = " << Quantity << " where refCharID = " << refCharID
            << " and refItemID = " << refItemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::writeProducts - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::writeProducts - %s",error.what());
    }
}

void XP (ScopedConnection &conn, const uint64_t CharID, const uint32_t exp)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update job set exp = " << exp << " where CharID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::XP - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::XP - %s",error.what());
    }
}

void Level (ScopedConnection &conn, const uint64_t CharID, const uint8_t level)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update job set level = " << mysqlpp::quote << level << " where CharID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::job::Level - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::job::Level - %s",error.what());
    }
}

}   }
