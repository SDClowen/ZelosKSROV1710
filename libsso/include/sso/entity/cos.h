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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_COS_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_COS_H__

#include <sso/entity/character.h>

namespace sso
{
    class Player;
    class Storage;

    class COS : public ICharacter
    {
    public:

        COS (const boost::shared_ptr<EntityProperty> &property);

        virtual ~COS ();

        virtual uint8_t getLevel () const;

        void setDatabaseID (const uint64_t ID);

        uint64_t getDatabaseID () const;

        void setOwner (const boost::shared_ptr<Player> &player);

        boost::shared_ptr<Player> getOwner () const;

        void setOwnerSlot (const uint8_t slot);

        uint8_t getOwnerSlot () const;

        boost::shared_ptr<Storage> getInventory () const;

        void setName (const std::string &name);

        const std::string& getName () const;

        void setSettings (const uint32_t settings);

        uint32_t getSettings () const;

    protected:

        mutable boost::mutex m_cos_mutex;

        uint32_t m_DBID;
        uint8_t m_OwnerSlot;
        boost::shared_ptr<Player> m_Owner;
        boost::shared_ptr<Storage> m_Inventory;

        std::string m_Name;
        uint32_t m_Settings;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_COS_H__
