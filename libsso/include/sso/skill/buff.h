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

#ifndef __ZELOS_LIBSSO_SSO_SKILL_BUFF_H__
#define __ZELOS_LIBSSO_SSO_SKILL_BUFF_H__

#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <stdint.h>
#include <vector>

#include <sso/skill/requirement.h>

namespace sso
{
    /**
     *
     *	@brief Buff tag identifiers.
     *
     **/

    enum SKILL_BUFF
    {
        BUFF_NONE,
        BUFF_CRITICAL_INC = 25458,
        BUFF_PARRY_INC = 25970,
        BUFF_BLOCK_INC = 25202,
        BUFF_HIT_INC = 26738,
        BUFF_RANGE_INC = 29301,
        BUFF_DAMAGE_INC = 6582901,
        BUFF_GOLD_DROP_INC = 6775922,
        BUFF_HP_INC = 6844521,
        BUFF_MP_INC = 7172201,
        BUFF_MAGICOPTION_LUCK_INC = 1634493301,
        BUFF_DAMAGE_PWR_INC = 1634754933,
        BUFF_CRIT_PARRY_INC = 1684238953,
        BUFF_DEFENSE_INC = 1684366960,
        BUFF_SPEED_INC2 = 1752396850,
        BUFF_SPEED_INC3 = 1752396851,
        BUFF_SPEED_INC = 1752396901,
        BUFF_INT_INC = 1768846441,
        BUFF_ENCHANT_LUCK_INC = 1819632491,
        BUFF_IGNORE_DEFENSE = 1836280164,
        BUFF_STR_INC = 1937011305,
        BUFF_PARRY_DEC= 1952805476,
        BUFF_HIT_DEC = 1953002084,
        BUFF_SHIELD_PWR_4_DAMAGE = 1936745569,
        BUFF_REINCARNATION = 1769105251,
        BUFF_MP_ABSORB = 1684499824,
        BUFF_DETECT = 6583412,
        BUFF_DETECT_V2 = 1685353584,
        BUFF_ABSORB_WALL = 28791,
        BUFF_REQUIRED_MP_DEC = 1684237680,
        BUFF_DAMAGE_REFLECT = 1684891506,
        BUFF_HEAL_INC = 1919447669,
        BUFF_ABSORB = 1868849522,
        BUFF_HP_DEC = 1886218352,
        BUFF_DAMAGE_DEC = 1886217319,
        BUFF_DEFENSE_DEC = 1886217328
    };

    enum BUFF_WALL_TYPE
    {
        WALL_TYPE_PHYSICAL = 7,
        WALL_TYPE_MAGICAL = 11
    };

    struct BuffEffect
    {
        BuffEffect ()
            : ID(BUFF_NONE)
        {
            Arg[0] = Arg[1] = Arg[2] = Arg[3] = 0;
        }

        SKILL_BUFF ID;
        boost::array<int16_t,4> Arg;
    };

    class Skill;

    struct Buff
    {
        Buff ()
            : CastID(0), active(true),
            persistent(false)
        {}

        uint32_t taskID;
        uint32_t CastID;

        bool active;
        bool persistent;

        boost::shared_ptr<Skill> skill;
    };

    enum SKILL_BUFF_END
    {
        BUFF_END_HP_LOST = 1667785586,
        BUFF_END_MP_LOST = 1668113266
    };

    struct BuffEnd
    {
        SKILL_BUFF_END ID;
        int16_t Arg0;
    };
}
#endif //__ZELOS_LIBSSO_SSO_SKILL_BUFF_H__
