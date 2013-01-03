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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_COS_TRANSPORT_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_COS_TRANSPORT_H__

#include <sso/entity/cos.h>

namespace sso
{
    enum TRANSPORT_COS_BACKGROUND_TASK
    {
        TRANSPORT_BG_JOB_SPAWN
    };

    class Transport : public COS
    {
    public:

        Transport (const boost::shared_ptr<EntityProperty> &property);

        virtual ~Transport ();

        bool has_goods () const;

        bool IsMounted () const;

        void Mount ();

        void Dismount ();

        uint8_t TradeScale () const;

        void CalculateTradeScale ();

    private:

        bool m_Mounted;

        uint8_t m_TradeScale;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_COS_TRANSPORT_H__
