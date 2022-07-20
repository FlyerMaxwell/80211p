//
// Created by cyx02 on 2022/6/27.
//

#ifndef BUBBLEMAC_INFOCOM_UPLOCATION_H
#define BUBBLEMAC_INFOCOM_UPLOCATION_H

#include <string>

using namespace std;

int init_simulation(struct Duallist *ALL_Vehicles);
void updateLocation(struct Duallist *ALL_Vehicles, int slot, string trace_path);
void handle_neighbours(struct Duallist *ALL_Vehicles);


#endif //BUBBLEMAC_INFOCOM_UPLOCATION_H
