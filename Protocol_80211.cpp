//
// Created by cyx02 on 2022/7/14.
//

#include "Protocol_80211.h"
#include <vector>
#include "common.h"
#include "vehicle.h"
#include <iostream>


using namespace std;

/*
 * 这是全新版本的802.11p，协议是怎样的呢？
 *
 * 整个协议非常的简单。一个车辆如果连续听到58微妙的持续idle，则设置一个回退计数器k \in [0, CW],每经过一个Ts，k--。当k减小到0的时候进行发射，发射持续的时间是0.67ms。
 * 每0.1秒产生一个packet
 *
 * 时槽设置为13微秒
 *
 * 每个时槽为1微秒
 * 1）每过0.1秒，enque一个packet
 * 2）sensed_idle ++或者置0，如果置0，则设置一个k。且k--
 * 3）当k = 0, 将包发射出去
 *
 * 与先前类似，发包即为将包挂在周围所有的节点身上
 *
 * 整个过程不需要对包的内容进行求解
 *
 * 车辆总共有几种状态？
 * 1） 正在发射
 * 2） 正在感知累计AIFS
 * 3） 准备发射状态。正在感知降低k
 *
 * 每个时槽1微秒喽
 * 在802.11p中 什么叫碰撞呢？
 * 在同一个接收节点处，同时听到了两个包
 *
 * 所以就是，遍历所有的车。
 * 每0.1秒产生一个广播需求
 *
 * 1） 如果车辆处于正在发射(TX)，则判断是否发射完了，如果没有发射完则保持发射状态；如果发射完了，切换到感知状态，重置AIFS
 * 2） 如果车辆处于感知状态(SENS)，则每经过Ts开始累计AIFS，当累计够了，则设置一个一个参数k，并且切换到准备发射状态；如果没有累计够，就继续累积，如果听到了有人发包则重新归零
 * 3） 如果车辆处于准备发射状态(PREP)，如果听到有人发包，则冻结当前的k，知道再经过AIFS没人说话则开始发包；如果k减小到0了，则开始发包
 *
 * 通过ACK决定是否重传。我们不采用重传的机制
 *
 * 正常是这个过程。但是这样写会有一个严重的问题，就是每一微秒，都要把所有的车辆全部过一遍，这个成本是很高的。有什么办法吗？
 *
 * 第二个问题，就是怎么检测发包过程中的碰撞呢？--->还是handle receiver，只不过这个处理相当于仅仅去看接收处的collision；即当全部车辆在一个时里面发包完成之后。还要再遍历一遍所有的车辆，看一下所有的车辆的收包情况。当然了，这个是不是可以在所有的车辆全部发包完了之后，每个车辆后面都挂了一堆的packet。这个时候进行处理呢？当然可以！这样就不用在每个微秒进行处理了！
 *
 *
 * 所以思路就很清楚了，先写一个大的时间循环，把整个发包的过程模拟出来。再处理一遍所有汽车的packet（其实这个时候有些车辆会消失，所以应该单独存一个数据结构，保存下来所有发的包) 或者直接把出界的车标一个状态，而不是清空掉。或者就直接对于要出界的车，先完成包的统计再清除掉。
 *
 * 如何完成碰撞的统计？
 *
 * 对于一个车来说，收到若干个包，这些包的起止时间是有的，要给一个算法，问有多少交叉。所以就直接保存一个数组楼，这个数组的元素就是[start,end],问交叠的和没交迭的packet分别有多少；这个数据结构倒是可以单独存一下 噢，一层hashtable，id-> 一维数组（存的是pair）的地址
 *
 *
 * 今天还要解决一个问题，就是统计的时候应该只统计安全相关的packets。这个要怎么统计呢？安全相关的包发了多少，接受率，碰撞率如何。

 */

