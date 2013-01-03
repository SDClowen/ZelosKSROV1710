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

#ifndef __ZELOS_GLOBALD_LOGIN_TICKET_H__
#define __ZELOS_GLOBALD_LOGIN_TICKET_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <string>
#include <stdint.h>

struct LoginTicket
{
    struct tagID {};
    struct tagAcc {};
    struct tagShard {};

    LoginTicket (const uint32_t tID = 0, const uint32_t accID = 0, const uint16_t sID = 0)
        : ID(tID), accountID(accID), shardID(sID), expireTime(5)
    {}

    uint32_t ID;
    uint32_t accountID;
    uint16_t shardID;
    int expireTime;
};

typedef boost::multi_index::multi_index_container<
    LoginTicket,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<LoginTicket::tagID>,
                BOOST_MULTI_INDEX_MEMBER(LoginTicket,uint32_t,ID)>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<LoginTicket::tagAcc>,
                BOOST_MULTI_INDEX_MEMBER(LoginTicket,uint32_t,accountID)>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<LoginTicket::tagShard>,
                BOOST_MULTI_INDEX_MEMBER(LoginTicket,uint16_t,shardID)> >
> ticket_set;

typedef boost::multi_index::index<ticket_set,LoginTicket::tagShard>::type TIndexShard;
typedef boost::multi_index::index<ticket_set,LoginTicket::tagAcc>::type TIndexAcc;

#endif // __ZELOS_GLOBALD_LOGIN_TICKET_H__
