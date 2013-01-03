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

#ifndef DEF_PKT_ITEM_H
#define DEF_PKT_ITEM_H

namespace pkt
{
    enum ITEM_UPDATE_RESULT
    {
        ITEM_UPDATE_SUCCESS = 1,
        ITEM_UPDATE_FAIL
    };

    enum CAPE_TYPE
    {
        CAPE_TYPE_PVP = 1,
        CAPE_TYPE_JOB
    };

    enum ITEM_ERROR
    {
        ITEM_ERROR_INVENTORY_FULL = 0x1807,
        ITEM_ERROR_INSUFFICIENT_GOLD = 0x180F,
        ITEM_ERROR_PICKUP_DIFFERENT_OWNER = 0x1812,
        ITEM_ERROR_DIFFERENT_SEX = 0x1816,
        ITEM_ERROR_EXCHANGE_WINDOW_FULL = 0x1821,
        ITEM_ERROR_EXCHANGE_INVENTORY_FULL = 0x1822,
        ITEM_ERROR_DIFFERENT_RACE = 0x182F,
        ITEM_ERROR_DIFFERENT_TYPE = 0x1832,
        ITEM_ERROR_EXCHANGE_PEER_INV_FULL = 0x1836,
        ITEM_ERROR_BROKEN_ITEM = 0x1837,
        ITEM_ERROR_DROP_INDISPOSABLE = 0x1838,
        ITEM_ERROR_ITEM_TRADE = 0x183A,
        ITEM_ERROR_EQUIP_JOB_CAPE_WHILE_JOBBING = 0x1846,
        ITEM_ERROR_SUMMON_LIMIT = 0x185C,
        ITEM_ERROR_PICKUP_OTHER_PLAYER_TRADE_ITEM = 0x1863,
        ITEM_ERROR_PICKUP_TRADE_FAR_FROM_TRANSPORT = 0x1865,
        ITEM_ERROR_LOWER_LV = 0x186C,
        ITEM_ERROR_DROP_EQUIPPED_ITEM = 0x186D,
        ITEM_ERROR_COS_SUMMON_ATTACK_STATE = 0x1878,
        ITEM_ERROR_COS_CANNOT_SUMMON_INSIDE_DUNGEON = 0x1879,
        ITEM_ERROR_BUY_INSUFFICIENT_SILK = 0x1880,
        ITEM_ERROR_TRANSGENDER_HIGHER_DEGREE = 0x1883,
        ITEM_ERROR_TRANSGENDER_UNAVAILABLE = 0x1884,
        ITEM_ERROR_USE_INVALID_RECALL_POINT = 0x1885,
        ITEM_ERROR_USE_INVALID_DEATH_POINT = 0x1886,
        ITEM_ERROR_USE_ONLY_WHILE_DEAD = 0x1887,
        ITEM_ERROR_USE_WHILE_DEAD = 0x1889,
        ITEM_ERROR_TRANSGEND_ITEM_EQUIPPED = 0x1892,
        ITEM_ERROR_BUFF_STACKING = 0x1894,
        ITEM_ERROR_NEED_JOB = 0x189E,
        ITEM_ERROR_EQUIP_JOB_CAPE_NEED_ALIAS = 0x189F,
        ITEM_ERROR_JOB_CAPE_ON_PARTY = 0x18A0,
        ITEM_ERROR_JOB_CAPE_ONLY_TOWN = 0x18A2,
        ITEM_ERROR_COS_STATE_IS_NOT_SUMMONABLE = 0x18A4,
        ITEM_ERROR_COS_HIGHER_LEVEL_REQUIRED_TO_USE_THISITEM = 0x18A5,
        ITEM_ERROR_COS_CANT_SUMMON_DUPLICATE_PETTYPE = 0x18A9,
        ITEM_ERROR_COS_STORE_SUMMONED = 0x18B2,
        ITEM_ERROR_COS_HGPFULL_NODRINK = 0x18B3,
        ITEM_ERROR_COS_TRADE_SUMMONED = 0x18B6,
        ITEM_ERROR_COS_SELL_SUMMONED = 0x18B7,
        ITEM_ERROR_BUY_NOT_QUALIFIED = 0x18C4,
        ITEM_ERROR_JOB_CAPE_ON_TRANSPORT = 0x18CA,
        ITEM_ERROR_INSUFFICIENT_GP = 0x18C5,
        ITEM_ERROR_EQUIP_AWAKEN_NASRUN = 0x18F8,
        ITEM_ERROR_LIMIT_REACHED = 0x18FE
    };

    enum ALCHEMY_ACTION_TYPE
    {
        ALCHEMY_MATERIAL_RONDO = 1,
        ALCHEMY_STONE_CREATE
    };

    /**
     *
     *  IN = INVENTORY
     *  ST = STORAGE
     *  EX = EXCHANGE WINDOW
     *  SH = SHOP
     *  IM = ITEM MALL
     *  COS = TRANSPORT & PETS
     *  GD = GUILD
     *  AV = AVATAR
     *	I = ITEM
     *	G = GOLD
     *
     **/

    enum ITEM_MOVE_ACTION
    {
        ITEM_MOVE_INV_TO_INV,
        ITEM_MOVE_ST_TO_ST,
        ITEM_MOVE_INV_TO_ST,
        ITEM_MOVE_ST_TO_INV,
        ITEM_MOVE_INV_TO_EX,
        ITEM_MOVE_EX_TO_INV,
        ITEM_MOVE_FLOOR_TO_INV,
        ITEM_MOVE_INV_TO_FLOOR_I,
        ITEM_MOVE_NPC_TO_INV,
        ITEM_MOVE_INV_TO_NPC,
        ITEM_MOVE_INV_TO_FLOOR_G,
        ITEM_MOVE_ST_WITHDRAW_G,
        ITEM_MOVE_ST_DEPOSIT_G,
        ITEM_MOVE_EX_G,
        ITEM_MOVE_CREATE,
        ITEM_MOVE_REMOVE,
        ITEM_MOVE_COS_TO_COS,
        ITEM_MOVE_NPC_TO_COS = 0x13,
        ITEM_MOVE_COS_TO_NPC,
        ITEM_MOVE_MALL_TO_INV = 0x18,
        ITEM_MOVE_COS_TO_INV = 0x1A,
        ITEM_MOVE_INV_TO_COS = 0x1B,
        ITEM_MOVE_GUILD_ST_TO_GUILD_ST = 0x1D,
        ITEM_MOVE_INV_TO_GUILD_ST,
        ITEM_MOVE_GUILD_ST_TO_INV,
        ITEM_MOVE_GUILD_ST_DEPOSIT_G,
        ITEM_MOVE_GUILD_ST_WITHDRAW_G,
        ITEM_MOVE_INV_TO_NPC_UNDO,
        ITEM_MOVE_AVA_TO_INV,
        ITEM_MOVE_INV_TO_AVA
    };

    enum ITEM_NOTIFY
    {
        ITEM_NOTIFY_MORPH = 0x01,
        ITEM_NOTIFY_QUANTITY = 0x08,
        ITEM_NOTIFY_STATE = 0x40,
        ITEM_NOTIFY_DURATION = 0x80
    };

}

#endif // DEF_PKT_ITEM_H
