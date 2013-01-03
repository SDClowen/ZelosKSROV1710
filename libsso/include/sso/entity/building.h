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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_BUILDING_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_BUILDING_H__

#include <vector>

#include <khamaileon/property/building.h>
#include <sso/entity/entity.h>

namespace sso
{
    enum TELEPORT_TASKS
    {
        TELEPORT_TASK_DESIGNATE = 7,
        TELEPORT_TASK_TELEPORT,
        TELEPORT_TASK_TELEPORT_RETURN_POINT
    };

    struct TeleportBuilding : public Entity
    {
        TeleportBuilding ()
            : TeleportID(0), TaxRate(0), TaxActive(false)
        {}

        uint16_t TeleportID;
        uint16_t TaxRate;
        bool TaxActive;
        BuildingProperty m_Property;

        std::vector<uint8_t> Tasks;
    };
}

#endif // __ZELOS_LIBSSO_SSO_ENTITY_BUILDING_H__
