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

#ifndef __ZELOS_CERTD_SERVICE_H__
#define __ZELOS_CERTD_SERVICE_H__

#include <net/service.h>

class Server;

class Service : public IService
{
public:

    Service ();

    virtual ~Service ();

    const std::string& getLastError () const;

    void certify (const uint32_t connID);

private:

    bool OnInitialize ();

    void OnConfigure (const std::map<std::string, std::string> &entries);

    void OnClientState (const uint32_t connID, const int type, const bool state);

    void OnClientError (const uint32_t connID, const int type, const boost::system::error_code &error);

    void OnRelease ();

    void OnCertify (const uint32_t connID);

private:

    boost::shared_ptr<Server> m_server;
};

#endif // __ZELOS_CERTD_SERVICE_H__
