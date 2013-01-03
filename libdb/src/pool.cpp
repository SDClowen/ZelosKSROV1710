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

#include "db/pool.h"

#include <mysql++/mysql++.h>

namespace db
{
    Pool::Pool (const char *db, const char *server, const char *user, const char *password)
        :   m_db(db ? db : ""),
            m_server(server ? server : ""),
            m_user(user ? user : ""),
            m_password(password ? password : "")
    {
    }

    Pool::~Pool()
    {
        clear();
    }

    mysqlpp::Connection* Pool::create()
    {
        mysqlpp::Connection *conn = new mysqlpp::Connection(m_db.c_str(),m_server.c_str(),
                                                            m_user.c_str(),m_password.c_str());

        if (conn)
        {
#ifdef DATABASE_RECONNECT
            mysqlpp::ReconnectOption *opt = new mysqlpp::ReconnectOption(true);

            conn->set_option(opt);
#endif
        }

        return conn;
    }

    void Pool::destroy(mysqlpp::Connection *conn)
    {
        delete conn;
    }

    unsigned int Pool::max_idle_time ()
    {
        return 60000;
    }
}
