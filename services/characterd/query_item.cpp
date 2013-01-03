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

#include "query_item.h"

#include <mysql++/mysql++.h>
#include <syslog.h>

namespace db {
namespace item {

uint64_t Create (ScopedConnection &conn, const uint64_t charID, const uint32_t refID,
                 const uint8_t pos, const uint8_t location, const uint32_t durability,
                 const uint16_t quantity)
{
    uint64_t itemID = 0;

    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "insert into items (charID,refItemID,Quantity,Location,Position,Durability) values ("
                << quote << charID << "," << quote << (mysqlpp::sql_int)refID << ","
                << quote << (mysqlpp::sql_int)quantity << "," << quote << location << ","
                << quote << pos << "," << quote << durability << ")";

        query.exec();

        itemID = query.insert_id();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Create - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Create -%s",error.what());
    }

    return itemID;
}

std::vector<sso::property::Item> Select (ScopedConnection &conn, const uint64_t charID,
                                         const uint8_t location)
{
    std::vector<sso::property::Item> v;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select * from items where CharID =" << charID << " and Location = "
                << mysqlpp::quote << location;

        mysqlpp::StoreQueryResult res = query.store();

        sso::property::Item item;

        for (size_t j=0; j < res.num_rows(); ++j)
        {
            item.ID = res[j]["ID"];
            item.refID = res[j]["refItemID"];
            item.position = res[j]["Position"];
            item.location = res[j]["Location"];
            item.enchantLv = res[j]["EnchantLevel"];
            item.quantity = res[j]["Quantity"];
            item.mods = res[j]["Modifiers"];
            item.arg = res[j]["Arg"];

            query.reset();

            query << "select * from items_magicoptions where itemID = " << item.ID;

            mysqlpp::StoreQueryResult ires = query.store();

            for (size_t i = 0; i < ires.num_rows(); ++i)
                item.magicoptions.push_back(std::make_pair(ires[i]["optionID"],ires[i]["Value"]));

            v.push_back(item);

            item.magicoptions.clear();
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Select - %s",error.what());
    }

    return v;
}

void Position (ScopedConnection &conn, const uint64_t itemID, const uint8_t pos)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update items set Position = " << mysqlpp::quote << pos << " where ID = " << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Position - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Position - %s",error.what());
    }
}

void EnchantLv (ScopedConnection &conn, const uint64_t itemID, const uint8_t level)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update items set EnchantLevel = " << mysqlpp::quote << level << " where ID = " << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::EnchantLv - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::EnchantLv - %s",error.what());
    }
}

void Quantity (ScopedConnection &conn, const uint64_t itemID, const uint16_t quantity)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update items set Quantity = " << quantity << " where ID =" << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Quantity - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Quantity - %s",error.what());
    }
}

void Location (ScopedConnection &conn, const uint64_t itemID, const uint32_t ownerID,
               const uint8_t location, const uint8_t pos)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update items set Location = " << mysqlpp::quote << location
                << ", Position = " << mysqlpp::quote << pos
                << ", charID = " << ownerID
                << " where ID = " << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Location - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Location - %s",error.what());
    }
}

void Durability (ScopedConnection &conn, const uint64_t itemID, const uint32_t durability)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update items set Durability ="  << durability << " where ID = " << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Durability - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Durability - %s",error.what());
    }
}

void Delete (ScopedConnection &conn, const uint64_t itemID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from items where ID =" << itemID;

        query.exec();

        query.reset();

        query << "delete from items_magicoptions where itemID = " << itemID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::Delete - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::Delete - %s",error.what());
    }
}

void MagicOption (ScopedConnection &conn, const uint64_t itemID, const uint32_t optionID,
        const uint32_t value)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "insert into items_magicoptions values (" << itemID << "," << optionID << "," << value << ")";

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::MagicOption - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::MagicOption - %s",error.what());
    }
}

uint32_t SkillID (ScopedConnection &conn, const std::string &refSkillName)
{
    uint32_t skillID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select refID from refskilldata where refName = " << mysqlpp::quote << refSkillName;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
            skillID = res[0]["refID"];
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::SkillID - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::SkillID - %s",error.what());
    }

    return skillID;
}

uint32_t CharID (ScopedConnection &conn, const std::string &refName)
{
    uint32_t refCharID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select refID from refcharacterdata where refName = " << mysqlpp::quote << refName;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
            refCharID = res[0]["refID"];
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::CharID - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::CharID - %s",error.what());
    }

    return refCharID;
}

uint32_t ItemID (ScopedConnection &conn, const std::string &refName)
{
    uint32_t refItemID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select refID from refitemdata where refName = " << mysqlpp::quote << refName;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
            refItemID = res[0]["refID"];
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::item::ItemID - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::item::ItemID - %s",error.what());
    }

    return refItemID;
}

}   }
