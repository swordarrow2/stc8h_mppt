//
// Created by SJF on 2025/12/5.
//

#include "../inc/oled.h"
#include <string.h>

// 区域定义
static Region_Info regions[9];

// 初始化区域信息
static void init_regions(void) {
    uint8_t region_width, region_height, i, col, row;

    // 计算每个区域的大小（考虑边框）
    region_width = OLED_WIDTH / 3;  // 128/3 ≈ 42.67
    region_height = OLED_HEIGHT / 3; // 64/3 ≈ 21.33

    for (i = 0; i < 9; i++) {
        col = i % 3;
        row = i / 3;

        regions[i].label[0] = '\0';
        regions[i].value[0] = '\0';
        regions[i].display_mode = DISPLAY_LABEL;
        regions[i].dirty = 1;

        // 计算区域坐标（考虑边框）
        regions[i].x_start = col * region_width + REGION_BORDER_WIDTH;
        regions[i].y_start = row * region_height + REGION_BORDER_WIDTH;

        // 计算区域尺寸（减去边框）
        regions[i].width = region_width - (2 * REGION_BORDER_WIDTH);
        regions[i].height = region_height - (2 * REGION_BORDER_WIDTH);
    }
}

// 计算文本居中位置
static uint8_t center_text_x(uint8_t col_start, uint8_t col_end, uint8_t text_len) {
    uint8_t region_width, text_width;

    region_width = col_end - col_start + 1;
    text_width = text_len * 6 - 1;  // 5像素字符 + 1像素间隔，最后一个字符没有间隔
    if (text_width > region_width) return col_start;
    return col_start + (region_width - text_width) / 2;
}

// 绘制一个区域的内容
static void draw_region(uint8_t region_idx) {

    Region_Info *region;
    const char *display_text;
    uint8_t text_width_pixels = 0, center_x = 0, center_y = 0, label_x = 0, label_y = 0, value_x = 0, value_y = 0, x = 0, y = 0, border_x1, border_y1, border_x2, border_y2;

    region = &regions[region_idx];
    // 首先清空整个区域（包括边框）
    for (y = region->y_start - REGION_BORDER_WIDTH; y < region->y_start + region->height + REGION_BORDER_WIDTH; y++) {
        for (x = region->x_start - REGION_BORDER_WIDTH;
             x < region->x_start + region->width + REGION_BORDER_WIDTH; x++) {
            // 确保坐标在屏幕范围内
            if (x < OLED_WIDTH && y < OLED_HEIGHT) {
                OLED_SetPixel(x, y, 0);
            }
        }
    }
    border_x1 = region->x_start - REGION_BORDER_WIDTH;
    border_y1 = region->y_start - REGION_BORDER_WIDTH;
    border_x2 = region->x_start + region->width;
    border_y2 = region->y_start + region->height;

    // 绘制边框（使用实心矩形或空心矩形）
    // 这里使用空心矩形作为边框
    OLED_DrawRect(border_x1, border_y1, border_x2, border_y2);

    // 清空边框内的内容区域（为文本准备）
    for (y = region->y_start; y < region->y_start + region->height; y++) {
        for (x = region->x_start; x < region->x_start + region->width; x++) {
            OLED_SetPixel(x, y, 0);
        }
    }
    // 计算文本居中位置（使用像素坐标）
    text_width_pixels = 0;
    display_text = NULL;
    // 根据显示模式选择显示的文本
    switch (region->display_mode) {
        case DISPLAY_LABEL:
            display_text = region->label;
            break;
        case DISPLAY_VALUE:
            display_text = region->value;
            break;
        case DISPLAY_BOTH:
            // 如果是显示两者，需要分别处理
            break;
        default:
            break;
    }

    if (display_text && strlen(display_text) > 0) {
        text_width_pixels = strlen(display_text) * 6 - 1;  // 5像素字符+1像素间隔
        if (text_width_pixels <= region->width) {
            center_x = region->x_start + (region->width - text_width_pixels) / 2;
            // 计算垂直居中：每个字符高8像素
            center_y = region->y_start + (region->height - 8) / 2;
            OLED_PutString(center_x, center_y, display_text);
        }
    }

    // 处理同时显示标签和数值的情况
    if (region->display_mode == DISPLAY_BOTH) {
        // 显示标签在上方
        if (strlen(region->label) > 0) {
            text_width_pixels = strlen(region->label) * 6 - 1;
            if (text_width_pixels <= region->width) {
                label_x = region->x_start + (region->width - text_width_pixels) / 2;
                label_y = region->y_start + 1;  // 上方留点边距
                OLED_PutString(label_x, label_y, region->label);
            }
        }

        // 显示数值在下方
        if (strlen(region->value) > 0) {
            text_width_pixels = strlen(region->value) * 6 - 1;
            if (text_width_pixels <= region->width) {
                value_x = region->x_start + (region->width - text_width_pixels) / 2;
                value_y = region->y_start + region->height - 9;  // 下方留点边距
                OLED_PutString(value_x, value_y, region->value);
            }
        }
    }

    region->dirty = 0;
}

