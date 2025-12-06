//
// Created by SJF on 2025/12/5.
//

#ifndef STC_MPPT_PD_H
#define STC_MPPT_PD_H

#include "../inc/config.h"

// 宏定义
#define ADC_CHANNEL_PD_INPUT_U 6
#define PD_CTRL P23

// 函数声明
uint8_t PD_Init(void);

uint8_t PD_Is_Connecting(void);

void PD_EnableInput(void);

void PD_DisableInput(void);

uint16_t PD_GetActualVoltage(void);

uint16_t PD_GetRequestVoltage(void);

void PD_SetRequestVoltage(uint16_t mv);

uint16_t PD_GetMaxAvailableCurrent(void);

const char *PD_GetProtocolType(void);

#endif //STC_MPPT_PD_H