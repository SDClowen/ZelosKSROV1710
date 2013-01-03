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

#ifndef __ZELOS_AGENTD_CLIENT_H__
#define __ZELOS_AGENTD_CLIENT_H__

#include <stdint.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/shared_ptr.hpp>

class IConnection;

struct Client
{
    struct tagID {};
    struct tagAcc {};
    struct tagArea {};

    uint32_t connID;
    uint32_t accountID;
    uint32_t areaID;
    boost::shared_ptr<IConnection> conn;
};

typedef boost::multi_index::multi_index_container<
    Client,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Client::tagID>,
                BOOST_MULTI_INDEX_MEMBER(Client,uint32_t,connID)>,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Client::tagAcc>,
                BOOST_MULTI_INDEX_MEMBER(Client,uint32_t,accountID)>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<Client::tagArea>,
                BOOST_MULTI_INDEX_MEMBER(Client,uint32_t,areaID)> >
> client_set;

typedef boost::multi_index::index<client_set,Client::tagAcc>::type CIndexAcc;
typedef boost::multi_index::index<client_set,Client::tagArea>::type CIndexArea;

#endif // __ZELOS_AGENTD_CLIENT_H__
