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

#include "query_player.h"

#include <syslog.h>

#include <mysql++/mysql++.h>

namespace db {
namespace player {

uint32_t AccountID (ScopedConnection &conn, const uint64_t charID)
{
    uint32_t accID = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select AccountID from characters where ID = " << charID;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
            accID = res[0]["AccountID"];
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::AccountID - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::AccountID - %s",error.what());
    }

    return accID;
}

bool Check (ScopedConnection &conn, const std::string& name)
{
    bool ret = true;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        escName.reserve(2*name.length()+1);
        query.escape_string(&escName,name.c_str(),name.length());

        query << "select Name from characters where Name = " << mysqlpp::quote << escName;

        mysqlpp::StoreQueryResult res = query.store();

        ret = !res.num_rows();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Check - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Check - %s",error.what());
    }

    return ret;
}

int Count (ScopedConnection &conn, const uint32_t accountID)
{
    int ret = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "select count(*) from characters where AccountID = " << accountID;

        mysqlpp::StoreQueryResult res = query.store();

        if (res.num_rows())
            ret = res[0]["count(*)"];
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Count - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Count - %s",error.what());
    }

    return ret;
}

std::vector<sso::CharacterView> Select (ScopedConnection &conn, const uint32_t accountID)
{
    std::vector<sso::CharacterView> v;;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from characters where AccountID = " << accountID << " and ExpirationDate <= now() "
                << "and ExpirationDate > CreationDate";

        query.exec();

        query.reset();

        query << "select refCharID,Name,Volume,Level,Strength,Intelligence,HP,MP,Stats,Exp,ExpirationDate "
                << "from characters where AccountID = " << accountID;

        mysqlpp::StoreQueryResult res = query.store();

        sso::CharacterView character;

        for (size_t i = 0; i < res.num_rows(); ++i)
        {
            character.refID = res[i]["refCharID"];
            character.name = std::string(res[i]["Name"]);
            character.scale = res[i]["Volume"];
            character.level = res[i]["Level"];
            character.statStr = res[i]["Strength"];
            character.statInt = res[i]["Intelligence"];
            character.HP = res[i]["HP"];
            character.MP = res[i]["MP"];
            character.stats = res[i]["Stats"];
            character.exp = res[i]["Exp"];

            time_t actual_time;
            time(&actual_time);
            mysqlpp::DateTime dt = res[i]["ExpirationDate"];

            if (actual_time > dt)
            {
                character.timeMin = static_cast<uint32_t>(difftime(actual_time,dt)/60);

                v.push_back(character);
            }
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Select - %s",error.what());
    }

    return v;
}

sso::CharacterInfo Select (ScopedConnection &conn, const uint32_t accountID, const std::string &name)
{
    sso::CharacterInfo character;

    try
    {
        mysqlpp::Query query = conn->query();

        query << "delete from characters where AccountID = " << accountID << " and ExpirationDate <= now() "
                << "and ExpirationDate > CreationDate";

        query.exec();

        query.reset();

        query << "select * from characters where AccountID = " << accountID
                << " and Name = " << mysqlpp::quote << name;

        mysqlpp::StoreQueryResult res = query.store();

        for (size_t i = 0; i < res.num_rows(); ++i )
        {
            character.ID = res[i]["ID"];
            character.RefCharID = res[i]["refCharID"];
            character.Name = std::string(res[i]["Name"]);
            character.Volume = res[i]["Volume"];
            character.Level = res[i]["Level"];
            character.MaxLevel = res[i]["MaxLevel"];
            character.Str = res[i]["Strength"];
            character.Int = res[i]["Intelligence"];
            character.HP = res[i]["HP"];
            character.MP = res[i]["MP"];
            character.Stats = res[i]["Stats"];
            character.SP = res[i]["SP"];
            character.Exp = res[i]["Exp"];
            character.SPExp = res[i]["SPExp"];
            character.Gold = res[i]["Gold"];
            character.BskCount = res[i]["BerserkCount"];
            character.PKLevel = res[i]["PK"];
            character.PKPoints = res[i]["PKLvl"];
            character.Zone = res[i]["Zone"];
            character.X = res[i]["X"];
            character.Y = res[i]["Y"];
            character.Z = res[i]["Z"];
            character.Angle = res[i]["Angle"];
            character.Rank = res[i]["Rank"];
            character.Tutorials = res[i]["Tutorials"];
            character.pvpCape = res[i]["PVPCape"];
            character.InvSlots = res[i]["InventorySlots"];
            character.InvSlotsIncreased = res[i]["InventorySlotsInc"];
            character.StoreSlots = res[i]["StorageSlots"];
            character.StorageSlotsIncreased = res[i]["StorageSlotsInc"];
            character.GuildID = res[i]["GuildID"];
            character.AcademyID = res[i]["AcademyID"];
            character.ReturnID = res[i]["ReturnID"];
            character.StallAvatarID = res[i]["StallAvatarID"];
            character.AbilityPetID = res[i]["AbilityPetID"];
            character.AttackPetID = res[i]["AttackPetID"];
            character.TransportID = res[i]["TransportID"];
            character.autopot.hp_active = res[i]["AutopotHPActive"];
            character.autopot.hp_value = res[i]["AutopotHPValue"];
            character.autopot.hp_bar = res[i]["AutopotHPBar"];
            character.autopot.hp_slot = res[i]["AutopotHPSlot"];
            character.autopot.mp_active = res[i]["AutopotMPActive"];
            character.autopot.mp_value = res[i]["AutopotMPValue"];
            character.autopot.mp_bar = res[i]["AutopotMPBar"];
            character.autopot.mp_slot = res[i]["AutopotMPSlot"];
            character.autopot.pill_active = res[i]["AutopotPillActive"];
            character.autopot.pill_bar = res[i]["AutopotPillBar"];
            character.autopot.pill_slot = res[i]["AutopotPillSlot"];
            character.autopot.delay_active = res[i]["AutopotDelay"];
        }
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Select - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Select - %s",error.what());
    }

    return character;
}

