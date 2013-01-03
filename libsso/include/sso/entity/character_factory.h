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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_FACTORY_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_FACTORY_H__

#include <map>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include <sso/property/player.h>

class EntityProperty;

namespace sso
{
    class ICharacter;
    typedef boost::shared_ptr<ICharacter> ICharPtr;

    class CharacterFactory
    {
    public:

        typedef boost::unordered_map<uint32_t,boost::shared_ptr<EntityProperty> > property_map;

        typedef std::multimap<uint32_t,std::pair<uint8_t,uint8_t> > quality_map;

    public:

        CharacterFactory (const property_map &property_list);

        CharacterFactory (const property_map &property_list, const quality_map &quality_chart);

        ICharPtr Create (const uint32_t ID) const;

        ICharPtr Create (const uint32_t refCharID, uint8_t quality) const;

        ICharPtr Create (const sso::property::Player &property) const;

        ICharPtr Create (const uint32_t ID, const uint8_t volume, const std::string &name) const;

        boost::shared_ptr<EntityProperty> getProperty (const uint32_t refCharID) const;

    private:

        quality_map m_quality_chart;
        property_map m_property_list;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_CHARACTER_FACTORY_H__
