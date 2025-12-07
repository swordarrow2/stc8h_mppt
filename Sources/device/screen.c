//
// Created by SJF on 2025/12/6.
//

#include "screen.h"
#include "battery.h"
#include "dcdc.h"
#include "pd.h"
#include "solar.h"
#include "../inc/oled.h"

bit screen_dirty = 0;
extern uint16_t last_main_loop_time;

uint8_t updateFlag = 0;

void SCREEN_Init(void) {
    OLED_UI_Init();

    OLED_UI_SetRegionLabel(REGION_TOP_LEFT, "PD");
    OLED_UI_SetRegionValue(REGION_TOP_LEFT, "0.0V");

    OLED_UI_SetRegionLabel(REGION_TOP_RIGHT, "Solar");
    OLED_UI_SetRegionValue(REGION_TOP_RIGHT, "0.0A");

    OLED_UI_SetRegionLabel(REGION_BOTTOM_RIGHT, "Bat");
    OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, "0.0V");

    OLED_UI_SetRegionLabel(REGION_MIDDLE_CENTER, "Power");
    OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, "0W");

    OLED_UI_SetRegionLabel(REGION_BOTTOM_CENTER, "Time");
    OLED_UI_SetRegionValue(REGION_BOTTOM_CENTER, "9961");

    OLED_UI_SetRegionLabel(REGION_MIDDLE_LEFT, "IN I");

    OLED_UI_SetRegionLabel(REGION_MIDDLE_RIGHT, "BAT I");

//    OLED_UI_UpdateDisplay(0);
//    OLED_UI_UpdateDisplay(2);
//    OLED_UI_UpdateDisplay(8);
//    OLED_UI_UpdateDisplay(4);
//    OLED_UI_UpdateDisplay(7);
}

void SCREEN_Update() {
    char str_buffer[10];
    uint16_t tmp;

    switch (updateFlag) {
        case 0:
            OLED_UI_SetRegionLabel(REGION_TOP_LEFT, PD_GetProtocolType());
            tmp = PD_GetActualVoltage();
            if (tmp < 4900) {
                OLED_UI_SetRegionValue(REGION_TOP_LEFT, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
                OLED_UI_SetRegionValue(REGION_TOP_LEFT, str_buffer);
            }
            break;
        case 1:
            break;
        case 2:
            tmp = SOLAR_GetVoltage();
            if (tmp < 4200) {
                OLED_UI_SetRegionValue(REGION_TOP_RIGHT, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
                OLED_UI_SetRegionValue(REGION_TOP_RIGHT, str_buffer);
            }
            break;
        case 3:
            sprintf(str_buffer, "%dmA", DCDC_GetInputCurrent());
            OLED_UI_SetRegionValue(REGION_MIDDLE_LEFT, str_buffer);
            break;
        case 4:
            sprintf(str_buffer, "%.2fW", DCDC_GetInputVoltage() / 1000.0f * DCDC_GetInputCurrent() / 1000.0f);
            OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, str_buffer);
            break;
        case 5:
            sprintf(str_buffer, "%dmA", BAT_GetCurrent());
            OLED_UI_SetRegionValue(REGION_MIDDLE_RIGHT, str_buffer);
            break;
        case 6:
            OLED_UI_SetRegionLabel(REGION_BOTTOM_LEFT, "USB");
            OLED_UI_SetRegionValue(REGION_BOTTOM_LEFT, "5V");
            break;
        case 7:
            sprintf(str_buffer, "%dHz", 1000 / last_main_loop_time);
            OLED_UI_SetRegionValue(REGION_BOTTOM_CENTER, str_buffer);
            break;
        case 8:
            tmp = BAT_GetVoltage();
            if (tmp < 2000) {
                OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, "NC");
            } else {
                sprintf(str_buffer, "%.2fV", BAT_GetVoltage() / 1000.0f);
                OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, str_buffer);
            }
            break;
    }
    OLED_UI_UpdateDisplay(updateFlag);
    ++updateFlag;
    if (updateFlag == 9) {
        updateFlag = 0;
    }
}