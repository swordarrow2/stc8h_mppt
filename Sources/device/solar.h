//
// Created by SJF on 2025/12/5.
//

#ifndef STC_MPPT_SOLAR_H
#define STC_MPPT_SOLAR_H

#include "config.h"

uint8_t SOLAR_Init(void);

void SOLAR_EnableInput(void);

void SOLAR_DisableInput(void);

uint16_t SOLAR_GetActualVoltage(void);//mV

#endif //STC_MPPT_SOLAR_H
