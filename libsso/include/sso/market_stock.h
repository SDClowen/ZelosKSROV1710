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

#ifndef __ZELOS_LIBSSO_SSO_MARKET_STOCK_H__
#define __ZELOS_LIBSSO_SSO_MARKET_STOCK_H__

#include <stdint.h>
#include <map>

namespace sso
{
    struct MarketStock
    {
        uint32_t refItemID;
        int16_t MaxProfit;
        int16_t MinProfit;
        uint32_t QuantitySold;
    };

    struct Market
    {
        uint32_t refCharID;
        uint64_t Max;
        std::map<uint32_t,MarketStock> Chart;
    };

    float Profit (const MarketStock &stock, uint16_t Quantity, uint32_t QuantitySold, uint64_t MaxSold);
}

#endif // __ZELOS_LIBSSO_SSO_MARKET_STOCK_H__
