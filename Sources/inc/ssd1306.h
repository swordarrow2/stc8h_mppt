//
// Created by SJF on 2025/12/4.
//

#ifndef STC_MPPT_OLED12864_H
#define STC_MPPT_OLED12864_H

#include <config.h>


// OLED配置
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64
#define SSD1306_PAGE_COUNT  8      // 128x64 = 8页 x 128列
#define SSD1306_ADDR        0x78   // I2C地址：0x3C << 1

// 显示模式
#define SSD1306_NORMAL      0
#define SSD1306_INVERSE     1
#define SSD1306_DISPLAY_OFF 2
#define SSD1306_DISPLAY_ON  3


// 函数声明
void SSD1306_Init(void);

void SSD1306_Clear(void);

void SSD1306_Fill(uint8_t pattern);

void SSD1306_Update(void);

void SSD1306_UpdateArea(uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end);

void SSD1306_SetPixel(uint8_t x, uint8_t y, uint8_t state);

void SSD1306_DrawHLine(uint8_t x1, uint8_t x2, uint8_t y);

void SSD1306_DrawVLine(uint8_t x, uint8_t y1, uint8_t y2);

void SSD1306_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void SSD1306_DrawFillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void SSD1306_PutChar(uint8_t x, uint8_t y, char ch);

void SSD1306_PutString(uint8_t x, uint8_t y, const char *str);

void SSD1306_PutStringPage(uint8_t x, uint8_t page, const char *str);

void SSD1306_PutNumber(uint8_t x, uint8_t y, int32_t num);

void SSD1306_DrawBitmap(const uint8_t *bitmap);

void SSD1306_SetContrast(uint8_t contrast);

void SSD1306_SetDisplayMode(uint8_t mode);

#endif //STC_MPPT_OLED12864_H
