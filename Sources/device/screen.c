//
// Created by SJF on 2025/12/6.
//

#include "screen.h"
#include "battery.h"
#include "dcdc.h"
#include "pd.h"
#include "solar.h"
#include "../inc/oled.h"

extern uint16_t last_main_loop_time;

uint8_t updateFlag = 0;

void SCREEN_Init(void) {
    OLED_UI_Init();

    OLED_UI_SetRegionLabel(REGION_TOP_LEFT, "PD");
    OLED_UI_SetRegionLabel(REGION_TOP_CENTER, "MPPT");
    OLED_UI_SetRegionLabel(REGION_TOP_RIGHT, "Bat");

    OLED_UI_SetRegionLabel(REGION_MIDDLE_LEFT, "IN I");
    OLED_UI_SetRegionLabel(REGION_MIDDLE_CENTER, "RUN");
    OLED_UI_SetRegionLabel(REGION_MIDDLE_RIGHT, "OUT I");

    OLED_UI_SetRegionLabel(REGION_BOTTOM_LEFT, "USB");
    OLED_UI_SetRegionLabel(REGION_BOTTOM_CENTER, "Power");
    OLED_UI_SetRegionLabel(REGION_BOTTOM_RIGHT, "Duty");
}

void SCREEN_Update() {
    char str_buffer[10];
    uint16_t tmp;

    switch (updateFlag) {
        case REGION_TOP_LEFT:
            OLED_UI_SetRegionLabel(REGION_TOP_LEFT, PD_GetProtocolType());
            tmp = PD_GetActualVoltage();
            if (tmp < 4900) {
                OLED_UI_SetRegionValue(REGION_TOP_LEFT, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
                OLED_UI_SetRegionValue(REGION_TOP_LEFT, str_buffer);
            }
            break;
        case REGION_TOP_CENTER:
            tmp = SOLAR_GetVoltage();
            if (tmp < 4900) {
                OLED_UI_SetRegionValue(REGION_TOP_CENTER, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
                OLED_UI_SetRegionValue(REGION_TOP_CENTER, str_buffer);
            }
            break;
        case REGION_TOP_RIGHT:
            tmp = BAT_GetVoltage();
            if (tmp < 2000) {
                OLED_UI_SetRegionValue(REGION_TOP_RIGHT, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
                OLED_UI_SetRegionValue(REGION_TOP_RIGHT, str_buffer);
            }
            break;
        case REGION_MIDDLE_LEFT:
            sprintf(str_buffer, "%dmA", DCDC_GetInputCurrent());
            OLED_UI_SetRegionValue(REGION_MIDDLE_LEFT, str_buffer);
            break;
        case REGION_MIDDLE_CENTER:
            if (DCDC_GetMode() == DCDC_MODE_NORMAL) {
                OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, "PD--->");
            } else if (DCDC_GetMode() == DCDC_MODE_MPPT) {
                OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, "MPPT->");
            }
            break;
        case REGION_MIDDLE_RIGHT:
            tmp = BAT_GetCurrent();
            if (tmp < 20) {
                OLED_UI_SetRegionValue(REGION_MIDDLE_RIGHT, "NC");
            } else {
                sprintf(str_buffer, "%dmA", tmp);
                OLED_UI_SetRegionValue(REGION_MIDDLE_RIGHT, str_buffer);
            }
            break;
        case REGION_BOTTOM_LEFT:
            OLED_UI_SetRegionValue(REGION_BOTTOM_LEFT, "dummy");
            break;
        case REGION_BOTTOM_CENTER:
            sprintf(str_buffer, "%.2fW", DCDC_GetInputVoltage() / 1000.0f * DCDC_GetInputCurrent() / 1000.0f);
            OLED_UI_SetRegionValue(REGION_BOTTOM_CENTER, str_buffer);
            break;
        case REGION_BOTTOM_RIGHT:
            sprintf(str_buffer, "%.1f%%", DCDC_GetDuty());
            OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, str_buffer);
            break;
    }
    OLED_UI_UpdateDisplay(updateFlag);
    ++updateFlag;
    if (updateFlag == 9) {
        updateFlag = 0;
    }
}