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

#ifndef __ZELOS_LIBSSO_SSO_ENTITY_MOB_H__
#define __ZELOS_LIBSSO_SSO_ENTITY_MOB_H__

#include <sso/entity/character.h>

#define MOB_MAX_SPAWN_POINT_DISTANCE 1600

namespace sso
{
    enum MOB_TASK
    {
        MOB_TASK_IDLE,
        MOB_TASK_MOVE,
        MOB_TASK_MOVE_TO_TARGET,
        MOB_TASK_MOVE_TO_SPOINT,
        MOB_TASK_CAST_WAIT,
        MOB_TASK_CAST,
        MOB_TASK_STUNNED
    };

    enum MOB_QUALITY
    {
        CH_QUALITY_NORMAL,
        CH_QUALITY_CHAMPION,
        CH_QUALITY_UNIQUE = 3,
        CH_QUALITY_GIANT,
        CH_QUALITY_TITAN,
        CH_QUALITY_ELITE,
        CH_QUALITY_EVENT_ELITE,
        CH_QUALITY_LESSER_UNIQUE
    };

    class Mob : public ICharacter
    {
    public:

        typedef ICharacter::attack_type attack_type;

        Mob (const boost::shared_ptr<EntityProperty> &property, const uint8_t quality);

        virtual ~Mob ();

        uint8_t Quality () const;

        virtual uint8_t getLevel () const;

        virtual bool IsTaskInterruptable () const;

        uint32_t get_next_skill ();

        void SetSpawnPoint (const size_t spawnIndex);

        size_t GetSpawnPoint () const;

        bool IsRespawnable () const;

    protected:

        virtual void calculatePreStats (std::vector<std::pair<CHAR_STATS, int32_t> > &bonus);

    private:

        uint8_t m_quality;
        uint8_t m_skill_idx;

        size_t m_spawn_idx;
    };
}
#endif // __ZELOS_LIBSSO_SSO_ENTITY_MOB_H__