uint32_t Create (ScopedConnection &conn, const uint32_t accountID, const uint32_t refID,
             const std::string &name, const uint8_t scale, const uint16_t zoneID,
             const float x, const float y, const float z, const uint32_t returnID)
{
    using mysqlpp::quote;

    uint32_t ret = 0;

    try
    {
        mysqlpp::Query query = conn->query();

        std::string escName;
        escName.reserve(2*name.length()+1);
        query.escape_string(&escName,name.c_str(),name.length());

        query << "insert into characters (AccountID,refCharID,Name,Volume,Zone,X,Y,Z,ReturnID,CreationDate)"
            << " values (" << accountID << "," << refID << "," << quote << escName << ","
            << quote << scale << "," << zoneID << "," << x << "," << y << "," << z << ","
            << returnID << ", now())";

        query.exec();

        ret = query.insert_id();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Create - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Create - %s",error.what());
    }

    return ret;
}

void Erase (ScopedConnection &conn, const uint32_t accountID, const std::string &name)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set ExpirationDate = now() + interval 5 minute "
                << "where AccountID = " << accountID
                << " and Name = " << mysqlpp::quote  << name;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Erase - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Erase - %s",error.what());
    }
}

void Restore (ScopedConnection &conn, const uint32_t accountID, const std::string &name)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set ExpirationDate = '0000-00-00 00:00:00' "
                << "where AccountID = " << accountID
                << " and Name = " << mysqlpp::quote  << name;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Restore - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Restore - %s",error.what());
    }
}

void Position (ScopedConnection &conn, const uint64_t charID, const int16_t zoneID,
               const float x, const float y, const float z)
{
    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "update characters set "
                << "Zone=" << quote << zoneID << ","
                << "X=" << quote << x << ","
                << "Y=" << quote << y << ","
                << "Z=" << quote << z << " "
                << "where ID =" << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Position - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Position - %s",error.what());
    }
}

void Angle (ScopedConnection &conn, const uint64_t CharID, const uint16_t angle)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set Angle = " << angle << " where ID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Angle - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Angle - %s",error.what());
    }
}

void SP (ScopedConnection &conn, const uint64_t CharID, const uint32_t points)
{
    try
    {
        using mysqlpp::quote;
        using mysqlpp::sql_int;

        mysqlpp::Query query = conn->query();

        query << "update characters set SP =" << (sql_int)points << " where ID =" << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::SP - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::SP - %s",error.what());
    }
}

void Exp (ScopedConnection &conn, const uint64_t charID, const uint64_t exp)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set Exp = " << exp << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Exp - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Exp - %s",error.what());
    }
}

void SPExp (ScopedConnection &conn, const uint64_t charID, const uint16_t exp)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set SPExp = " << exp << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::SPExp - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::SPExp - %s",error.what());
    }
}

void Stats (ScopedConnection &conn, const uint64_t charID, const uint16_t stats)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set "
            << "Stats=" << mysqlpp::quote << stats << " "
            << "where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s",error.what());
    }
}

void StatsStr (ScopedConnection &conn, const uint64_t charID, const uint16_t stats,
               const uint16_t sStr)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set "
            << "Strength=" << mysqlpp::quote << sStr << " , "
            << "Stats=" << mysqlpp::quote << stats << " "
            << "where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s",error.what());
    }
}

void StatsInt (ScopedConnection &conn, const uint64_t charID, const uint16_t stats,
               const uint16_t sInt)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set "
            << "Intelligence=" << mysqlpp::quote << sInt << " , "
            << "Stats=" << mysqlpp::quote << stats << " "
            << "where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Stats - %s",error.what());
    }
}

