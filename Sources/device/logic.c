//
// Created by SJF on 2025/12/6.
//

#include "logic.h"
#include "battery.h"
#include "dcdc.h"
#include "pd.h"
#include "solar.h"

void LOGIC_Init(void) {
    BAT_Init();
    DCDC_Init();
    PD_Init();
    SOLAR_Init();
}


void LOGIC_Update() {

    //TODO

}