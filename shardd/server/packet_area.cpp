#include "server/packet_area.h"

#include <net/opcode/shard_area_srv.h>
#include <packet/packet.h>

#include "area.h"

namespace srv_pkt
{
    void AreaConfig (OPacket *pkt, Area *area)
    {
        pkt->WriteOpcode(SRV_AREA_CONFIG);
        pkt->Write<uint16_t>(area->ID);
        pkt->Write<uint8_t>(area->Instanced);
        pkt->Write<uint16_t>(area->Zones.size());

        for (size_t i = 0; i < area->Zones.size(); ++i)
            pkt->Write<uint16_t>(area->Zones[i]);
    }

    void AreaAction (OPacket *pkt, bool action)
    {
        pkt->WriteOpcode(SRV_AREA_ACTION);
        pkt->Write<uint8_t>(action);
    }
}
