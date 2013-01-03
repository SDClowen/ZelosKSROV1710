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

#ifndef __ZELOS_LIBNET_OPCODE_GLOBAL_GATEWAY_SRV_H__
#define __ZELOS_LIBNET_OPCODE_GLOBAL_GATEWAY_SRV_H__

enum
{
    SRV_CONFIG = 0xB001,
    SRV_ACCOUNT_PROCESS,
    SRV_NEWS_INSERT,
    SRV_NEWS_REMOVE,
    SRV_SHARD_INSERT,
    SRV_SHARD_REMOVE,
    SRV_SHARD_NOTIFY,
    SRV_PATCH_INSERT,
    SRV_PATCH_REMOVE,
    SRV_PATCH_NOTIFY
};

#endif // __ZELOS_LIBNET_OPCODE_GLOBAL_GATEWAY_SRV_H__
