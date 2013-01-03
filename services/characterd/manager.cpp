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

#include "manager.h"

#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>

#include <db/pool.h>
#include <db/scoped_connection.h>
#include <khamaileon/parser_manager.h>
#include <khamaileon/parser/itemdata.h>
#include <net/connection.h>
#include <packet/packet.h>
#include <sso/basic_items.h>
#include <sso/location.h>

#include "query_block.h"
#include "query_buff.h"
#include "query_cos.h"
#include "query_hotkey.h"
#include "query_item.h"
#include "query_item_basic.h"
#include "query_job.h"
#include "query_mastery.h"
#include "query_player.h"
#include "query_skill.h"
#include "query_storage.h"
#include "server/packet_character.h"
#include "server/packet_dc.h"
#include "server/packet_lobby.h"

Manager::Manager (boost::asio::io_service &io_service)
    : m_io_service(io_service),
    m_chinese(false),
    m_european(false)
{
}

bool Manager::initialize ()
{
    try
    {
        m_dbPool.reset(new db::Pool(m_db.c_str(),m_host.c_str(),m_user.c_str(),
                                    m_password.c_str()));

        if (!m_dbPool)
        {
            m_error = "Failed to initialize database pool";
            return false;
        }
    }
    catch (const mysqlpp::ConnectionFailed &error)
    {
        m_error = "Failed to establish a connection to the database.";
        return false;
    }
    catch (const mysqlpp::BadQuery &error)
    {
        m_error = error.what();
        return false;
    }

    if (m_path.empty())
        return false;

    ParserManager manager(m_path);

    boost::function<void (const ItemProperty&)> itemfunc =
            boost::bind(&Manager::OnItemdata,this,_1);

    manager.attachParser("itemdata.txt",boost::bind(&ParserItemdata,_1,itemfunc));

    if (!manager.loadMedia())
    {
        m_error = "Failed to load media data. Check your MediaPath in the configuration file.";
        return false;
    }

    return true;
}

void Manager::release ()
{
    m_dbPool.reset();
}

const std::string& Manager::getError () const
{
    return m_error;
}

void Manager::setHost (const std::string &host)
{
    m_host = host;
}

void Manager::setDatabase (const std::string &db)
{
    m_db = db;
}

void Manager::setUser (const std::string &usr, const std::string &pw)
{
    m_user = usr;
    m_password = pw;
}

void Manager::setCharRules (bool createCH, bool createEU)
{
    m_chinese = createCH;
    m_european = createEU;
}

void Manager::setMediaPath (const std::string &path)
{
    m_path = path;
}

void Manager::Create (const std::string &name,
                                        const uint32_t refID,
                                        const uint8_t scale,
                                        const uint32_t *items,
                                        const uint32_t accountID,
                                        const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnCreate,this,name,refID,scale,
                                  items,accountID,conn));
}

void Manager::List (const uint32_t accountID,
                                      const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnList,this,accountID,conn));
}

void Manager::Erase (const std::string &name, const uint32_t accountID,
                                       const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnErase,this,name,accountID,conn));
}

void Manager::Name (const std::string &name, const uint32_t accountID,
                                      const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnName,this,name,accountID,conn));
}

void Manager::Restore (const std::string &name, const uint32_t accountID,
                                         const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnRestore,this,name,accountID,conn));
}

void Manager::Select (const std::string &name, const uint32_t accountID,
                                        const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnSelect,this,name,accountID,conn));
}

