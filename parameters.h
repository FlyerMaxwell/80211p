//
// Created by cyx02 on 2022/6/27.
//

#ifndef BUBBLEMAC_INFOCOM_PARAMETERS_H
#define BUBBLEMAC_INFOCOM_PARAMETERS_H

#define TX 0
#define SENS 1
#define PREP 2



#define TX_COLI 0   //同时发射
#define NO_COLI 1   //没有碰撞,正常解包
#define RX_COLI 2   //接收端碰撞

#include <fstream>
using namespace std;


extern int UpLocSlot;
extern int SlotPerFrame;
extern int Car_Number;
extern int cnt_pkt_0;
extern int cnt_pkt;
extern int cnt_pkt_1;
extern int cnt_pkt_2;
extern int cnt_received;
extern int traffic_density;
extern int cnt_coli;
extern int counterToChange;
extern int len_head_resource;
extern int len_tail_resouce;



//---802.11p parameters
extern int counter_collision;
extern int Ts;
extern int pkt_gen_gap;
extern int CW_min;
extern int duration_tx;
extern int counter_received;
extern int counter_tx;

extern int total_frame;



extern bool log_process_flag;
extern  ofstream log_process_file;

extern bool log_statistic_flag;
extern ofstream log_statistic_file;



// Statistic Parameters
extern int cnt_cars;                //当前这一帧中出现的车辆数（其实数最后一个slot就可以，大概估计）1
extern int cnt_tx_collision;        //发射过程的collision，如果一个包发射的时候，其他的包也在发，则产生发射collision
extern int cnt_tx_normal;
extern int cnt_rx_normal;           //正常收包，指完整的一个包的周期只有这一个包，故能够解出
extern int cnt_rx_colli;            //收包过程中的碰撞
extern int cnt_pkt_tx;              //发射个数 发包就统计一下  1
extern int cnt_pkt_tx_normal;       //整个发包周期都没有发生碰撞
extern int cnt_frontV_normal;       //前车发的正常的包
extern int cnt_rearV_normal;        //后车的正常的包
extern int cnt_frontV_colli;        //前车的colli
extern int cnt_rearV_Colli;         //后车的colli


#endif //BUBBLEMAC_INFOCOM_PARAMETERS_H
