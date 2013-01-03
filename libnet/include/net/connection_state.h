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

#ifndef __ZELOS_LIBNET_CONNECTION_STATE_HPP__
#define __ZELOS_LIBNET_CONNECTION_STATE_HPP__

#include <syslog.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <packet/packet.h>

enum MSG_PARSE_RESULT
{
    MSG_SUCCESS,
    MSG_ERROR,
    MSG_ERROR_OPCODE,
    MSG_ERROR_SIZE,
    MSG_ERROR_ARG,
    MSG_ERROR_ORDER
};

template <typename T>
class ConnectionState
{
public:

    typedef boost::function<int (const IPacket&)> opcode_handler;

    typedef std::map<uint32_t,opcode_handler>::iterator opcode_iterator;
    typedef std::map<uint32_t,opcode_handler>::const_iterator opcode_const_iterator;

    ConnectionState (const boost::shared_ptr<T> &conn)
        : m_connection(conn)
    {
    }

    virtual ~ConnectionState ()
    {
    }

    int Parse (const IPacket &packet)
    {
        opcode_iterator iter = m_opcode_table.find(packet.opcode());

        if (iter == m_opcode_table.end())
        {
            syslog(LOG_INFO,"srv::ConnectionState::Parse - Unknown opcode %X",packet.opcode());
            return MSG_ERROR_OPCODE;
        }

        int ret = MSG_SUCCESS;

        try
        {
            opcode_handler function = iter->second;

            ret = function(packet);
        }
        catch (const Exception::InvalidPacketSize &error)
        {
            ret = MSG_ERROR_SIZE;
        }
        catch(const std::exception& e)
        {
            syslog(LOG_INFO,"srv::ConnectionState::Parse - Exception: %s",e.what());
            ret = MSG_SUCCESS;
        }
        catch (...)
        {
            syslog(LOG_INFO,"srv::ConnectionState::Parse - Unhandled error.");
            ret = MSG_ERROR;
        }

        switch(ret)
        {
        case MSG_SUCCESS:
            break;
        case MSG_ERROR:
            syslog(LOG_ERR,"SrvClient Op %X - Error while parsing packet.",
                   packet.opcode());
            break;
        case MSG_ERROR_OPCODE:
            break;
        case MSG_ERROR_SIZE:
            syslog(LOG_ERR,"SrvClient Op %X -  Error while parsing packet. Packet size mismatch",
                   packet.opcode());
            break;
        case MSG_ERROR_ARG:
            syslog(LOG_ERR,"SrvClient Op %X- Error while parsing packet. Packet argument(s) invalid",
                   packet.opcode());
            break;
        case MSG_ERROR_ORDER:
            syslog(LOG_ERR,"SrvClient Op %X - Error while parsing packet. Packet invalid",
                   packet.opcode());
            break;
        default:
            syslog(LOG_ERR,"SrvClient Op %X - Error while parsing packet. Error number = %i",
                   ret,packet.opcode());
            break;
        }

        return ret;
    }

    virtual void Close () = 0;

protected:

    boost::shared_ptr<T> m_connection;
    std::map<uint32_t,opcode_handler> m_opcode_table;
};

#endif // __ZELOS_LIBNET_CONNECTION_STATE_HPP__