void Manager::Fetch (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{
    m_io_service.post(boost::bind(&Manager::OnFetch,this,accountID,conn));
}

void Manager::OnCreate (const std::string &name, const uint32_t refID,
                                 const uint8_t scale, const uint32_t items[4],
                                 const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{

    enum
    {
        POS_CHEST,
        POS_LEG,
        POS_FOOT,
        POS_WEAPON,
        MAX_BASE_ITEMS
    };

    enum { MAX_PLAYER_COUNT = 4 };

    enum { LOCATION_INVENTORY = 0 };

    OPacket pkt;

    db::ScopedConnection dbConn(m_dbPool);

    if (db::player::Count(dbConn,accountID) >= MAX_PLAYER_COUNT)
    {
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_CREATE,pkt::LOBBY_ERROR_MAX_CHARACTERS);
        conn->send(&pkt);
        return;
    }

    /// Validate data sent
    if (!IsValid(name,accountID,conn))
        return;

    /// CHECK IF THE ARMOR IS OF THE SAME TYPE
    if (items[0]+1 != items[1] || items[1] +1 != items[2])
    {
        srv_pkt::DC(&pkt,accountID);
        conn->send(&pkt);
        return;
    }

    uint16_t returnID = 0;

    uint16_t zoneID;
    bool isChinese = true;
    float x = 0, y = 0, z = 0;

    /// CHECK THAT THE ITEMS CORRESPOND TO THE RACE,GENDER AND WEAPON
    if ( refID >= 1907 && refID <= 1932 )
    {
        /// CHINESE MODEL
        if (!m_chinese)
        {
            srv_pkt::DC(&pkt,accountID);
            conn->send(&pkt);
            return;;
        }

        /// CHECK FOR VALID CHINESE WEAPONS
        switch(items[POS_WEAPON])
        {
        case ITEM_CH_SWORD_01_A_DEF:
        case ITEM_CH_BLADE_01_A_DEF:
        case ITEM_CH_TBLADE_01_A_DEF:
            zoneID = 25000;
            x = 953.57257;
            y = 165.26994;
            z = -0.263591;
            break;
        case ITEM_CH_SPEAR_01_A_DEF:
        case ITEM_CH_BOW_01_A_DEF:
            zoneID = 25256;
            x = 956.6355;
            y = 1077.78;
            z = 40;
            break;
        default:
            srv_pkt::DC(&pkt,accountID);
            conn->send(&pkt);
            return;
        }

        if (refID >= 1907 && refID <= 1919)
        {
            /// CHECK FOR MEN MODEL AND ITEMS
            if (items[0] > ITEM_CH_M_CLOTHES_01_FA_A_DEF)
            {
                srv_pkt::DC(&pkt,accountID);
                conn->send(&pkt);
                return;
            }
        }
        else
        {
            /// CHECK FOR WOMEN MODEL AND ITEMS
            if (items[0] < ITEM_CH_W_HEAVY_01_BA_A_DEF)
            {
                srv_pkt::DC(&pkt,accountID);
                conn->send(&pkt);
                return;
            }
        }

        returnID = 1;
    }
    else if ( refID >= 14477 && refID <= 14502 )
    {
        if (!m_european)
        {
            srv_pkt::DC(&pkt,accountID);
            conn->send(&pkt);
            return;
        }

        isChinese = false;

         /// CHECK FOR VALID CHINESE WEAPONS
        switch(items[POS_WEAPON])
        {
        case ITEM_EU_SWORD_01_A_DEF:
        case ITEM_EU_TSWORD_01_A_DEF:
        case ITEM_EU_AXE_01_A_DEF:
        case ITEM_EU_CROSSBOW_01_A_DEF:
        case ITEM_EU_DAGGER_01_A_DEF:
            zoneID = 27471;
            x = 1180.5;
            y = 383.594;
            z = 80.146;
            break;
        case ITEM_EU_DARKSTAFF_01_A_DEF:
        case ITEM_EU_TSTAFF_01_A_DEF:
        case ITEM_EU_HARP_01_A_DEF:
        case ITEM_EU_STAFF_01_A_DEF:
            zoneID = 26957;
            x = 1459.8226;
            y = 1468.6079;
            z = 79.168945;
            break;
        default:
            srv_pkt::DC(&pkt,accountID);
            conn->send(&pkt);
            return;
        }

        if (refID >= 14477 && refID <= 14489)
        {
            /// CHECK FOR MEN MODEL AND ITEMS
            if (items[0] > ITEM_EU_M_CLOTHES_01_FA_A_DEF)
            {
                srv_pkt::DC(&pkt,accountID);
                conn->send(&pkt);
                return;
            }
        }
        else
        {
            /// CHECK FOR WOMEN MODEL AND ITEMS
            if (items[0] < ITEM_EU_W_HEAVY_01_BA_A_DEF)
            {
                srv_pkt::DC(&pkt,accountID);
                conn->send(&pkt);
                return;
            }
        }

        returnID = 20;
    }
    else
    {
        srv_pkt::DC(&pkt,accountID);
        conn->send(&pkt);
        return;
    }

    item_const_iterator iiter;
    uint8_t epos = INVALID_SLOT;
    std::vector<std::pair<uint32_t,uint8_t> > iitems;

    for (int i = 0; i < MAX_BASE_ITEMS; ++i)
    {
        iiter = m_items.find(items[i]);

        if (iiter != m_items.end())
        {
            epos = iiter->second.getDefaultPosition();

            if (epos == INVALID_SLOT)
            {
                srv_pkt::DC(&pkt,accountID);
                conn->send(&pkt);
                return;
            }

            iitems.push_back(std::make_pair(items[i],epos));
        }
    }

    uint32_t charID = db::player::Create(dbConn,accountID,refID,name,scale,zoneID,x,y,z,returnID);

    for (int i = 0; i < MAX_BASE_ITEMS; ++i)
        db::item::Create(dbConn,charID,iitems[i].first,iitems[i].second,LOCATION_INVENTORY);

    /// Look if the initial weapon requires a shield or bolt or arrows.
    uint32_t extraID = 0;

    switch(items[POS_WEAPON])
    {
    case ITEM_CH_SWORD_01_A_DEF:
    case ITEM_CH_BLADE_01_A_DEF:
        extraID = ITEM_CH_SHIELD_01_A_DEF;
        break;
    case ITEM_EU_SWORD_01_A_DEF:
    case ITEM_EU_DARKSTAFF_01_A_DEF:
    case ITEM_EU_STAFF_01_A_DEF:
        extraID = ITEM_EU_SHIELD_01_A_DEF;
        break;
    case ITEM_EU_CROSSBOW_01_A_DEF:
        extraID = ITEM_ETC_AMMO_BOLT_01_DEF;
        break;
    case ITEM_CH_BOW_01_A_DEF:
        extraID = ITEM_ETC_AMMO_ARROW_01_DEF;
        break;
    default:
        break;
    }

    if (extraID)
    {
        iiter = m_items.find(extraID);

        if (iiter != m_items.end())
        {
            epos = iiter->second.getDefaultPosition();

            if (epos != INVALID_SLOT)
                db::item::Create(dbConn,charID,extraID,epos,LOCATION_INVENTORY,
                                 iiter->second.durability.first,iiter->second.MaxStack);
        }
        else
            syslog(LOG_INFO,"Character::Create - Item %i dont exists.",extraID);
    }

    std::vector<std::pair<uint32_t,uint16_t> > basicItems = db::item_basic::Select(dbConn);

    {
        int pos = 13;
        std::vector<std::pair<uint32_t,uint16_t> >::const_iterator it;

        for (it = basicItems.begin(); it != basicItems.end(); ++it, ++pos)
        {
            iiter = m_items.find(extraID);

            if (iiter != m_items.end())
            {
                epos = iiter->second.getDefaultPosition();

                if (epos != INVALID_SLOT)
                {
                    db::item::Create(dbConn,charID,it->first,pos,LOCATION_INVENTORY,
                                     iiter->second.durability.first,it->second);
                }
            }
            else
                syslog(LOG_INFO,"Character::Create - Starting item %i dont exists.",it->first);
        }
    }

    if (isChinese)
    {
        for (int i = 0; i < 3; ++i)
            db::mastery::Create(dbConn,charID,257+i,1);

        for (int i = 0; i < 4; ++i)
            db::mastery::Create(dbConn,charID,273+i,1);
    }
    else
    {
        for (int i = 0; i < 6; ++i)
            db::mastery::Create(dbConn,charID,513+i,1);
    }


    srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_CREATE);
    conn->send(&pkt);
}

