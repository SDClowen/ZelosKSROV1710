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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_COS_ATTACK_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_COS_ATTACK_H__

#include <sso/entity/cos.h>

#define MAX_HGP 9999

namespace sso
{
    enum ATTACK_COS_SETTING
    {
        ATTACK_COS_DEFENSIVE = 0x01,
    };

    enum ATTACK_COS_BACKGROUND_TASK
    {
        ACOS_BG_HGP
    };

    class AttackCOS : public COS
    {
    public:

        AttackCOS (const boost::shared_ptr<EntityProperty> &property);

        virtual ~AttackCOS ();

        void setXP (const uint64_t XP);

        uint64_t getXP () const;

        void setHGP (const uint16_t HGP);

        void healHGP (const uint16_t value);

        void consumeHGP (const uint16_t value);

        uint16_t getHGP () const;

        bool IsDefensive () const;

    private:

        uint64_t m_XP;
        uint16_t m_HGP;
        uint32_t m_Settings;
        std::string m_Name;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_COS_ATTACK_H__