void mac_80211p(struct Duallist *ALL_Vehicles, int slot){
    struct Item * aItem;
    struct vehicle* aCar;
    aItem = ALL_Vehicles->head;
    while(aItem != NULL) {
        aCar = (struct vehicle *) aItem->datap;
        aCar->commRadius = 300;

        //每100ms 添加发包的需求
        if((slot - aCar->slot_appeared)% pkt_gen_gap == 0){//pkt_gen_gap = 100 000,即100毫秒

            aCar->counter_to_TX++;
        }

        if(aCar->condition_80211 == SENS){
            if( (slot - aCar->sense_timestamp) % Ts == 0){
                if(Is_received(aCar) == false){
                    aCar->counter_sense++;
                    if(aCar->counter_sense *Ts >= Ts*2 +32){

                        aCar->condition_80211 = PREP;
                        aCar->k = rand() % CW_min + 2;

                        aCar->prep_timestamp = slot;
                        aCar->counter_froze = 0;
                        aCar->prep_frozen = false;
                    }
                }else{
                    aCar->counter_sense = 0;
                }
            }

        }else if(aCar->condition_80211 == PREP){

            if((slot - aCar->prep_timestamp) % Ts == 0){

                if(aCar->prep_frozen == false && Is_received(aCar) == false){

                    if(aCar->k != 0) aCar->k -= 1;

                    if(aCar->k <= 0 && aCar->counter_to_TX != 0){

                        aCar->condition_80211 = TX;
                        aCar->tx_timestamp = slot;
                        //发包 todo 将生成的包挂到对应位置的接收端
                        transmit(aCar);
                        counter_tx++;

                        aCar->counter_to_TX -=1;
                        aCar->transmitted_packets++;
                    }
                }else if(aCar->prep_frozen == true && Is_received(aCar) == false){

                    aCar->counter_froze++;
                    if(aCar->counter_froze*Ts >= 2*Ts+32){
                        aCar->prep_frozen = false;
                        if(aCar->k != 0) aCar->k -= 1;

                        if(aCar->k <= 0 && aCar->counter_to_TX != 0){


                            aCar->condition_80211 = TX;
                            aCar->tx_timestamp = slot;
                            //发包  将生成的包挂到对应位置的接收端
                            transmit(aCar);
                            counter_tx++;

                            aCar->counter_to_TX -=1;
                            aCar->transmitted_packets++;
                        }
                    }
                }else if(aCar->prep_frozen == false && Is_received(aCar) == true){

                    aCar->prep_frozen = true;
                    aCar->counter_froze = 0;
                }else if(aCar->prep_frozen == true && Is_received(aCar) == true){

                    aCar->counter_froze = 0;
                }else{

                    cerr<<"Error PREP"<<endl;
                }
            }
        }else if(aCar->condition_80211 == TX){
            if(slot == aCar->tx_timestamp + duration_tx){//结束发包

                aCar->condition_80211 = SENS;
                aCar->counter_sense = 0;
                aCar->sense_timestamp = slot;
            }
        }else{
            cerr<<"Error! Strange condition!";
        }
//        if(slot == 19999990) cout <<"slot: " << slot << "  id : " <<  aCar->id << " tx " << aCar->transmitted_packets << endl;
        aItem = aItem->next;
    }


}

//生成一个packet，挂到通信半径范围内的车上
void transmit(struct vehicle* aCar){
    struct Item *bItem;
    struct vehicle *bCar;

    bItem = (struct Item*)aCar->neighbours.head;//遍历当前transmitter的邻居节点
    int cnt = 0, num_neighbour = 0;
    while(bItem != NULL) {
        bCar = (struct vehicle*)bItem->datap;
        double distanceAB = distance_between_vehicle(aCar, bCar);
        if(aCar->commRadius <distanceAB){
            bItem = bItem->next;
        }else{
            num_neighbour++;
            cnt++;
            bCar->packets->push_back(aCar->tx_timestamp);

            bItem = bItem->next;
        }
    }
//    cout << aCar->id << " send " << cnt << " in "<< num_neighbour << endl;
}

//判断是否周围有车在发包。遍历一遍邻居，看有没有在300米内，处于TX的邻居，如果有，则说明有人在发，返回true；否则返回false
bool Is_received(struct vehicle* aCar){
    struct Item *bItem;
    struct vehicle *bCar;

    bItem = (struct Item*)aCar->neighbours.head;//遍历当前transmitter的邻居节点
    while(bItem != NULL) {
        bCar = (struct vehicle*)bItem->datap;
        double distanceAB = distance_between_vehicle(aCar, bCar);
        if(aCar->commRadius < distanceAB){
            bItem = bItem->next;
        }else{
            if(bCar->condition_80211 == TX)
                return true;
            else{
                bItem = bItem->next;
            }
        }
    }
    return false;
}

//数一下车辆有没有碰撞，有的话是多少; 顺便也数一下正常收包的个数
vector<int> count_collisions_received(struct vehicle* aCar){
    int collisions =0;
    vector<int> ans;
    int len = aCar->packets->size();
    if(len == 0)
        return ans;
    if(len == 1){
        counter_received++;
        return vector<int>{0,1};
    }

    vector<int> mark(len,0);

    for(int i = 0; i < len - 1; i++){
        if(   (*(aCar->packets))[i]+ duration_tx >=  (*(aCar->packets))[i+1]  ) {
            mark[i] = -1;
            mark[i+1] = -1;
        }
    }

    for(int i = 0; i< mark.size(); i++){
        if(mark[i] == -1){
            collisions++;
            counter_collision++;
        }


    }
    counter_received += len - collisions; // 更新全局的正常收包个数

//    cout<<" len="<<len<<", received="<<len - collisions<<", collision="<<collisions<<endl;

    ans.push_back(collisions);
    ans.push_back(len - collisions);
    return ans;
}