// 初始化用户界面
void OLED_UI_Init(void) {
    OLED_Init();
    OLED_Clear();

    init_regions();
    OLED_UI_DrawGrid();
    OLED_UI_RefreshAll();
}

// 为所有区域绘制边框
void OLED_UI_DrawGrid(void) {
    uint8_t i, border_x1, border_y1, border_x2, border_y2;
    Region_Info *region;
    // 为每个区域绘制边框
    for (i = 0; i < 9; i++) {
        region = &regions[i];
        border_x1 = region->x_start - REGION_BORDER_WIDTH;
        border_y1 = region->y_start - REGION_BORDER_WIDTH;
        border_x2 = region->x_start + region->width;
        border_y2 = region->y_start + region->height;
        // 绘制空心矩形作为边框
        OLED_DrawRect(border_x1, border_y1, border_x2, border_y2);
    }
    OLED_Update();
}

// 更新显示（只更新脏区域）
void OLED_UI_UpdateDisplay(void) {
    uint8_t i, page_start, page_end, col_start, col_end;

    for (i = 0; i < 9; i++) {
        if (regions[i].dirty) {
            draw_region(i);
            page_start = regions[i].y_start / 8;
            page_end = (regions[i].y_start + regions[i].height - 1) / 8;
            col_start = regions[i].x_start;
            col_end = regions[i].x_start + regions[i].width - 1;
            OLED_UpdateArea(page_start, page_end, col_start, col_end);
        }
    }
}

// 设置区域标签
void OLED_UI_SetRegionLabel(uint8_t region, const char *label) {
    if (region >= 9) return;

    strncpy(regions[region].label, label, sizeof(regions[region].label) - 1);
    regions[region].label[sizeof(regions[region].label) - 1] = '\0';
    regions[region].dirty = 1;
}

// 设置区域数值
void OLED_UI_SetRegionValue(uint8_t region, const char *value) {
    if (region >= 9) return;
    strncpy(regions[region].value, value, sizeof(regions[region].value) - 1);
    regions[region].value[sizeof(regions[region].value) - 1] = '\0';
    regions[region].dirty = 1;
}

// 设置区域数值（整数）
void OLED_UI_SetRegionNumber(uint8_t region, int32_t value) {
    char buffer[12];
    char *ptr;
    int negative;

    if (region >= 9) return;

    ptr = buffer + 11;
    *ptr = '\0';

    negative = 0;
    if (value < 0) {
        negative = 1;
        value = -value;
    } else if (value == 0) {
        *(--ptr) = '0';
    }

    while (value > 0) {
        *(--ptr) = (value % 10) + '0';
        value /= 10;
    }

    if (negative) {
        *(--ptr) = '-';
    }

    OLED_UI_SetRegionValue(region, ptr);
}

// 设置显示模式
void OLED_UI_SetDisplayMode(uint8_t region, uint8_t mode) {
    if (region >= 9 || mode > DISPLAY_BOTH) return;

    regions[region].display_mode = mode;
    regions[region].dirty = 1;
}

// 清除区域内容
void OLED_UI_ClearRegion(uint8_t region) {
    if (region >= 9) return;

    regions[region].label[0] = '\0';
    regions[region].value[0] = '\0';
    regions[region].dirty = 1;
}

// 强制刷新所有区域
void OLED_UI_RefreshAll(void) {
    uint8_t i;

    for (i = 0; i < 9; i++) {
        regions[i].dirty = 1;
    }
    OLED_UI_UpdateDisplay();
}