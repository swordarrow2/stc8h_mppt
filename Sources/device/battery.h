//
// Created by SJF on 2025/12/5.
//

#ifndef STC_MPPT_BATTERY_H
#define STC_MPPT_BATTERY_H

#include "../inc/config.h"

uint8_t BAT_Init(void);

void BAT_EnableOutput(void);

void BAT_DisableOutput(void);

uint16_t BAT_GetVoltage(void); //mV

uint16_t BAT_GetCurrent(void); //mA

#endif //STC_MPPT_BATTERY_H
