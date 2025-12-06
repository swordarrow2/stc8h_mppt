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

void SCREEN_Init(void) {
    OLED_UI_Init();

    OLED_UI_SetRegionLabel(REGION_TOP_LEFT, "PD");
    OLED_UI_SetRegionValue(REGION_TOP_LEFT, "12.1V");

    OLED_UI_SetRegionLabel(REGION_TOP_RIGHT, "Solar");
    OLED_UI_SetRegionValue(REGION_TOP_RIGHT, "1.2A");

    OLED_UI_SetRegionLabel(REGION_BOTTOM_LEFT, "USB");
    OLED_UI_SetRegionValue(REGION_BOTTOM_LEFT, "5V");

    OLED_UI_SetRegionLabel(REGION_BOTTOM_RIGHT, "Bat");
    OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, "3.7V");

    OLED_UI_SetRegionLabel(REGION_MIDDLE_CENTER, "Power");
    OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, "13W");

    OLED_UI_UpdateDisplay();
}

void SCREEN_Update() {
    static char str_buffer[6];
    static uint16_t tmp;

    OLED_UI_SetRegionLabel(REGION_TOP_LEFT, PD_GetProtocolType());

    tmp = PD_GetActualVoltage();
    if (tmp < 4900) {
        OLED_UI_SetRegionValue(REGION_TOP_LEFT, "NC");
    } else {
        sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
        OLED_UI_SetRegionValue(REGION_TOP_LEFT, str_buffer);
    }

    tmp = SOLAR_GetVoltage();
    if (tmp < 4200) {
        OLED_UI_SetRegionValue(REGION_TOP_RIGHT, "NC");
    } else {
        sprintf(str_buffer, "%.2fV", tmp / 1000.0f);
        OLED_UI_SetRegionValue(REGION_TOP_RIGHT, str_buffer);
    }

    tmp = BAT_GetVoltage();
    if (tmp < 2000) {
        OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, "NC");
    } else {
        sprintf(str_buffer, "%.2fV", BAT_GetVoltage() / 1000.0f);
        OLED_UI_SetRegionValue(REGION_BOTTOM_RIGHT, str_buffer);
    }

    sprintf(str_buffer, "%.2fW", DCDC_GetInputVoltage() / 1000.0f * DCDC_GetInputCurrent() / 1000.0f);
    OLED_UI_SetRegionValue(REGION_MIDDLE_CENTER, str_buffer);

    OLED_UI_UpdateDisplay();
}