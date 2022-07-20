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

    cout << "Hello, World!" << std::endl;

    // Set the trace path
    string trace_path;
    trace_path = argv[1];       //读取文件路径

    trace_path = "C:\\Users\\cyx02\\Desktop\\transformed\\transformed\\carposition_";
    traffic_density = 240;

    cout<< "Current trace file is from:"<< trace_path <<"..."<<endl;

    //仿真的详细log路径，记录所有发生的事件
    string log_dir = ".\\log.txt";
    ofstream logfile;

    logfile.open(log_dir);
    if(!logfile){cerr <<"create file error \n"; return 1;}

    srand(0);

    int slot_start = 0;         //这里一个slot变成1微秒， 1秒等于1000毫秒，等于100 000微秒
    int slot_end =20000000;   //注意单位是微秒
    int slot_step = 1;
    //int slot_len = 0.5;

    total_frame = slot_end/1000;   //0.1秒是一帧





    vector<float> rate;
    vector<int> arr = {1000000, 2000000, 5000000, 8000000, 10000000, 12000000, 15000000, 16000000, 17000000, 18000000, 19000000, slot_end - slot_step};
    int critic = 0;

    struct Duallist ALL_Vehicles;
    duallist_init(&ALL_Vehicles);

    for(int slot = slot_start; slot < slot_end; slot += slot_step){
        //cout<<"slot = "<< slot<<endl;

//        logVehilcesInfo(&ALL_Vehicles, logfile);
//        logfile<<endl;

        if(slot % (UpLocSlot*1000) == 0){ //仍然是每5毫秒更新一下位置
            if(slot % 100000 == 0) cout << "slot = " << slot << endl;
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
        if (critic != arr.size() && (int)slot == arr[critic]){
            rate.push_back((float) (counter_received) / (0.01 + counter_received + counter_collision));

            critic++;
        }
    }

    struct Item *aItem;
    struct vehicle* aCar;
    aItem = ALL_Vehicles.head;

//    cout<<counter_received<<endl;
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
    for(int i = 0; i < rate.size(); i++){
        cout << arr[i] << " packets with rate = " << rate[i] << endl;
    }

    return 0;
}