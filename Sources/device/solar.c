//
// Created by SJF on 2025/12/5.
//

#include "solar.h"

#define ADC_CHANNEL_SOLAR_IN_VOLTAGE 11

#define SOLAR_CTRL P27

uint8_t SOLAR_Init(void) {
    return 0;
}

void SOLAR_EnableInput(void) {
    SOLAR_CTRL = 1;
}

void SOLAR_DisableInput(void) {
    SOLAR_CTRL = 0;
}

uint16_t SOLAR_GetActualVoltage(void) { //mV
    return (uint16_t)(vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_SOLAR_IN_VOLTAGE) * 11);
}