//
// Created by SJF on 2025/12/6.
//

#include "dcdc.h"

#define MAX_DUTY 95.0f

#define ADC_CHANNEL_DCDC_INPUT_U 7
#define ADC_CHANNEL_DCDC_INPUT_I 10

static float duty = 0;
static int vcc = 0;
static bit enable_mppt = 0;

void dcdc_update(void) {
    //TODO
}

void dcdc_init() {
    vcc = (int) (4096L * *BGV / adc_read(0xf));
}

void dcdc_enable(void) {
    eg2104_enable();
}

void dcdc_disable(void) {
    eg2104_disable();
}

uint16_t dcdc_get_input_u(void) { //mV
    return (uint16_t) (vcc / 4095.0 * adc_read(ADC_CHANNEL_DCDC_INPUT_U) * 11);
}

uint16_t dcdc_get_input_i(void) { //mA
//I=U/R    U=ADC_u/100  R=0.005f
//((vcc / 1024.0 * ADCRead(ADC_CHANNAL_I1)) / 100.0 / 5*1000)
    return (uint16_t) (vcc / 4095.0 * adc_read(ADC_CHANNEL_DCDC_INPUT_I) * 2);
}

void dcdc_inc_duty(void) {
    if (duty < MAX_DUTY) {
        duty += 1;
        eg2104_set_duty(duty);
    }
}

void dcdc_dec_duty(void) {
    if (duty > 0) {
        duty -= 1;
        eg2104_set_duty(duty);
    }
}

float dcdc_get_duty(void) {
    return duty;
}

void dcdc_set_duty(float value) {
    if (value <= MAX_DUTY && 0 < value) {
        duty = value;
        eg2104_set_duty(value);
    }
}
