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

#ifndef __ZELOS_LIBKHAMAILEON_KHAMAILEON_PARSER_SIEGEFORTRESS_H__
#define __ZELOS_LIBKHAMAILEON_KHAMAILEON_PARSER_SIEGEFORTRESS_H__

#include <vector>
#include <boost/function.hpp>

#include <khamaileon/property/fortress.h>

void ParserSiegefortress (const std::vector<std::string> &entries,
                          boost::function<void (const FortressProperty&)> func);

#endif // __ZELOS_LIBKHAMAILEON_KHAMAILEON_PARSER_SIEGEFORTRESS_H__
