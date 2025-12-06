//
// Created by SJF on 2025/12/5.
//

#include "battery.h"


#define ADC_CHANNEL_BAT_OUT_U 8
#define ADC_CHANNEL_BAT_OUT_I 9

#define BAT_CTRL P26

uint8_t BAT_Init(void) {
    return 0;
}

void BAT_EnableOutput(void) {
    BAT_CTRL = 1;
}

void BAT_DisableOutput(void) {
    BAT_CTRL = 0;
}

uint16_t BAT_GetVoltage(void) { //mV
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_BAT_OUT_U) * 11);
}

uint16_t BAT_GetCurrent(void) { //mA
//I=U/R    U=ADC_u/100  R=0.005f
//((vcc / 1024.0 * ADCRead(ADC_CHANNAL_I1)) / 100.0 / 5*1000)
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_BAT_OUT_I) * 2);
}