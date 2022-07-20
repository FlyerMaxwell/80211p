#include <iostream>

#include <iostream>
#include <fstream>
#include "common.h"
#include "UpLocation.h"
#include "parameters.h"
#include "Protocol_80211.h"
#include "vehicle.h"


using namespace std;

int main(int argc, char * argv[]) {

    cout<<"hello world"<<endl;


    // Set the trace path and traffic density
    string trace_path;
//    int protocol = argv[2][0] - '0'; //0: bubble 1:802.11 2:veMAC
//    trace_path = argv[1];       //读取文件路径
    trace_path = "C:\\Users\\cyx02\\Desktop\\transformed\\transformed\\carposition_";
    traffic_density = 240;



    cout<< "Current trace file is from:"<< trace_path <<"..."<<endl;
    cout<<"Current Protocol is Bubble MAC..."<<endl;


    // Set the dir of log
    string log_dir_process = ".\\log_process_";
    log_dir_process += to_string(traffic_density);
    log_dir_process += ".txt";

    string log_dir_statistic = ".\\log_statistic_";
    log_dir_statistic += to_string(traffic_density);
    log_dir_statistic += ".txt";

    log_process_file.open(log_dir_process);
    if(!log_process_file){cerr <<"create file error \n"; return 1;}

    log_statistic_file.open(log_dir_statistic);
    if(!log_statistic_file){cerr <<"create file error \n"; return 1;}


    // Reset the Statistical Para
    cnt_cars = 0;
    cnt_tx_collision = 0;       //两个发射碰撞
    cnt_rx_normal = 0;          //正常收包
    cnt_rx_colli = 0;           //产生碰撞的包
    cnt_pkt_tx = 0;             //发射的包个数
    cnt_pkt_tx_normal = 0;      //发射过程中没有TX碰撞的个数
    cnt_frontV_normal = 0;
    cnt_rearV_normal = 0;
    cnt_frontV_colli = 0;
    cnt_rearV_Colli = 0;

    srand(0);

    int slot_start = 0; //每个slot是0.5毫秒
    int slot_end =1000; //19200 挂掉了
    int slot_step = 1;

    struct Duallist ALL_Vehicles;
    duallist_init(&ALL_Vehicles);

    log_process_file << "The simulation is starting..."<<endl;
    log_process_file <<"slot start from "<< slot_start<<" to "<<slot_end<<", the slot step is "<<slot_step<<endl<<endl;

    Car_Number = 0;


    collision_vehicle = new struct vehicle;
    strcpy(collision_vehicle->id,"rx_collision");



    for(int slot = slot_start; slot < slot_end; slot += slot_step){
        //cout<<"slot = "<< slot<<endl;

//        logVehilcesInfo(&ALL_Vehicles, logfile);
//        logfile<<endl;

        if(slot % (UpLocSlot*1000) == 0){ //仍然是每5毫秒更新一下位置
            init_simulation(&ALL_Vehicles);
            updateLocation(&ALL_Vehicles, slot, trace_path);
            handle_neighbours(&ALL_Vehicles);
            //cout<<"The location of vehicles has been updated, Current slot = "<<slot<<endl;
            //printVehilces(&ALL_Vehicles);

//            logfile<<"Event: Update location! Current slot = "<<slot<<", Car_Number="<<Car_Number<<endl;
//            logVehilcesInfo(&ALL_Vehicles, logfile);
//            logfile<<endl;
        }

         mac_80211p(&ALL_Vehicles, slot);

    }

    struct Item *aItem;
    struct vehicle* aCar;
    aItem = ALL_Vehicles.head;

    cout<<counter_received<<endl;
    while(aItem != NULL) {
        aCar = (struct vehicle *) aItem->datap;
        count_collisions_received(aCar);
        aItem = aItem->next;
    }

    cout<<"Transmitted packets:"<<counter_tx<<endl;
    cout<<"Collisions:"<<counter_collision<<endl;
    cout<<"Received:"<<counter_received<<endl;
    cout<<"total Cars:"<<Car_Number<<endl;
    logfile.close();

    return 0;
}