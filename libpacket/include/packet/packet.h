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

#ifndef __ZELOS_LIBPACKET_PACKET_HPP__
#define __ZELOS_LIBPACKET_PACKET_HPP__

#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <cassert>
#include <stdint.h>

namespace Exception
{

class InvalidPacketSize
{};

}

enum ANSWER_TYPE
{
    ANSWER_ACCEPT = 0x01,
    ANSWER_ERROR = 0x02
};

struct PacketHeader
{
    // Size of this packet
    uint16_t size;

    // Opcode of this packet
    uint16_t opcode;

    // Security count byte (0 from server to client packets)
    uint8_t securityCount;

    // Security crc byte (0 from server to client packets)
    uint8_t securityCRC;
};

struct IPacket
{
    enum
    {
        HEADER_LENGTH = 6,
        BODY_MAX_LENGTH = 4096
    };

    IPacket (const uint8_t *buffer,size_t size);

    IPacket (const IPacket &packet);

    uint16_t size () const;

    uint16_t opcode () const;

    void resize ();

    template<typename T>
    T Read() const
    {
        if (index+sizeof(T) > m_data.size())
            throw Exception::InvalidPacketSize();

        T val = *(T*)(m_data.data()+index);
        index += static_cast<uint16_t>(sizeof(T));

        return val;
    }

    void Skip (uint16_t bytes) const;

    std::string Read() const;

    std::string ReadUString () const;

    bool EndOfStream () const;

    const uint8_t* buffer() const;

    uint8_t* buffer();

private:

    mutable uint16_t index;
    PacketHeader *m_header;
    std::vector<uint8_t> m_data;
};

class OPacket
{
public:

    enum
    {
        ENTRY_SIZE_POSITION,
        ENTRY_OPCODE_POSITION = 2,
        ENTRY_COUNT_POSITION = 4,
        ENTRY_CRC_POSITION = 5,
        HEADER_LENGTH = 6,
        MAX_PACKET_SIZE = 8192
    };

    enum STRING_TYPE
    {
        STRING_ASCII,
        STRING_UNICODE
    };

    OPacket();

    OPacket (const OPacket &packet);

    OPacket (const uint16_t opcode);

    uint16_t size () const;

    // Set the packet's opcode
    void WriteOpcode(const uint16_t opcode);

    void WriteOpcode (const uint16_t opcode, const ANSWER_TYPE answer);

    void WriteOpcode (const uint16_t opcode, const ANSWER_TYPE answer, const uint16_t error);

    // Set the packet's opcode
    void WriteCountSecurity(const uint8_t security);

    // Set the packet's opcode
    void WriteCrcSecurity(const uint8_t security);

    // Set the current packet size
    void WriteSize(const bool encrypted = false);

    // Get the packet buffer
    uint8_t* buffer();

    const uint8_t* buffer () const;

    template<typename T>
    void Write (const T new_data, const bool write_size = true)
    {
        uint8_t* stream = m_wpos;
        (*(T*)stream) = new_data;

        if (write_size)
            m_size += static_cast<uint16_t>(sizeof(new_data));

        m_wpos += sizeof(new_data);
    }

    template<typename OuputIterator>
    void Write (OuputIterator begin, OuputIterator end)
    {
        for ( OuputIterator i = begin; i != end; ++i)
            Write(*i);
    }

    template<typename T>
    void Write (const std::vector<T> &v)
    {
        Write(v.size());
        Write(v.begin(),v.end());
    }

    template<typename T>
    void Write(const std::list<T> &v)
    {
        Write(v.size());
        Write(v.begin(),v.end());
    }

    template<typename T>
    void Write(const std::set<T> &v)
    {
        Write(v.size());
        Write(v.begin(),v.end());
    }

    template<typename T, typename V>
    void Write(const std::pair<T,V> &p)
    {
        Write(p.first);
        Write(p.second);
    }

    void Write (float val);

    void Write (double val);

    void Write (const std::string& str, const STRING_TYPE type = STRING_ASCII);

    void Write(const char * str,const uint16_t len);

    // Clear the packet data
    void Clear();

private:

    PacketHeader *m_header;
    uint8_t data[MAX_PACKET_SIZE];
    uint8_t* m_wpos;
    uint16_t m_size;
};

#endif // __ZELOS_LIBPACKET_PACKET_HPP__
