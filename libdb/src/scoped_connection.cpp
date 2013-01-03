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

#include "db/scoped_connection.h"

#include <mysql++/connection.h>

#include "db/pool.h"

namespace db
{
    ScopedConnection::ScopedConnection (const boost::weak_ptr<db::Pool> &connPool)
        : m_conn(0), m_Pool(connPool)
    {
        boost::shared_ptr<db::Pool> pool = m_Pool.lock();

        if (pool)
        {
            m_conn = pool->grab();

            if (m_conn)
                m_conn->thread_start();
        }
    }

    ScopedConnection::~ScopedConnection ()
    {
        if (m_conn)
        {
            m_conn->thread_end();

            boost::shared_ptr<db::Pool> pool = m_Pool.lock();

            if (pool)
                pool->release(m_conn);
        }

    }

    mysqlpp::Connection* ScopedConnection::operator ->() const
    {
        return m_conn;
    }

    bool IsInputValid (const std::string &str)
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (!isalpha(str[i]) || str[i] == '\'' || str[i] == '\\' || str[i] == '"')
                return false;
        }

        return true;
    }
}
