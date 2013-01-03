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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_H__

#include <stdint.h>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <db/scoped_connection.h>
#include <sso/property/item.h>

class Item;

namespace db {
namespace item {

    /**
     *
     *	@brief Insert a new item into the database.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param refID: Item reference ID.
     *	@param pos: Item current position.
     *	@param location: Item current location.
     *	@param durability: Item current durability.
     *	@param quantity: Item current quantity.
     *	@retval Item database refID.
     *
     **/

    uint64_t Create (ScopedConnection &conn, const uint64_t charID, const uint32_t refID,
                     const uint8_t pos, const uint8_t location, const uint32_t durability = 1,
                     const uint16_t quantity = 1);

    /**
     *
     *	@brief Retrieve character item list in the selected location.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param location Storage location (inventory,storage,avatar,etc..)
     *	@retval Character item properties list.
     *
     **/

    std::vector<sso::property::Item> Select (ScopedConnection &conn, const uint64_t charID,
                                             const uint8_t location);

    /**
     *
     *	@brief Update item position.
     *	@param conn Database connection.
     *	@param itemID Selected item refID.
     *	@param pos Item current position.
     *
     **/

    void Position (ScopedConnection &conn, const uint64_t itemID, const uint8_t pos);

    /**
     *
     *	@brief Update item enchant level.
     *	@param itemID Selected item refID.
     *	@param level Item current enchant level.
     *
     **/

    void EnchantLv (ScopedConnection &conn, const uint64_t itemID, const uint8_t level);

    /**
     *
     *	@brief Update item quantity.
     *	@param conn Database connection.
     *	@param itemID Selected item refID.
     *	@param quantity Item current quantity.
     *
     **/

    void Quantity (ScopedConnection &conn, const uint64_t itemID, const uint16_t quantity);

    /**
     *
     *	@brief Update item location.
     *	@param conn: Database connection.
     *	@param itemID: Selected item refID.
     *	@param ownerID: Storage owner refID.
     *	@param location: Location where is stored.
     *	@param pos: Item new position.
     *
     **/

    void Location (ScopedConnection &conn, const uint64_t itemID, const uint32_t ownerID,
                   const uint8_t location, const uint8_t pos);

    /**
     *
     *	@brief Update item current durability.
     *	@param conn Database connection.
     *	@param itemID Selected item refID.
     *	@param durability Item current durability.
     *
     **/

    void Durability (ScopedConnection &conn, const uint64_t itemID, const uint32_t durability);

    /**
     *
     *	@brief Delete an item from the database.
     *	@param conn Database connection.
     *	@param itemID Selected item refID.
     *
     **/

    void Delete (ScopedConnection &conn, const uint64_t itemID);

    /**
     *
     *	@brief Insert a magicoption into the item list.
     *	@param conn Database connection.
     *	@param itemID Selected item refID.
     *	@param optionID Magicoption ID.
     *	@param value Magicoption value.
     *
     **/

    void MagicOption (ScopedConnection &conn, const uint64_t itemID, const uint32_t optionID,
        const uint32_t value);

    /**
     *
     *	@brief Retrieve associated skill refID.
     *	@param conn Database connection.
     *	@param refName Skill reference name (Must be in refskilldata).
     *	@retval Skill refID.
     *
     **/

    uint32_t SkillID (ScopedConnection &conn, const std::string &refName);

    /**
     *
     *	@brief Retrieve associated character refID.
     *	@param conn Database connection.
     *	@param refName Character reference name (Must be in refcharacterdata).
     *	@retval Character refID.
     *
     **/

    uint32_t CharID (ScopedConnection &conn, const std::string &refName);

    /**
     *
     *	@brief Retrieve associated item refID.
     *	@param conn Database connection.
     *	@param refName Item reference name (Must be in refitemdata).
     *	@retval Item refID.
     *
     **/

    uint32_t ItemID (ScopedConnection &conn, const std::string &refName);

}   }

#endif //__ZELOS_SERVICES_CHARACTERD_QUERY_ITEM_H__
