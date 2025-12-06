//
// Created by SJF on 2025/12/6.
//

#include "eg2104.h"

#define EG2104_SD P15

void eg2104_enable(void) {
    EG2104_SD = 1;
}

void eg2104_disable(void) {
    EG2104_SD = 0;
}

void eg2104_set_duty(float value) {
    PWM_UpdateDuty(PWMA_CH3, value, PWM_MODE1);
}
