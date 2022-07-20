//
// Created by cyx02 on 2022/7/20.
//

#include "packets.h"
#include "string.h"


struct packet * generate_packet(struct vehicle *aCar, struct vehicle *bCar, int slot, int condition){
    struct packet* pkt;
    pkt = (struct packet*)malloc(sizeof(struct packet));
    pkt->timestamp = slot;
    // pkt->slot_condition = 0;//还没有发生碰撞
    pkt->srcVehicle = aCar;
    pkt->dstVehicle = bCar;
    pkt->condition = condition;

    strcpy(pkt->src_lane, aCar->lane);
    strcpy(pkt->dst_lane,bCar->lane );


    pkt->src_pos = aCar->pos;
    pkt->dst_pos = bCar->pos;

    return pkt;
}
