//
// Created by cyx02 on 2022/7/14.
//

#ifndef INC_80211_INFOCOM_PROTOCOL_80211_H
#define INC_80211_INFOCOM_PROTOCOL_80211_H
#include <vector>

using namespace std;

void mac_80211p(struct Duallist *ALL_Vehicles, int slot);
vector<int> count_collisions_received(struct vehicle* aCar);
bool Is_received(struct vehicle* aCar);
void transmit(struct vehicle* aCar, int slot);

void count_collisions_received_each_slot(struct vehicle *aCar, int slot);


#endif //INC_80211_INFOCOM_PROTOCOL_80211_H
