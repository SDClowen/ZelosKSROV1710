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

#include "query/query_account.h"

#include <mysql++/mysql++.h>

namespace db {
namespace account {

LOGIN_RET Authenticate (db::ScopedConnection &conn, const std::string &usr, const std::string &pw,
    const std::string &IP)
{
    mysqlpp::Query query = conn->query();

    query << "select * from accounts where username = " << mysqlpp::quote << usr
        << " and ban_expiration_date > now()";

    mysqlpp::StoreQueryResult res = query.store();

    if (res.num_rows())
        return LOGIN_BANNED;

    query.reset();

    if (!db::account::Valid(conn,usr,pw))
        return LOGIN_REJECTED;

    if (db::account::IsLogged(conn,usr))
        return LOGIN_ALREDY_LOGGED;

    query << "update accounts set last_login_ip = " << mysqlpp::quote << IP
        << " where username = " << mysqlpp::quote << usr;

    query.exec();

    query.reset();

    query << "select id from accounts where username = " << mysqlpp::quote << usr;

    res = query.store();

    if (res.num_rows())
    {
        query.reset();

        query << "insert into account_history values (" << res[0]["id"] << "," << mysqlpp::quote << IP
                << ", now())";

        query.exec();
    }

    return LOGIN_ACCEPTED;
}

bool Valid (db::ScopedConnection &conn, const std::string& user, const std::string& pass)
{
    std::string pw = user+pass+"341765";

    mysqlpp::Query query = conn->query();

    query << "select * from accounts where username = " << mysqlpp::quote
        << user << " and password = md5(" << mysqlpp::quote << pw << ")";

    mysqlpp::StoreQueryResult res = query.store();

    return res.num_rows() > 0;
}

bool IsLogged (db::ScopedConnection &conn, const std::string& name)
{
    mysqlpp::Query query = conn->query();
    query << "select * from accounts where username = " << mysqlpp::quote << name;

    mysqlpp::StoreQueryResult res = query.store();

    bool logged = false;

    if (res.num_rows())
        logged = res[0]["state"];

    return logged;
}

void writeState (db::ScopedConnection &conn, const uint32_t accountID, bool status)
{
    mysqlpp::Query query = conn->query();

    query << "update accounts set state = " << status << " where id = " << accountID;

    query.exec();
}

void writeStateClear (db::ScopedConnection &conn)
{
    mysqlpp::Query query = conn->query();

    query << "update accounts set state = 0";

    query.exec();
}

void writeSilk (db::ScopedConnection &conn, const uint32_t accountID, const uint32_t silk)
{
    mysqlpp::Query query = conn->query();

    query << "update accounts set silk= " << silk << " where id = " << mysqlpp::quote << accountID;

    query.exec();
}

uint32_t readSilk (db::ScopedConnection &conn, const uint32_t accountID)
{
    uint32_t amount = 0;

    mysqlpp::Query query = conn->query();

    query << "select silk from accounts where id = " << mysqlpp::quote << accountID;

    mysqlpp::StoreQueryResult res = query.store();

    if (res && res.num_rows())
        amount = res[0]["silk"];

    return amount;
}

mysqlpp::DateTime readBanExpirationDate (db::ScopedConnection &conn, const std::string &usr)
{
    mysqlpp::Query query = conn->query();

    query << "select ban_expiration_date from accounts where username = " << mysqlpp::quote << usr;

    mysqlpp::StoreQueryResult res = query.store();

    mysqlpp::DateTime dt;

    if (res.num_rows())
        dt = res[0]["ban_expiration_date"];

    return dt;
}

AccountProperty readAccount (db::ScopedConnection &conn, const std::string &usr)
{
    AccountProperty property;

    mysqlpp::Query query = conn->query();

    query << "select id,access_level,security_level,silk from accounts where username = "
        << mysqlpp::quote << usr;

    mysqlpp::StoreQueryResult res = query.store();

    if (res.num_rows())
    {
        property.ID = res[0]["id"];
        property.acc = res[0]["access_level"];
        property.sec = res[0]["security_level"];
        property.silk = res[0]["silk"];
    }

    return property;
}

std::string readUsername (db::ScopedConnection &conn, const uint32_t accountID)
{
    std::string usr;

    mysqlpp::Query query = conn->query();

    query << "select username from accounts where accountID =  " << accountID;

    mysqlpp::StoreQueryResult res = query.store();

    if (res.num_rows())
        usr = static_cast<std::string>(res[0]["username"]);

    return usr;
}

void issueBan (db::ScopedConnection &conn, const uint32_t accountID, const int years)
{
    mysqlpp::Query query = conn->query();

    query << "update accounts set ban_expiration_date = now() + interval " << years
            << " year where id = " << accountID;

    query.exec();
}

}   }

