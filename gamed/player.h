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

#ifndef __ZELOS_GAMED_PLAYER_H__
#define __ZELOS_GAMED_PLAYER_H__

#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

class IConnection;

struct Player
{
    struct tagID {};
    struct tagAgent {};
    struct tagChar {};
    struct tagGame {};
    struct tagName {};

    uint32_t accountID;
    uint32_t agentID;
    uint32_t charID;
    uint32_t gameID;
    std::string name;

    uint8_t state;
    uint32_t zoneID;

    boost::shared_ptr<IConnection> agent;
    boost::shared_ptr<IConnection> game;
};

typedef boost::multi_index::multi_index_container<
    Player,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Player::tagID>,
                BOOST_MULTI_INDEX_MEMBER(Player,uint32_t,accountID)>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<Player::tagAgent>,
                BOOST_MULTI_INDEX_MEMBER(Player,uint32_t,agentID)>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Player::tagChar>,
                BOOST_MULTI_INDEX_MEMBER(Player,uint32_t,charID)>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<Player::tagGame>,
                BOOST_MULTI_INDEX_MEMBER(Player,uint32_t,gameID)>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Player::tagName>,
                BOOST_MULTI_INDEX_MEMBER(Player,std::string,name)> >
> PlayerIndex;

typedef boost::multi_index::index<PlayerIndex,Player::tagAgent>::type PIndexAgent;
typedef boost::multi_index::index<PlayerIndex,Player::tagChar>::type PIndexChar;
typedef boost::multi_index::index<PlayerIndex,Player::tagGame>::type PIndexGame;
typedef boost::multi_index::index<PlayerIndex,Player::tagName>::type PIndexName;

#endif //__ZELOS_GAMED_PLAYER_H__
