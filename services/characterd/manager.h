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

#ifndef __ZELOS_SERVICES_CHARACTERD_MANAGER_H__
#define __ZELOS_SERVICES_CHARACTERD_MANAGER_H__

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

#include <khamaileon/property/item.h>
#include <sso/property/character.h>

enum { MAX_BASE_ITEMS = 4 };

namespace db
{
    class Pool;
}

class IConnection;

class Manager
{
public:

    typedef boost::unordered_map<uint32_t,ItemProperty>::iterator item_iterator;
    typedef boost::unordered_map<uint32_t,ItemProperty>::const_iterator item_const_iterator;

public:

    Manager (boost::asio::io_service &io_service);

    bool initialize ();

    void release ();

    const std::string& getError () const;

    void setHost (const std::string &host);

    void setDatabase (const std::string &db);

    void setUser (const std::string &usr, const std::string &pw);

    void setCharRules (bool createCH, bool createEU);

    void setMediaPath (const std::string &path);

    void Create (const std::string &name, const uint32_t refID, const uint8_t scale,
                 const uint32_t *items, const uint32_t accountID,
                 const boost::shared_ptr<IConnection> &conn);

    void List (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

    void Erase (const std::string &name, const uint32_t accountID,
                const boost::shared_ptr<IConnection> &conn);

    void Name (const std::string &name, const uint32_t accountID,
               const boost::shared_ptr<IConnection> &conn);

    void Restore (const std::string &name, const uint32_t accountID,
                  const boost::shared_ptr<IConnection> &conn);

    void Select (const std::string &name, const uint32_t accountID,
                 const boost::shared_ptr<IConnection> &conn);

    void Fetch (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

private:

    void OnCreate (const std::string &name, const uint32_t refID, const uint8_t scale,
                   const uint32_t items[4], const uint32_t accountID,
                   const boost::shared_ptr<IConnection> &conn);

    void OnList (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

    void OnErase (const std::string &name, const uint32_t accountID,
                  const boost::shared_ptr<IConnection> &conn);

    void OnName (const std::string &name, const uint32_t accountID,
                 const boost::shared_ptr<IConnection> &conn);

    void OnRestore (const std::string &name, const uint32_t accountID,
                    const boost::shared_ptr<IConnection> &conn);

    void OnSelect (const std::string &name, const uint32_t accountID,
                   const boost::shared_ptr<IConnection> &conn);

    void OnFetch (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn);

private:

    bool IsValid (const std::string &name, const uint32_t accountID,
                  const boost::shared_ptr<IConnection> &conn);

    void OnItemdata (const ItemProperty &property);

private:

    boost::asio::io_service &m_io_service;

    std::string m_host;
    std::string m_db;
    std::string m_user;
    std::string m_password;
    std::string m_path;
    std::string m_error;

    bool m_chinese;
    bool m_european;
    std::string m_ch_filter;
    std::vector<std::string> m_substr_filter_list;
    boost::shared_ptr<db::Pool> m_dbPool;

    boost::unordered_map<uint32_t,ItemProperty> m_items;

    boost::mutex m_char_mutex;
    boost::unordered_map<uint32_t,sso::CharacterInfo> m_chars;
};

#endif // __ZELOS_SERVICES_CHARACTERD_MANAGER_H__
