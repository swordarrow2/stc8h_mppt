//
// Created by SJF on 2025/12/6.
//

#ifndef STC_MPPT_DCDC_H
#define STC_MPPT_DCDC_H

#include "../inc/eg2104.h"
#include "battery.h"
#include "dcdc.h"
#include "pd.h"
#include "solar.h"

extern bit logic_dirty;
// 工作模式枚举
typedef enum {
    DCDC_MODE_NORMAL = 0,    // 普通电源模式
    DCDC_MODE_PD,            // PD模式
    DCDC_MODE_MPPT           // MPPT模式
} DCDC_Mode_t;

typedef void (*DCDC_UpdateFunc_t)(void);

void DCDC_Update(void);

void DCDC_Init(void);

void DCDC_Enable(void);

void DCDC_Disable(void);

uint16_t DCDC_GetInputVoltage(void);

uint16_t DCDC_GetInputCurrent(void);

void DCDC_IncDuty(void);

void DCDC_DecDuty(void);

float DCDC_GetDuty(void);

void DCDC_SetDuty(float value);

void DCDC_SetMode(DCDC_Mode_t mode);

DCDC_Mode_t DCDC_GetMode(void);

const char *DCDC_GetModeString(void);


#endif //STC_MPPT_DCDC_H
