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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_JOB_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_JOB_H__

#include <vector>
#include <boost/unordered_map.hpp>

#include <db/scoped_connection.h>
#include <sso/job.h>
#include <sso/market_stock.h>

namespace db {
namespace job {

    /**
     *
     *  @brief Check if the alias is available.
     *  @param conn Database connection.
     *  @param alias Job alias to check.
     *  @retval True if the the alias exists, false otherwise.
     *
     */

    bool Check (ScopedConnection &conn, const std::string& alias);

    /**
     *
     *	@brief Create a job for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *	@param job Job description.
     *
     **/

    void Create (ScopedConnection &conn, const uint64_t CharID, const sso::Job &job);

    /**
     *
     *	@brief Delete job for the selected character.
     *	@param conn Database connection.
     *	@param charID Selected character database ID.
     *
     **/

    void Delete (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Get character job description.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@retval Job description.
     *
     **/

    sso::Job Select (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Set character job alias.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param alias Job alias.
     *
     **/

    void Alias (ScopedConnection &conn, const uint64_t charID, const std::string &alias);

    /**
     *
     *	@brief Server job global rank list.
     *	@param conn Database connection.
     *	@param type Job type.
     *	@retval Rank list in descending order by level and experience.
     *
     **/

    std::vector<sso::Job> Ranking (ScopedConnection &conn, const uint8_t type);

    /**
     *
     *	@brief Server job weekly ranking list.
     *	@param conn Database connection.
     *	@param type Job type.
     *	@retval Rank list ordered by contribution.
     *
     **/

    std::vector<sso::Job> Weekly (ScopedConnection &conn, const uint8_t type);

    /**
     *
     *	@brief Get character pending payment.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@retval Payment amount.
     *
     **/

    uint32_t Payment (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Obtain job product prices sorted by refCharID.
     *	@param conn Database connection.
     *	@retval Product prices list ordered by refCharID.
     *
     **/

    boost::unordered_map<uint32_t,sso::Market> readProducts (ScopedConnection &conn);

    /**
     *
     *	@brief Update product total sold quantity.
     *	@param conn Database connection.
     *	@param refCharID Selected npc refID (Must be in refcharacterdata).
     *	@param refItemID Selected item refID (Must be in refitemdata).
     *	@param Quantity Total quantity sold.
     *
     **/

    void writeProducts (ScopedConnection &conn, uint32_t refCharID, int32_t refItemID, uint32_t Quantity);

    /**
     *
     *	@brief Update character job experience.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param Exp Experience amount.
     *
     **/

    void XP (ScopedConnection &conn, const uint64_t charID, const uint32_t Exp);

    /**
     *
     *	@brief Update character job level.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param level Job level.
     *
     **/

    void Level (ScopedConnection &conn, const uint64_t charID, const uint8_t level);

}   }

#endif /// __ZELOS_SERVICES_CHARACTERD_QUERY_JOB_H__
