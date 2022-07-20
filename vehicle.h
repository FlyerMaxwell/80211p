//
// Created by cyx02 on 2022/6/27.
//

#ifndef BUBBLEMAC_INFOCOM_VEHICLE_H
#define BUBBLEMAC_INFOCOM_VEHICLE_H

#include "common.h"
#include <vector>
#include <unordered_map>
#include "parameters.h"
#include <fstream>



using namespace std;

typedef struct vehicle_info{
    struct vehicle* address;
    int timestamp;
}Vehicle_info;



typedef struct vehicle
{
    int handled;                  //  to indicate whether the car has been updated during this time
    int slot_appeared;
    //basic info
    char id[20];// id of the car
    char type[10]; //model of the car

    //dynamic info
    double x;
    double y;
    double angle;
    double speed;
    double pos;
    char lane[10];
    char prev_lane[10];//记录上一个车道
    double slope;
    double flow;
    double speed2;


    double acc = 4.5;  // for this version. this is determined by type.


    double commRadius;

    //Packets Received
    //struct Duallist packets;
    //Neighbors to acc
    struct Duallist neighbours;



    //802.11p related parameters
    int condition_80211; //TX  0; SENS  1; PREP
    int counter_to_TX;    //准备要发的包的个数,初始化为0，后面随时间累积
    int sense_timestamp; //开始sense的时间

    int prep_timestamp;
    int counter_sense;   //记录sense到的TS的个数
    int k;
    bool prep_frozen;  //false表示没有
    int counter_froze;

    int tx_timestamp;

    vector<int> * packets;
    int transmitted_packets;

}Vehicle;

double distance_between_vehicle(const struct vehicle* aCar, const struct vehicle* bCar);
void printVehilces(struct Duallist *ALL_Vehicles);
void logVehilcesInfo(struct Duallist *ALL_Vehicles, ofstream & logfile);
void logACar(struct vehicle* aCar, ofstream & logfile);

#endif //BUBBLEMAC_INFOCOM_VEHICLE_H
