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

#ifndef __ZELOS_GAMED_CLIENT_H__
#define __ZELOS_GAMED_CLIENT_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

class IConnection;

struct Client
{
    struct tagAcc {};
    struct tagAgent {};

    Client ()
        :  accountID(0), agentID(0), ingameRequest(false)
    {}

    uint32_t accountID;
    uint32_t agentID;
    bool ingameRequest;
    boost::shared_ptr<IConnection> agent;
};

typedef boost::multi_index::multi_index_container<
    Client,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::tag<Client::tagAcc>,
                BOOST_MULTI_INDEX_MEMBER(Client,uint32_t,accountID)>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<Client::tagAgent>,
                BOOST_MULTI_INDEX_MEMBER(Client,uint32_t,agentID)> >
> client_index;

typedef boost::multi_index::index<client_index,Client::tagAgent>::type CIndexAgent;

#endif // __ZELOS_GAMED_CLIENT_H__
