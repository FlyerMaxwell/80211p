//
// Created by cyx02 on 2022/7/20.
//

#ifndef INC_80211_INFOCOM_PACKETS_H
#define INC_80211_INFOCOM_PACKETS_H


#include "vehicle.h"
#include <unordered_map>
#include <vector>
#include <fstream>
#include "common.h"

typedef struct packet{
    struct vehicle* srcVehicle, *dstVehicle;//for debug
    //timestamp
    int timestamp;
    int condition;

    char src_lane[20];
    char dst_lane[20];
    int src_pos;
    int dst_pos;
}Packet;


struct packet * generate_packet(struct vehicle *aCar, struct vehicle *bCar ,int slot, int condition);


#endif //INC_80211_INFOCOM_PACKETS_H
