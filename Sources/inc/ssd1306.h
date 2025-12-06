//
// Created by SJF on 2025/12/4.
//

#ifndef STC_MPPT_OLED12864_H
#define STC_MPPT_OLED12864_H

#include <config.h>


// OLED配置
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define OLED_PAGE_COUNT  8      // 128x64 = 8页 x 128列
#define OLED_ADDR        0x78   // I2C地址：0x3C << 1

// 显示模式
#define OLED_NORMAL      0
#define OLED_INVERSE     1
#define OLED_DISPLAY_OFF 2
#define OLED_DISPLAY_ON  3


// 函数声明
void OLED_Init(void);

void OLED_Clear(void);

void OLED_Fill(uint8_t pattern);

void OLED_Update(void);

void OLED_UpdateArea(uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end);

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t state);

void OLED_DrawHLine(uint8_t x1, uint8_t x2, uint8_t y);

void OLED_DrawVLine(uint8_t x, uint8_t y1, uint8_t y2);

void OLED_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void OLED_DrawFillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void OLED_PutChar(uint8_t x, uint8_t y, char ch);

void OLED_PutString(uint8_t x, uint8_t y, const char *str);

void OLED_PutStringPage(uint8_t x, uint8_t page, const char *str);

void OLED_PutNumber(uint8_t x, uint8_t y, int32_t num);

void OLED_DrawBitmap(const uint8_t *bitmap);

void OLED_SetContrast(uint8_t contrast);

void OLED_SetDisplayMode(uint8_t mode);

#endif //STC_MPPT_OLED12864_H
