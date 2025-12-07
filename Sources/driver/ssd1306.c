//
// Created by SJF on 2025/12/4.
//
// oled12864.c
// SSD1306 OLED 128x64驱动程序 - C51版本，支持5x8字体

#include "../inc/ssd1306.h"
#include "../inc/oledfont.h"

// OLED内部显存（8页 x 128列）
static uint8_t oled_buffer[SSD1306_PAGE_COUNT][SSD1306_WIDTH];

// 发送命令到OLED
static void SSD1306_WriteCommand(uint8_t cmd) {
    I2C_MasterStart();
    I2C_MasterSendByte(SSD1306_ADDR);      // 发送设备地址
    I2C_MasterSendByte(0x00);           // 控制字节：命令模式
    I2C_MasterSendByte(cmd);            // 发送命令
    I2C_MasterStop();
}

// 发送数据到OLED
static void SSD1306_WriteData(uint8_t _data) {
    I2C_MasterStart();
    I2C_MasterSendByte(SSD1306_ADDR);      // 发送设备地址
    I2C_MasterSendByte(0x40);           // 控制字节：数据模式
    I2C_MasterSendByte(_data);           // 发送数据
    I2C_MasterStop();
}

// 批量发送数据（优化：减少I2C启动/停止次数）
static void SSD1306_WriteDataBulk(const uint8_t *_data, uint16_t length) {
    uint16_t i;

    if (length == 0) return;

    I2C_MasterStart();
    I2C_MasterSendByte(SSD1306_ADDR);      // 发送设备地址
    I2C_MasterSendByte(0x40);           // 控制字节：数据模式

    // 批量发送数据
    for (i = 0; i < length; i++) {
        I2C_MasterSendByte(_data[i]);
    }

    I2C_MasterStop();
}

// OLED初始化
void SSD1306_Init(void) {
    // 初始化I2C（如果外部未初始化）
    I2C_Init();

    // 等待OLED上电稳定
    delay_ms(100);

    // 初始化命令序列
    SSD1306_WriteCommand(0xAE);            // 关闭显示

    SSD1306_WriteCommand(0xD5);            // 设置显示时钟分频比/振荡器频率
    SSD1306_WriteCommand(0x80);            // 建议值

    SSD1306_WriteCommand(0xA8);            // 设置多路复用率
    SSD1306_WriteCommand(0x3F);            // 63 (64行-1)

    SSD1306_WriteCommand(0xD3);            // 设置显示偏移
    SSD1306_WriteCommand(0x00);            // 无偏移

    SSD1306_WriteCommand(0x40);            // 设置显示开始行

    SSD1306_WriteCommand(0xA0);            // 设置段重映射：0xA1正常，0xA0镜像

    SSD1306_WriteCommand(0xC0);            // 设置COM扫描方向：0xC8正常，0xC0镜像

    SSD1306_WriteCommand(0xDA);            // 设置COM硬件配置
    SSD1306_WriteCommand(0x12);            // 启用备用COM引脚配置

    SSD1306_WriteCommand(0x81);            // 设置对比度控制
    SSD1306_WriteCommand(0x7F);            // 对比度值：0-255

    SSD1306_WriteCommand(0xA4);            // 禁用整个显示亮起

    SSD1306_WriteCommand(0xA6);            // 正常显示（0xA7为反色）

    SSD1306_WriteCommand(0xD9);            // 设置预充电周期
    SSD1306_WriteCommand(0xF1);            // 建议值

    SSD1306_WriteCommand(0xDB);            // 设置VCOMH去选择级别
    SSD1306_WriteCommand(0x30);            // 0.83 x Vcc

    SSD1306_WriteCommand(0x8D);            // 设置充电泵
    SSD1306_WriteCommand(0x14);            // 启用内部DC/DC

    SSD1306_WriteCommand(0xAF);            // 开启显示

    // 清屏
    SSD1306_Clear();
    SSD1306_Update();
}

// 设置显示窗口（优化显示更新）
static void SSD1306_SetWindow(uint8_t page, uint8_t col_start, uint8_t col_end) {
    SSD1306_WriteCommand(0xB0 + page);     // 设置页地址
    SSD1306_WriteCommand(col_start & 0x0F); // 设置列地址低4位
    SSD1306_WriteCommand(0x10 | ((col_start >> 4) & 0x0F)); // 设置列地址高4位

    // 如果列结束地址不为0xFF，设置列地址模式
    if (col_end != 0xFF) {
        SSD1306_WriteCommand(0x21);        // 设置列地址
        SSD1306_WriteCommand(col_start);
        SSD1306_WriteCommand(col_end);
    }
}

