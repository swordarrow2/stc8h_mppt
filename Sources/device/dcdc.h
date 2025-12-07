//
// Created by SJF on 2025/12/6.
//

#ifndef STC_MPPT_DCDC_H
#define STC_MPPT_DCDC_H

#include "../inc/eg2104.h"
#include "battery.h"
#include "solar.h"
#include "pd.h"

typedef enum {
    DCDC_MODE_NORMAL = 0,    // 普通模式，可设置输入输出限压限流
    DCDC_MODE_MPPT           // MPPT模式，扰动观察法
} DCDC_Mode_t;

typedef void (*DCDC_UpdateFunc_t)(void);

void DCDC_Update(void);

void DCDC_Init(void);

void DCDC_Enable(void);

void DCDC_Disable(void);

uint16_t DCDC_GetInputVoltage(void);

uint16_t DCDC_GetInputCurrent(void);

float DCDC_GetDuty(void);

void DCDC_SetMode(DCDC_Mode_t mode);

DCDC_Mode_t DCDC_GetMode(void);

const char *DCDC_GetModeString(void);

void DCDC_SetInputCurrentLimit(uint16_t current_ma);

void DCDC_SetOutputVoltageLimit(uint16_t voltage_mv);

void DCDC_SetOutputCurrentLimit(uint16_t current_ma);

uint16_t DCDC_GetInputCurrentLimit(void);

uint16_t DCDC_GetOutputVoltageLimit(void);

uint16_t DCDC_GetOutputCurrentLimit(void);

#endif //STC_MPPT_DCDC_H