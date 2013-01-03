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

#ifndef __ZELOS_SERVICES_CHARACTERD_QUERY_PLAYER_H__
#define __ZELOS_SERVICES_CHARACTERD_QUERY_PLAYER_H__

#include <db/scoped_connection.h>
#include <sso/property/character.h>

namespace db {
namespace player {

    /**
     *
     *	@brief Retrieve character associated account ID.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@retval Associated accountID to this character.
     *
     **/

    uint32_t AccountID (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Check if the character name is available.
     *	@param conn Database connection.
     *	@param name Character name.
     *	@retval True if available, false otherwise.
     *
     **/

    bool Check (ScopedConnection &conn, const std::string& name);

    /**
     *
     *	@brief Check the current amount of character associated with an account.
     *	@param conn: Database connection.
     *	@param accountID: Account to retrieve characters.
     *	@retval Amount of characters associated with the account.
     *
     **/

    int Count (ScopedConnection &conn, const uint32_t accountID);

    /**
     *
     *	@brief Retrieve character list for the selected account.
     *	@param conn Database connection.
     *	@param AccountID Associated account refID.
     *	@retval Characters property list.
     *
     **/

    std::vector<sso::CharacterView> Select (ScopedConnection &conn, const uint32_t AccountID);

    sso::CharacterInfo Select (ScopedConnection &conn, const uint32_t accountID, const std::string &name);

    /**
     *
     *	@brief Create a new character in the database.
     *
     *	@param conn Database connection.
     *	@param AccountID Associated account refID.
     *	@param player Character to insert.
     *	@param name Character current name.
     *
     **/

    uint32_t Create (ScopedConnection &conn, const uint32_t accountID, const uint32_t refID,
                     const std::string &name, const uint8_t scale, const uint16_t zoneID,
                     const float x, const float y, const float z, const uint32_t returnID);

    /**
     *
     *	@brief Mark character to be erased in 5 mins.
     *	@param conn: Database connection.
     *	@param accountID: Associated account.
     *	@param name: Character name to delete.
     *
     **/

    void Erase (ScopedConnection &conn, const uint32_t accountID, const std::string &name);

    /**
     *
     *	@brief Unmark a character to be erased.
     *	@param conn: Database connection.
     *	@param accountID: Associated account.
     *	@param name: Character name to delete.
     *
     **/

    void Restore (ScopedConnection &conn, const uint32_t accountID, const std::string &name);

    /**
     *
     *	@brief Delete a character from the database.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *
     **/

    void Delete (ScopedConnection &conn, const uint64_t charID);

    /**
     *
     *	@brief Update player actual position in world.
     *	@param conn: Dabase connection.
     *	@param charID: Selected character refID.
     *	@param zoneID: Character current zone.
     *	@param x: X position.
     *	@param y: Y position.
     *	@param z: Z position.
     *
     **/

    void Position (ScopedConnection &conn, const uint64_t charID, const int16_t zoneID,
                   const float x, const float y, const float z);

    /**
     *
     *	@brief Update character facing angle.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param angle Character facing angle.
     *
     **/

    void Angle (ScopedConnection &conn, const uint64_t charID, const uint16_t angle);

    /**
     *
     *	@brief Update character current skill points.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param points Character current skill points.
     *
     **/

    void SP (ScopedConnection &conn, const uint64_t charID, const uint32_t points);

    /**
     *
     *	@brief Update character current experience points.
     *	@param conn Database connection.
     *	@param charID Selected characte refID.
     *	@param exp Character accumulated experience points.
     *
     **/

    void Exp (ScopedConnection &conn, const uint64_t charID, const uint64_t exp);

    /**
     *
     *	@brief Update character current skill experience points.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param exp Character accumulated skill experience points.
     *
     **/

    void SPExp (ScopedConnection &conn, const uint64_t charID, const uint16_t exp);

    /**
     *
     *	@brief Update character current stats (strenght,intelligence,remaining stat points).
     *	@param conn: Database connection.
     *	@param charID: Selected character refID.
     *	@param stats: Current stats points.
     *
     **/

    void Stats (ScopedConnection &conn, const uint64_t charID, const uint16_t stats);

    /**
     *
     *	@brief Update character current stats (strenght,intelligence,remaining stat points).
     *	@param conn: Database connection.
     *	@param charID: Selected character refID.
     *	@param stats: Current stats points.
     *	@param sStr: Current strength points.
     *
     **/

    void StatsStr (ScopedConnection &conn, const uint64_t charID, const uint16_t stats,
                   const uint16_t sStr);

    /**
     *
     *	@brief Update character current stats (strenght,intelligence,remaining stat points).
     *	@param conn: Database connection.
     *	@param charID: Selected character refID.
     *	@param stats: Current stats points.
     *	@param sInt: Current intelligence points.
     *
     **/

    void StatsInt (ScopedConnection &conn, const uint64_t charID, const uint16_t stats,
                   const uint16_t sInt);

    /**
     *
     *	@brief Update character current level and max level.
     *	@param conn Database connection.
     *	@param player Character.
     *
     **/

    void Lvl (ScopedConnection &conn, const uint64_t charID, const uint8_t curLv, const uint8_t maxLv);

    /**
     *
     *	@brief Update character current gold.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param gold Character current gold amount.
     *
     **/

    void Gold (ScopedConnection &conn, const uint64_t charID, const uint64_t gold);

    /**
     *
     *	@brief Update character return point.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param teleportID Selected return point refID.
     *
     **/

    void ReturnPoint (ScopedConnection &conn, const uint64_t charID, const uint32_t teleportID);

    /**
     *
     *	@brief Update character health.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param hp Character current hit points.
     *	@param mp Character current magic points.
     *
     **/

    void Health (ScopedConnection &conn, const uint64_t charID, const uint32_t hp, const uint32_t mp);

    /**
     *
     *	@brief Update character berserk orbs.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param count Character current berserk orb count.
     *
     **/

    void Berserk (ScopedConnection &conn, const uint64_t charID, const uint8_t count);

    /**
     *
     *	@brief Update character state.
     *	@param conn Database connection.
     *	@param state Character state (true online, false offline).
     *
     **/

    void State (ScopedConnection &conn, const uint64_t charID, bool state);

    /**
     *
     *	@brief Update character current guild.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param GuildID Character current guild refID.
     *
     **/

    void GuildID (ScopedConnection &conn, const uint64_t charID, const uint32_t GuildID);

    /**
     *
     *	@brief Update character stall avatar.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param avatarID Stall avatar refItemID (Must be in refitemdata).
     *
     **/

    void StallAvatar (ScopedConnection &conn, const uint64_t charID, const uint32_t avatarID);

    /**
     *
     *	@brief Update character autopot settings.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param settings Character autopot settings.
     *
     **/

    void Autopot (ScopedConnection &conn, const uint64_t charID, const sso::Autopot &settings);

    /**
     *
     *	@brief Update character inventory max capacity.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param size Inventory max capacity.
     *
     **/

    void InventoryCapacity (ScopedConnection &conn, const uint64_t charID, const uint8_t size);

    /**
     *
     *	@brief Update character storage max capacity.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param size Storage max capacity.
     *
     **/

    void StorageCapacity (ScopedConnection &conn, const uint64_t charID, const uint8_t size);

    /**
     *
     *	@brief Update character model and volume.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param refCharID Character current model (Must be in refcharacterdata).
     *	@param volume Character current volume.
     *
     **/

    void Skin (ScopedConnection &conn, const uint64_t charID, const uint32_t refcharID,
        const uint8_t volume);

    /**
     *
     *	@brief Update character tutorial displayed list.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param tutorials Character current tutorial display list (bitfields).
     *
     **/

    void Tutorial (ScopedConnection &conn, const uint64_t charID, const uint64_t tutorials);

    /**
     *
     *	@brief Update character PvP team.
     *	@param conn Database connection.
     *	@param charID Selected character refID.
     *	@param cape Character current PvP team.
     *
     **/

    void Cape(ScopedConnection &conn, const uint64_t charID, const uint8_t cape);

}	}

#endif // __ZELOS_SERVICES_CHARACTERD_QUERY_PLAYER_H__
