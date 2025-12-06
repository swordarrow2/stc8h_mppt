//
// Created by SJF on 2025/12/5.
//

#include "pd.h"

#define ADC_CHANNEL_PD_INPUT_U 6

#define PD_CTRL P23

static uint16_t request_voltage_mv = 5000;

uint8_t pd_init(void) {
    ch224_init();
    return ch224_check_connection();
}

void pd_enable_input(void) {
    PD_CTRL = 1;
}

void pd_disable_input(void) {
    PD_CTRL = 0;
}

uint16_t pd_get_actual_voltage(void) { //mV
    return (uint16_t) (vcc / 4095.0 * adc_read(ADC_CHANNEL_PD_INPUT_U) * 11);
}

uint16_t pd_get_request_voltage_mv(void) {
    return request_voltage_mv;
}

void pd_set_request_voltage_mv(uint16_t mv) {
    uint8_t result = 1;
    result = ch224_request_voltage(mv);
    if (!result) {
        request_voltage_mv = mv;
    }
}

uint16_t pd_get_max_available_current(void) {//mA
    return ch224_read_max_current();
}

const char *pd_get_protocol_type(void) {
    uint8_t voltage_flag;
    uint8_t status;
    voltage_flag = ch224_read_register(0x0A);
    status = ch224_read_status();
    if (status == 0xFF) return "PD OFF";
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
        return "PD";
    }
    if (status & CH224_STATUS_QC3_ACTIVE) return "QC3.0";
    if (status & CH224_STATUS_QC2_ACTIVE) return "QC2.0";
    if (status & CH224_STATUS_BC_ACTIVE) return "BC1.2";

    return "Unknown";
}
