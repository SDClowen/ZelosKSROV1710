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

#ifndef SCOPED_CONNECTION_H
#define SCOPED_CONNECTION_H

#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace mysqlpp
{
    class Connection;
};

namespace db
{
    class Pool;

    class ScopedConnection : public boost::noncopyable
    {
    public:

        ScopedConnection (const boost::weak_ptr<db::Pool> &connPool);

        ~ScopedConnection ();

        mysqlpp::Connection* operator-> () const;

    private:

        mysqlpp::Connection *m_conn;
        boost::weak_ptr<db::Pool> m_Pool;
    };

    bool IsInputValid (const std::string &str);
}

#endif // SCOPED_CONNECTION_H