void Lvl (ScopedConnection &conn, const uint64_t charID, const uint8_t curLv, const uint8_t maxLv)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set "
            << "Level = " << mysqlpp::quote << curLv << " , "
            << "MaxLevel = " << mysqlpp::quote << maxLv << " "
            << "where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Lvl - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Lvl - %s",error.what());
    }
}

void Gold (ScopedConnection &conn, const uint64_t charID, const uint64_t gold)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set Gold = " << gold << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Gold - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Gold - %s",error.what());
    }
}

void ReturnPoint (ScopedConnection &conn, const uint64_t charID, const uint32_t teleport_id)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set ReturnID = " << teleport_id << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::ReturnPoint - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::ReturnPoint - %s",error.what());
    }
}

void Health (ScopedConnection &conn, const uint64_t charID, const uint32_t HP, const uint32_t MP)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set HP = " << HP << ", MP = " << MP
                << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Health - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Health - %s",error.what());
    }
}

void Berserk (ScopedConnection &conn, const uint64_t charID, const uint8_t count)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set BerserkCount = " << mysqlpp::quote << count
            << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Berserk - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Berserk - %s",error.what());
    }
}

void State (ScopedConnection &conn, const uint64_t charID, bool state)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set State = " << state << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::State - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::State - %s",error.what());
    }
}

void GuildID (ScopedConnection &conn, const uint64_t CharID, const uint32_t guildID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set GuildID = " << guildID << " where ID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::GuildID - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::GuildID - %s",error.what());
    }
}

void StallAvatar (ScopedConnection &conn, const uint64_t charID, const uint32_t avatarID)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set StallAvatarID = " << avatarID << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::StallAvatar - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::StallAvatar - %s",error.what());
    }
}

void Autopot (ScopedConnection &conn, const uint64_t CharID, const sso::Autopot &settings)
{
    try
    {
        using mysqlpp::quote;

        mysqlpp::Query query = conn->query();

        query << "update characters set AutopotHPActive = " << quote << settings.hp_active << ","
            << "AutopotHPValue = " << quote << settings.hp_value << ","
            << "AutopotHPBar = " << quote << settings.hp_bar << ","
            << "AutopotHPSlot = " << quote << settings.hp_slot << ","
            << "AutopotMPActive = " << quote << settings.mp_active << ","
            << "AutopotMPValue = " << quote << settings.mp_value << ","
            << "AutopotMPBar = " << quote << settings.mp_bar << ","
            << "AutopotMPSlot = " << quote << settings.mp_slot << ","
            << "AutopotPillActive = " << quote << settings.pill_active << ","
            << "AutopotPillBar = " << quote << settings.pill_bar << ","
            << "AutopotPillSlot = " << quote << settings.pill_slot << ","
            << "AutopotDelayActive = " << quote << settings.delay_active << ","
            << "AutopotDelay = " << quote << settings.delay_value
            << " where ID = " << CharID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Autopot - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Autopot -  %s",error.what());
    }
}

void InventoryCapacity (ScopedConnection &conn, const uint64_t charID, const uint8_t size)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set InventorySlotsInc = TRUE, InventorySlots = " << mysqlpp::quote << size
            << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::InventoryCapacity - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::InventoryCapacity - %s",error.what());
    }
}

void StorageCapacity (ScopedConnection &conn, const uint64_t charID, const uint8_t size)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set StorageSlotsInc = TRUE, StorageSlots = " << mysqlpp::quote << size
            << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::StorageCapacity - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::StorageCapacity - %s",error.what());
    }
}

void Skin (ScopedConnection &conn, const uint64_t charID, const uint32_t refCharID,
    const uint8_t vol)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set refCharID = " << refCharID << ","
            << "Volume = " << mysqlpp::quote << vol << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Skin - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Skin - %s",error.what());
    }
}

void Tutorial (ScopedConnection &conn, const uint64_t charID, const uint64_t tutorials)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set Tutorials = " << tutorials << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::Tutorial - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::Tutorial - %s",error.what());
    }
}

void Cape (ScopedConnection &conn, const uint64_t charID, const uint8_t cape)
{
    try
    {
        mysqlpp::Query query = conn->query();

        query << "update characters set PVPCape = " << mysqlpp::quote << cape << " where ID = " << charID;

        query.exec();
    }
    catch (const mysqlpp::BadQuery &error)
    {
        syslog(LOG_INFO,"db::player::WriteCape - %s, Errornum = %i",error.what(),error.errnum());
    }
    catch (const mysqlpp::Exception &error)
    {
        syslog(LOG_INFO,"db::player::WriteCape - %s",error.what());
    }
}

}	}
