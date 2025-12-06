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
uint8_t pd_init(void);

void pd_enable_input(void);

void pd_disable_input(void);

uint16_t pd_get_actual_voltage(void);

uint16_t pd_get_request_voltage_mv(void);

void pd_set_request_voltage_mv(uint16_t mv);

uint16_t pd_get_max_available_current(void);

const char *pd_get_protocol_type(void);

#endif //STC_MPPT_PD_H
