//
// Created by SJF on 2025/12/5.
//

#ifndef STC_MPPT_CH224_H
#define STC_MPPT_CH224_H

#include <config.h>  // 包含I2C底层驱动

// CH224 I2C 地址定义 (7位地址，不含读写位)
#define CH224_I2C_ADDR_0    0x22  // I2C地址选项1
#define CH224_I2C_ADDR_1    0x23  // I2C地址选项2
#define CH224_I2C_ADDR      CH224_I2C_ADDR_0  // 默认使用地址1

// 寄存器地址定义
#define CH224_REG_STATUS    0x09  // R 状态
#define CH224_REG_VOLTAGE   0x0A  // W 电压控制
#define CH224_REG_CURRENT   0x50  // R PD最大可用电流电流
#define CH224Q_REG_AVS_HIGH  0x51  // W AVS电压配置高字节
#define CH224Q_REG_AVS_LOW   0x52  // W AVS电压配置低字节
#define CH224Q_REG_PPS       0x53  // W PPS电压配置寄存器
#define CH224Q_REG_PD_DATA   0x60  // R PD电源数据寄存器起始地址

// 电压档位定义 (写入电压控制寄存器)
typedef enum {
    CH224_VOLTAGE_5V = 0,  // 5V
    CH224_VOLTAGE_9V = 1,  // 9V
    CH224_VOLTAGE_12V = 2,  // 12V
    CH224_VOLTAGE_15V = 3,  // 15V
    CH224_VOLTAGE_20V = 4,  // 20V
    CH224_VOLTAGE_28V = 5,  // 28V
    CH224Q_VOLTAGE_PPS = 6,  // PPS模式
    CH224Q_VOLTAGE_AVS = 7   // AVS模式
} CH224_Voltage_t;

typedef enum {
    CH224_PPS_OK = 0,        // PPS设置成功
    CH224_PPS_PARAM_ERROR,   // 参数错误
    CH224_PPS_I2C_ERROR,     // I2C通信错误
    CH224_PPS_NOT_SUPPORTED, // 芯片不支持PPS
    CH224_PPS_NO_PD          // PD协议未激活
} CH224_PPS_Result_t;

// 协议状态位定义 (读取状态寄存器)
#define CH224_STATUS_BC_ACTIVE   (1 << 0)  // BC协议激活
#define CH224_STATUS_QC2_ACTIVE  (1 << 1)  // QC2.0协议激活
#define CH224_STATUS_QC3_ACTIVE  (1 << 2)  // QC3.0协议激活
#define CH224_STATUS_PD_ACTIVE   (1 << 3)  // PD协议激活
#define CH224_STATUS_EPR_ACTIVE  (1 << 4)  // EPR模式激活
#define CH224_STATUS_EPR_EXIST   (1 << 5)  // EPR模式存在
#define CH224_STATUS_AVS_EXIST   (1 << 6)  // AVS模式存在

// 函数声明
void CH224_Init(void);

uint8_t CH224_CheckConnection(void);

uint8_t CH224_ReadStatus(void);

BOOL CH224_SetVoltage(CH224_Voltage_t voltage);

uint8_t CH224_ReadMaxCurrent(uint16_t *current_ma);

uint8_t CH224_ReadRegister(uint8_t reg_addr);

BOOL CH224_WriteRegister(uint8_t reg_addr, uint8_t value);

uint8_t CH224_RequestVoltage(uint16_t voltage_mv);

CH224_PPS_Result_t CH224_EnablePPS(uint16_t voltage_mv);

CH224_PPS_Result_t CH224_SetPPSVoltage(uint16_t voltage_mv);

#endif //STC_MPPT_CH224_H