void Manager::OnList (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{
    db::ScopedConnection dbConn(m_dbPool);

    std::vector<sso::CharacterView> chars = db::player::Select(dbConn,accountID);

    OPacket pkt;
    srv_pkt::Lobby(&pkt,accountID,chars);
    conn->send(&pkt);
}

void Manager::OnErase (const std::string &name, const uint32_t accountID,
                                const boost::shared_ptr<IConnection> &conn)
{
    db::ScopedConnection dbConn(m_dbPool);

    db::player::Erase(dbConn,accountID,name);

    OPacket pkt;
    srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_ERASE);
    conn->send(&pkt);
}

void Manager::OnName (const std::string &name, const uint32_t accountID,
                               const boost::shared_ptr<IConnection> &conn)
{
    if (IsValid(name,accountID,conn))
    {
        OPacket pkt;
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME);
        conn->send(&pkt);
    }
}

void Manager::OnRestore (const std::string &name, const uint32_t accountID,
                                  const boost::shared_ptr<IConnection> &conn)
{
    db::ScopedConnection dbConn(m_dbPool);

    db::player::Restore(dbConn,accountID,name);

    OPacket pkt;
    srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_RESTORE);
    conn->send(&pkt);
}

void Manager::OnSelect (const std::string &name, const uint32_t accountID,
                                 const boost::shared_ptr<IConnection> &conn)
{
    db::ScopedConnection dbConn(m_dbPool);

    sso::CharacterInfo character = db::player::Select(dbConn,accountID,name);

    if (character.ID)
    {
        character.inventory = db::item::Select(dbConn,character.ID,sso::LOCATION_INVENTORY);

        {
            std::vector<sso::property::Item>::iterator it;
            boost::unordered_map<uint32_t,ItemProperty>::const_iterator iit;
            for (it = character.inventory.begin(); it != character.inventory.end(); ++it)
            {
                iit = m_items.find(it->refID);

                if (iit != m_items.end() &&
                    (iit->second.GetType() == ITEM_COS_PET || iit->second.GetType() == ITEM_COS_GRAB_PET))
                {
                    it->cos = db::cos::Select(dbConn,it->arg);
                }
            }
        }

        character.avatar = db::item::Select(dbConn,character.ID,sso::LOCATION_AVATAR);

        character.storage = db::item::Select(dbConn,accountID,sso::LOCATION_STORAGE);

        character.masteries = db::mastery::Select(dbConn,character.ID);

        character.skills = db::skill::Select(dbConn,character.ID);

        character.blocks = db::block::Select(dbConn,character.ID);

        character.hotkeys = db::hotkey::Select(dbConn,character.ID);

        character.job = db::job::Select(dbConn,character.ID);

        character.buffs = db::buff::Select(dbConn,character.ID);

        boost::lock_guard<boost::mutex> lock(m_char_mutex);
        m_chars.insert(std::make_pair(accountID,character));

        OPacket pkt;
        srv_pkt::Select(&pkt,accountID,character.Zone);
        conn->send(&pkt);
    }
    else
    {
        OPacket pkt;
        srv_pkt::DC(&pkt,accountID);
        conn->send(&pkt);
    }
}

