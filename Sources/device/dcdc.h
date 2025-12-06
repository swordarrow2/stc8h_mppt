//
// Created by SJF on 2025/12/6.
//

#ifndef STC_MPPT_DCDC_H
#define STC_MPPT_DCDC_H

#include "../inc/eg2104.h"

void dcdc_update(void);

void dcdc_init();

void dcdc_enable(void);

void dcdc_disable(void);

uint16_t dcdc_get_input_u(void);

uint16_t dcdc_get_input_i(void);

void dcdc_inc_duty(void);

void dcdc_dec_duty(void);

float dcdc_get_duty(void);

void dcdc_set_duty(float value);

#endif //STC_MPPT_DCDC_H
