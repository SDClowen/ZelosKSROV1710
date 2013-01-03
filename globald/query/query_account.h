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

#ifndef __ZELOS_GLOBALD_QUERY_QUERY_ACCOUNT_H__
#define __ZELOS_GLOBALD_QUERY_QUERY_ACCOUNT_H__

#include <stdint.h>
#include <mysql++/datetime.h>

#include <db/scoped_connection.h>

#include "account_property.h"

namespace db {
namespace account {

    enum LOGIN_RET
    {
        LOGIN_ACCEPTED,
        LOGIN_REJECTED,
        LOGIN_BANNED,
        LOGIN_ALREDY_LOGGED
    };

    LOGIN_RET Authenticate (db::ScopedConnection &conn, const std::string &name, const std::string &pass,
                            const std::string &IP);

    bool Valid (db::ScopedConnection &conn, const std::string& name, const std::string& pass);

    bool IsLogged (db::ScopedConnection &conn, const std::string& name);

    /**
     *
     *	@brief Set the account current state in the database.
     *
     *	@param conn Database connection to use.
     *	@param accountID Account ID to update.
     *	@param state Account state (0 offline, 1 online)
     *
     **/

    void writeState (db::ScopedConnection &conn, const uint32_t accountID, bool state);

    void writeStateClear (db::ScopedConnection &conn);

    void writeSilk (db::ScopedConnection &conn, const uint32_t accountID, const uint32_t silk);

    uint32_t readSilk (db::ScopedConnection &conn, const uint32_t accountID);

    mysqlpp::DateTime readBanExpirationDate (db::ScopedConnection &conn, const std::string &usr);

    AccountProperty readAccount (db::ScopedConnection &conn, const std::string &usr);

    std::string readUsername (db::ScopedConnection &conn, const uint32_t accountID);

    void issueBan (db::ScopedConnection &conn, const uint32_t accountID, const int years);

}   }

#endif // __ZELOS_GLOBALD_QUERY_QUERY_ACCOUNT_H__