// 清屏（清空缓冲区）
void SSD1306_Clear(void) {
    memset(oled_buffer, 0x00, sizeof(oled_buffer));
}

// 全屏填充
void SSD1306_Fill(uint8_t pattern) {
    memset(oled_buffer, pattern, sizeof(oled_buffer));
}

// 更新整个显示屏
void SSD1306_Update(void) {
    uint8_t page;
    // 1. 绘制所有区域到显存
    for (page = 0; page < SSD1306_PAGE_COUNT; page++) {
        SSD1306_SetWindow(page, 0, SSD1306_WIDTH - 1);
        SSD1306_WriteDataBulk(oled_buffer[page], SSD1306_WIDTH);
//        delay_ms(15);
    }
}

// 更新指定区域（部分刷新，提高性能）
void SSD1306_UpdateArea(uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end) {
    uint8_t page;

    if (page_start > page_end || col_start > col_end) return;

    for (page = page_start; page <= page_end; page++) {
        SSD1306_SetWindow(page, col_start, col_end);
        SSD1306_WriteDataBulk(&oled_buffer[page][col_start], col_end - col_start + 1);
    }
}

// 设置像素点（坐标：x:0-127, y:0-63）
void SSD1306_SetPixel(uint8_t x, uint8_t y, uint8_t state) {
    uint8_t page, _bit;

    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

    page = y >> 3;  // y/8
    _bit = y & 0x07; // y%8

    if (state) {
        oled_buffer[page][x] |= (1 << _bit);   // 设置像素
    } else {
        oled_buffer[page][x] &= ~(1 << _bit);  // 清除像素
    }
}

// 绘制水平线
void SSD1306_DrawHLine(uint8_t x1, uint8_t x2, uint8_t y) {
    uint8_t x, temp;

    if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }

    for (x = x1; x <= x2; x++) {
        SSD1306_SetPixel(x, y, 1);
    }
}

// 绘制垂直线
void SSD1306_DrawVLine(uint8_t x, uint8_t y1, uint8_t y2) {
    uint8_t y, temp;

    if (y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for (y = y1; y <= y2; y++) {
        SSD1306_SetPixel(x, y, 1);
    }
}

// 绘制矩形
void SSD1306_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    SSD1306_DrawHLine(x1, x2, y1);     // 上边
    SSD1306_DrawHLine(x1, x2, y2);     // 下边
    SSD1306_DrawVLine(x1, y1, y2);     // 左边
    SSD1306_DrawVLine(x2, y1, y2);     // 右边
}

// 绘制实心矩形
void SSD1306_DrawFillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t y;

    if (x1 > x2 || y1 > y2) return;

    for (y = y1; y <= y2; y++) {
        SSD1306_DrawHLine(x1, x2, y);
    }
}

// 显示字符（5x8字体）- 针对5x8字体优化
void SSD1306_PutChar(uint8_t x, uint8_t y, char ch) {
    uint8_t i, page, bit_offset, _data, mask_low, mask_high;
    uint16_t index;
    // 检查边界
    if (x >= SSD1306_WIDTH - 4 || y >= SSD1306_HEIGHT) return;
    // 计算字体数据索引（ASCII 32-126）
    if (ch < 32 || ch > 126) ch = ' ';
    index = ch - 32;
    // 计算页和位偏移
    page = y >> 3;          // 页 = y/8
    bit_offset = y & 0x07;  // 页内行偏移
    // 处理5列数据
    for (i = 0; i < 5; i++) {
        _data = font5x8[index][i];
        // 如果bit_offset为0，直接写入当前页
        if (bit_offset == 0) {
            oled_buffer[page][x + i] = _data;
        } else {
            // 需要跨页处理
            // 创建掩码：清除将要写入的区域
            mask_low = 0xFF << bit_offset;    // 当前页要清除的位
            mask_high = 0xFF >> (8 - bit_offset); // 下一页要清除的位
            oled_buffer[page][x + i] &= ~mask_low;    // 清除当前页对应位置
            oled_buffer[page][x + i] |= (_data << bit_offset); // 写入当前页部分：data的低(8-bit_offset)位
            oled_buffer[page + 1][x + i] &= ~mask_high;
            oled_buffer[page + 1][x + i] |= (_data >> (8 - bit_offset));
        }
    }
    // 字符间添加1像素间隔
    if (bit_offset == 0) {
        oled_buffer[page][x + 5] = 0x00;
    } else {
        oled_buffer[page][x + 5] &= ~((0xFF << bit_offset) & 0xFF);
        if (page + 1 < SSD1306_PAGE_COUNT) {
            oled_buffer[page + 1][x + 5] &= ~(0xFF >> (8 - bit_offset));
        }
    }
}

