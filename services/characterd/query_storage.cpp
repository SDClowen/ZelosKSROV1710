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

#include "query_storage.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace storage {

void Gold (ScopedConnection &conn, const uint64_t accountID, const uint64_t amount)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update storage_gold set Amount = " << amount << " where AccountID = " << accountID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::storage::Gold - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::storage::Gold - %s",error.what());
    }
}

uint64_t Gold (ScopedConnection &conn, const uint64_t accountID)
{
    uint64_t amount = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from storage_gold where AccountID = " << accountID;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
        {
            amount = res[0]["Amount"];
        }
        else
        {
            /// IF THERES NO ENTRY MAKE ONE
            query.reset();

            query << "insert into storage_gold values (" << accountID << "," << 0 << ")";

            query.exec();
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::storage::Gold - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::storage::Gold - %s",error.what());
    }

    return amount;
}

}	}
