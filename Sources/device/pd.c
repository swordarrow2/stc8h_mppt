//
// Created by SJF on 2025/12/5.
//

#include "pd.h"
#include "ch224.h"

#define ADC_CHANNEL_PD_IN_VOLTAGE 6

#define PD_CTRL P23

static uint16_t request_voltage = 5000; //mV

uint8_t PD_Init(void) {
    CH224_Init();
    return 0;
}

uint8_t PD_Is_Connecting(void) {
    return CH224_CheckConnection();
}

void PD_EnableInput(void) {
    PD_CTRL = 1;
}

void PD_DisableInput(void) {
    PD_CTRL = 0;
}

uint16_t PD_GetActualVoltage(void) { //mV
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_PD_IN_VOLTAGE) * 11);
}

uint16_t PD_GetRequestVoltage(void) {
    return request_voltage;
}

void PD_SetRequestVoltage(uint16_t mv) {
    uint8_t result = 1;
    result = CH224_RequestVoltage(mv);
    if (!result) {
        request_voltage = mv;
    }
}

uint16_t PD_GetMaxAvailableCurrent(void) {//mA
    uint16_t result;
    result = CH224_ReadMaxCurrent();
    if (result == 0xFF) {
        return 1;
    }
    return result;
}

const char *PD_GetProtocolType(void) {
    uint8_t voltage_flag;
    uint8_t status;
    voltage_flag = CH224_ReadRegister(0x0A);
    status = CH224_ReadStatus();
    if (status == 0xFF || status == 0x00) return "PD";
    if (status & CH224_STATUS_PD_ACTIVE) {
        if (voltage_flag == 6) {
            return "PD-PPS";
        }
        if (voltage_flag == 7) {
            return "PD-AVS";
        }
        if (status & CH224_STATUS_EPR_ACTIVE) {
            return "PD-EPR";
        }
        return "USB PD";
    }
    if (status & CH224_STATUS_QC3_ACTIVE) return "QC3.0";
    if (status & CH224_STATUS_QC2_ACTIVE) return "QC2.0";
    if (status & CH224_STATUS_BC_ACTIVE) return "BC1.2";

    return "PD";
}