//
// Created by SJF on 2025/12/6.
//

#include "screen.h"
#include "../inc/oled.h"

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
}

void SCREEN_Update() {
    OLED_UI_UpdateDisplay();
}