void Manager::OnFetch (const uint32_t accountID, const boost::shared_ptr<IConnection> &conn)
{
    sso::CharacterInfo character;

    boost::unique_lock<boost::mutex> lock(m_char_mutex);

    boost::unordered_map<uint32_t,sso::CharacterInfo>::iterator it = m_chars.find(accountID);

    if (it != m_chars.end())
    {
        character = it->second;
        m_chars.erase(it);
    }

    lock.unlock();

    OPacket pkt;
    srv_pkt::CharacterInfo(&pkt,accountID,&character);
    conn->send(&pkt);
}

bool Manager::IsValid (const std::string &name, const uint32_t accountID,
                                const boost::shared_ptr<IConnection> &conn)
{
    OPacket pkt;

    /// CHECK THE NAME LENGTH DONT EXCEEDS THE MAX
    if (name.length() > MAX_PLAYER_NAME_LENTH)
    {
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME,pkt::LOBBY_ERROR_MAX_CHARACTERS);
        conn->send(&pkt);
        return false;
    }

    if (!db::IsInputValid(name))
    {
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME,pkt::LOBBY_ERROR_NAME_EXIST);
        conn->send(&pkt);
        return false;
    }

    /// CHECK FOR INVALID CHARACTER
    if (name.find_first_of(m_ch_filter) != std::string::npos)
    {
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME,pkt::LOBBY_ERROR_NAME_EXIST);
        conn->send(&pkt);
        return false;
    }

    std::vector<std::string>::const_iterator i;
    for ( i = m_substr_filter_list.begin(); i != m_substr_filter_list.end(); ++i)
    {
        if (name.find(*i) != std::string::npos)
        {
            srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME,pkt::LOBBY_ERROR_NAME_EXIST);
            conn->send(&pkt);
            return false;
        }
    }

    /// CHECK THAT THE NAME ISNT ALREDY IN USE

    db::ScopedConnection dbConn(m_dbPool);

    if (!db::player::Check(dbConn,name))
    {
        srv_pkt::Lobby(&pkt,accountID,pkt::LOBBY_ACTION_VALIDATE_NAME,pkt::LOBBY_ERROR_NAME_EXIST);
        conn->send(&pkt);
        return false;
    }

    return true;
}

void Manager::OnItemdata (const ItemProperty &property)
{
    m_items.insert(std::make_pair(property.refID,property));
}
