//
// Created by SJF on 2025/12/6.
//

#include "eg2104.h"

#define EG2104_SD P15

void EG2104_Init(void) {
    EG2104_Disable();
    EG2104_SetDuty(0.0f);
}

void EG2104_Enable(void) {
    EG2104_SD = 1;
}

void EG2104_Disable(void) {
    EG2104_SD = 0;
}

void EG2104_SetDuty(float value) {
    PWM_UpdateDuty(PWMA_CH3, value, PWM_MODE1);
}
