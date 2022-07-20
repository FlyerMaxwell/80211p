//
// Created by cyx02 on 2022/6/27.
//

#ifndef BUBBLEMAC_INFOCOM_PARAMETERS_H
#define BUBBLEMAC_INFOCOM_PARAMETERS_H

#define TX 0
#define SENS 1
#define PREP 2


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




// Statistic Parameters
extern int cnt_cars;
extern int cnt_tx_collision;
extern int cnt_rx_normal;
extern int cnt_rx_colli;
extern int cnt_pkt_tx;
extern int cnt_pkt_tx_normal;
extern int cnt_frontV_normal;
extern int cnt_rearV_normal;
extern int cnt_frontV_colli;
extern int cnt_rearV_Colli;


#endif //BUBBLEMAC_INFOCOM_PARAMETERS_H
