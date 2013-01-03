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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_ENTITY_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_ENTITY_H__

#include <stdint.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <pos/coordinates.h>
#include <sso/types.h>

namespace sso
{
    class Entity
    {
    public:

        Entity (float x=0.0, float y=0.0, float z=0.0, int zone = 0);

        Entity (const Coord &position);

        /**
         *
         *	@brief Set entity current channel refID.
         *
         *	@param channelID Selected channel refID.
         *
         **/

        void setChannelID (const uint32_t channelID);

        /**
         *
         *	@brief Retrieve entity current channel refID.
         *
         *	@retval Associated channel refID.
         *
         **/

        uint32_t getChannelID () const;

        /**
         *
         *	@brief Get entity global unique identifier.
         *
         *	@retval Entity associated GUID.
         *
         **/

        uint32_t GUID() const;

        /**
         *
         *	@brief Establish entity position.
         *
         *	@param position: Desired position.
         *
         **/

        void setPosition (const Coord& position);

        /**
         *
         *	@brief Retrieve entity current position.
         *
         *	@retval Position.
         *
         **/

        Coord getPosition() const;

        /**
         *
         *	@brief Establish facing angle (high precision).
         *
         *	@param angle Desired angle.
         *
         **/

        void setAngle (const uint16_t angle);

        /**
         *
         *	@brief Retrieve facing angle (high precision).
         *
         *	@retval Entity current facing angle.
         *
         **/

        uint16_t getAngle () const;

    protected:

        mutable boost::shared_mutex m_channel_mutex;

        uint32_t m_ChannelID;

        mutable boost::mutex m_mov_mutex;

        Coord m_CurrentPos;
        uint16_t m_Angle;

        uint32_t m_UniqueID;
        static uint32_t m_Count;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_ENTITY_H__
