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

#ifndef POOL_H
#define POOL_H

#include <mysql++/cpool.h>

namespace mysqlpp
{
    class Connection;
}

namespace db
{
    class Pool : public mysqlpp::ConnectionPool
    {
    public:

        Pool (const char* db, const char* server, const char* user, const char* password);

        ~Pool ();

    protected:

        mysqlpp::Connection* create();

        void destroy (mysqlpp::Connection *);

        unsigned int max_idle_time ();

    private:

        std::string m_db;
        std::string m_server;
        std::string m_user;
        std::string m_password;
    };
}

#endif // POOL_H
