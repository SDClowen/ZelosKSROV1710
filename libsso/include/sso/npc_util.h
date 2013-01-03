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

#ifndef __ZELOS_LIBSSO_SSO_NPC_UTIL_H__
#define __ZELOS_LIBSSO_SSO_NPC_UTIL_H__

#include <stdint.h>

namespace sso
{
    bool IsInteractiveNPC (const uint32_t refCharID);

    bool IsTeleportNPC (const uint32_t refCharID);

    bool IsShop (const uint32_t refCharID);

    bool IsBlacksmithShop (const uint32_t refCharID);

    bool IsProtectorShop (const uint32_t refCharID);

    bool IsSpecialShop (const uint32_t refCharID);

    bool IsStorageManager (const uint32_t refCharID);

    bool IsGuildManager (const uint32_t refCharID);

    bool IsJobManager (const uint32_t refCharID);

    bool IsJanganShop (const uint32_t refCharID);

    bool IsJanganSpecialShop (const uint32_t refCharID);

    bool IsJanganTransport (const uint32_t refCharID);

    bool IsHotanShop (const uint32_t refCharID);

    bool IsUniqueNPC (const uint32_t refCharID);
}

#endif
