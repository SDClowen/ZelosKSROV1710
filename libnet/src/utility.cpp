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

#include "net/utility.h"

#include <algorithm>
#include <fstream>
#include <iostream>

bool read_config_file (const std::string &filename, std::map<std::string,std::string> &entries)
{
    std::ifstream file(filename.c_str());

    if (!file)
    {
        std::cerr << "File " << filename << " dont exists." << std::endl;
        return false;
    }

    size_t i = 0;
    std::string line;

    while (!file.eof())
    {
        std::getline(file,line);

        if (!line.empty())
        {
            ///A LINE BEGGINING WITH A # IS A COMMENT

            if (*line.begin() != '#')
            {
                ///TRIM WHITESPACES
                line.erase(std::remove(line.begin(),line.end(),' '),line.end());

                size_t pos = line.find_first_of('=');

                if (pos == std::string::npos)
                {
                    std::cerr << "Invalid option in line " << i << ", " << line << std::endl;
                    return false;
                }

                entries.insert(std::make_pair(line.substr(0,pos),line.substr(pos+1)));
            }
        }

        ++i;
    }

    return true;
}
