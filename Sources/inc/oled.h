//
// Created by SJF on 2025/12/5.
//

#ifndef STC_MPPT_OLED_H
#define STC_MPPT_OLED_H

#include "ssd1306.h"

// 区域定义
#define REGION_TOP_LEFT       0
#define REGION_TOP_CENTER     1
#define REGION_TOP_RIGHT      2
#define REGION_MIDDLE_LEFT    3
#define REGION_MIDDLE_CENTER  4
#define REGION_MIDDLE_RIGHT   5
#define REGION_BOTTOM_LEFT    6
#define REGION_BOTTOM_CENTER  7
#define REGION_BOTTOM_RIGHT   8

// 文本显示模式
#define DISPLAY_LABEL         2  // 只显示标签
#define DISPLAY_VALUE         1  // 只显示数值
#define DISPLAY_BOTH          0  // 显示标签和数值

#define REGION_BORDER_WIDTH   1   // 区域边框厚度（像素）

typedef struct {
    char label[12];
    char value[12];
    uint8_t display_mode;
    uint8_t dirty;
    uint8_t x_start;        // 起始x坐标(0-127)
    uint8_t y_start;        // 起始y坐标(0-63)
    uint8_t width;          // 区域宽度
    uint8_t height;         // 区域高度
} Region_Info;

// 函数声明
void OLED_UI_Init(void);

void OLED_UI_DrawGrid(void);

void OLED_UI_UpdateDisplay(void);

void OLED_UI_SetRegionLabel(uint8_t region, const char *label);

void OLED_UI_SetRegionValue(uint8_t region, const char *value);

void OLED_UI_SetRegionNumber(uint8_t region, int32_t value);

void OLED_UI_SetDisplayMode(uint8_t region, uint8_t mode);

void OLED_UI_ClearRegion(uint8_t region);

void OLED_UI_RefreshAll(void);

#endif //STC_MPPT_OLED_H