// 显示字符串（5x8字体）- 优化版本
void SSD1306_PutString(uint8_t x, uint8_t y, const char *str) {
    uint8_t cursor_x = x;
    uint8_t page = y >> 3;
    uint8_t bit_offset = y & 0x07;

    while (*str) {
        SSD1306_PutChar(cursor_x, y, *str++);
        cursor_x += 6;  // 5列字符 + 1列间隔

        // 换行处理
        if (cursor_x > SSD1306_WIDTH - 6) {
            cursor_x = x;
            y += 8;  // 下移8像素（下一行）
            if (y >= SSD1306_HEIGHT) break;
        }
    }
}

// 显示字符串在指定页（更方便的API）
void SSD1306_PutStringPage(uint8_t x, uint8_t page, const char *str) {
    uint8_t cursor_x = x;

    while (*str) {
        SSD1306_PutChar(cursor_x, page * 8, *str++);
        cursor_x += 6;  // 5列字符 + 1列间隔

        // 换行处理
        if (cursor_x > SSD1306_WIDTH - 6) {
            cursor_x = x;
            page++;
            if (page >= SSD1306_PAGE_COUNT) break;
        }
    }
}

// 显示数字（5x8字体）
void SSD1306_PutNumber(uint8_t x, uint8_t y, int32_t num) {
    char buffer[12];
    char *ptr = buffer + 11;
    int negative = 0;

    *ptr = '\0';

    if (num < 0) {
        negative = 1;
        num = -num;
    } else if (num == 0) {
        *(--ptr) = '0';
    }

    while (num > 0) {
        *(--ptr) = (num % 10) + '0';
        num /= 10;
    }

    if (negative)
        *(--ptr) = '-';

    SSD1306_PutString(x, y, ptr);
}

// 显示数字在指定页
void SSD1306_PutNumberPage(uint8_t x, uint8_t page, int32_t num) {
    SSD1306_PutNumber(x, page * 8, num);
}

// 显示图像（128x64，1位深度）
void SSD1306_DrawBitmap(const uint8_t *bitmap) {
    memcpy(oled_buffer, bitmap, sizeof(oled_buffer));
}

// 设置对比度
void SSD1306_SetContrast(uint8_t contrast) {
    SSD1306_WriteCommand(0x81);            // 设置对比度控制
    SSD1306_WriteCommand(contrast);
}

// 设置显示模式
void SSD1306_SetDisplayMode(uint8_t mode) {
    switch (mode) {
        case SSD1306_NORMAL:
            SSD1306_WriteCommand(0xA6);    // 正常显示
            break;
        case SSD1306_INVERSE:
            SSD1306_WriteCommand(0xA7);    // 反色显示
            break;
        case SSD1306_DISPLAY_OFF:
            SSD1306_WriteCommand(0xAE);    // 关闭显示
            break;
        case SSD1306_DISPLAY_ON:
            SSD1306_WriteCommand(0xAF);    // 开启显示
            break;
    }
}

// 绘制位图（任意位置和大小）
void SSD1306_DrawBitmapCustom(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap) {
    uint8_t i, j;
    uint16_t bmp_index = 0;

    // 按列处理
    for (i = 0; i < width; i++) {
        // 按行处理
        for (j = 0; j < height; j++) {
            // 计算位图中的像素
            uint8_t byte_index = j >> 3;
            uint8_t bit_index = j & 0x07;
            uint8_t pixel = (bitmap[bmp_index + byte_index] >> bit_index) & 0x01;

            // 设置OLED像素
            SSD1306_SetPixel(x + i, y + j, pixel);
        }

        // 移动到下一列数据
        bmp_index += (height + 7) >> 3;  // 字节对齐
    }
}

