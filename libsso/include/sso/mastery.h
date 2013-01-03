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

#ifndef __ZELOS_LIBSSO_SSO_MASTERY_H__
#define __ZELOS_LIBSSO_SSO_MASTERY_H__

#include <map>
#include <stdint.h>

#include <sso/types.h>

namespace sso
{
    enum MASTERY_TYPES
    {
        MASTERY_CH_SWORD = 257,
        MASTERY_CH_SPEAR,
        MASTERY_CH_BOW,
        MASTERY_CH_COLD = 273,
        MASTERY_CH_FIRE,
        MASTERY_CH_LIGHT,
        MASTERY_CH_WATER,

        MASTERY_EU_WARRIOR = 513,
        MASTERY_EU_WIZARD,
        MASTERY_EU_ROGUE,
        MASTERY_EU_WARLOCK,
        MASTERY_EU_BARD,
        MASTERY_EU_CLERIC
    };

    /**
     *
     *  CHARACTER MASTERY TREE OBJECT
     *
     *  CHINESE RACE MASTERY LIMIT IS 330
     *  EUROPEAN RACE MASTERY LIMIT IS CHARACTER_LEVEL * 2
     *
     **/

    class MasteryTree
    {
    public:

        typedef std::pair<uint32_t,uint8_t> Mastery;

        typedef std::map<uint32_t,uint8_t>::iterator iterator;
        typedef std::map<uint32_t,uint8_t>::const_iterator const_iterator;

        MasteryTree ();

        MasteryTree (const std::map<uint32_t,uint8_t> &tree);

        int getLimit (const RACE race, const uint8_t lv) const;

        int getTotal () const;

        int getBonus (const uint32_t masteryID) const;

        const std::map<uint32_t,uint8_t>& getMasteries () const;

        bool IsMax (const RACE race, const uint8_t lv) const;

        /**
         *
         *  RETURN THE 2 HIGHEST MASTERY IN THE TREE
         *
         **/

        std::pair<Mastery,Mastery> getHighests () const;

        static MasteryTree Create (const RACE race);

    private:

        std::map<uint32_t,uint8_t> m_tree;
    };
}

#endif //__SLES_MASTERY_TREE_HPP__
