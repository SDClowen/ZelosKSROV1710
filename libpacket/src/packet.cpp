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

#include "packet/packet.h"

IPacket::IPacket(const uint8_t *buffer, size_t size)
    : index(HEADER_LENGTH), m_data(size)
{
    memcpy(m_data.data(),buffer,size);
    m_header = (PacketHeader*)(m_data.data());
}

IPacket::IPacket (const IPacket &packet)
    : index(HEADER_LENGTH),m_data(packet.m_data)
{
    m_header = (PacketHeader*)(m_data.data());
}

uint16_t IPacket::size () const
{
    return m_header->size;
}

uint16_t IPacket::opcode () const
{
    return m_header->opcode;
}

void IPacket::resize ()
{
    m_data.resize(m_header->size+HEADER_LENGTH);
}

void IPacket::Skip (uint16_t bytes) const
{
    index += bytes;
}

std::string IPacket::Read() const
{
    uint16_t len = Read<uint16_t>();

    if (index+len > m_data.size())
        throw Exception::InvalidPacketSize();

    std::string str(m_data.data()+index,m_data.data()+index+len);
    index += len;

    return str;
}

std::string IPacket::ReadUString () const
{
    uint16_t len = Read<uint16_t>()*2;

    if (index+len > m_data.size())
        throw Exception::InvalidPacketSize();

    std::string str(m_data.data()+index,m_data.data()+index+len);
    index += len;

    return str;
}

bool IPacket::EndOfStream () const
{
    return index == m_data.size();
}

const uint8_t* IPacket::buffer() const
{
    return m_data.data();
}

uint8_t* IPacket::buffer()
{
    return m_data.data();
}

OPacket::OPacket ()
{
    m_header = reinterpret_cast<PacketHeader*>(data);

    Clear();
}

OPacket::OPacket (const OPacket &packet)
{
    memcpy(data,packet.data,MAX_PACKET_SIZE);

    m_header = reinterpret_cast<PacketHeader*>(data);
}

OPacket::OPacket (const uint16_t opcode)
{
    m_header = reinterpret_cast<PacketHeader*>(data);

    Clear();

    WriteOpcode(opcode);
}

uint16_t OPacket::size () const
{
    return m_size & 0x7FFF;
}

void OPacket::WriteOpcode(const uint16_t opcode)
{
    m_header->opcode = opcode;
}

void OPacket::WriteOpcode (const uint16_t opcode, const ANSWER_TYPE answer)
{
    WriteOpcode(opcode);
    Write<uint8_t>(answer);
}

void OPacket::WriteOpcode (const uint16_t opcode, const ANSWER_TYPE answer, const uint16_t error)
{
    WriteOpcode(opcode);
    Write<uint8_t>(answer);
    Write<uint16_t>(error);
}

void OPacket::WriteCountSecurity(const uint8_t security)
{
    m_header->securityCount = security;
}

void OPacket::WriteCrcSecurity(const uint8_t security)
{
    m_header->securityCRC = security;
}

void OPacket::WriteSize(const bool encrypted)
{
    uint16_t size = m_size - HEADER_LENGTH;

    if(encrypted)
        size |= 0x8000;

    m_header->size = size;
}

uint8_t* OPacket::buffer()
{
    return (uint8_t*)data;
}

const uint8_t* OPacket::buffer () const
{
    return (const uint8_t*)data;
}

void OPacket::Write (float val)
{
    uint8_t* stream = m_wpos;
    (*(float*)stream) = val;

    m_size += sizeof(float);

    m_wpos += sizeof(float);
}

void OPacket::Write (double val)
{
    uint8_t* stream = m_wpos;
    (*(double*)stream) = val;

    m_size += sizeof(double);

    m_wpos += sizeof(double);
}

void OPacket::Write (const std::string& str, const STRING_TYPE type)
{
    if ( type == STRING_ASCII)
        Write<uint16_t>(str.length());
    else
        Write<uint16_t>(str.length()/2);

    Write(str.c_str(),str.length());
}

void OPacket::Write(const char * str,const uint16_t len)
{
    uint8_t* stream = (uint8_t*)m_wpos;
    memcpy((char*)stream, str,len);
    m_size += (uint16_t)len;
    m_wpos += len;
}

void OPacket::Clear()
{
    m_header->opcode = 0;
    m_header->securityCount = 0;
    m_header->securityCRC = 0;
    m_header->size = 0;

    m_size = 6;
    m_wpos = data + m_size;
}
