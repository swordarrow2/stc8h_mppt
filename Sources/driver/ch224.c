//
// Created by SJF on 2025/12/5.
//

#include "../inc/ch224.h"


uint8_t ch224_i2c_addr = CH224_I2C_ADDR;  // CH224 I2C地址

void CH224_Init(void) {

}

uint8_t CH224_ReadStatus(void) {
    return CH224_ReadRegister(CH224_REG_STATUS);
}

BOOL CH224_SetVoltage(CH224_Voltage_t voltage) {
    if (voltage > CH224Q_VOLTAGE_AVS) {
        return 1;  // 参数错误
    }
    return CH224_WriteRegister(CH224_REG_VOLTAGE, (uint8_t) voltage);
}

////////////////////////////////////////
// 读取当前最大电流值
// 入口参数: current_ma - 输出电流值(单位: mA)
// 函数返回: 0-成功，1-失败
// 说明: 电流值 = 寄存器值 × 50mA
////////////////////////////////////////
uint16_t CH224_ReadMaxCurrent() {
    uint8_t reg_value;  // 单位: 50mA
    reg_value = CH224_ReadRegister(CH224_REG_CURRENT);
    return (uint16_t) reg_value * 50;
}

////////////////////////////////////////
// 读取CH224寄存器
// 入口参数: reg_addr - 寄存器地址
// 函数返回: 寄存器值
////////////////////////////////////////
uint8_t CH224_ReadRegister(uint8_t reg_addr) {
    uint8_t _data = 0xFF;

    // I2C读操作流程:
    // 1. 发送起始信号
    // 2. 发送设备地址(写)
    // 3. 发送寄存器地址
    // 4. 发送重复起始信号
    // 5. 发送设备地址(读)
    // 6. 读取数据
    // 7. 发送停止信号

    I2C_MasterStart();                      // 起始信号

    // 发送设备地址(写) = (7位地址 << 1) | 0
    if (I2C_MasterSendByte((ch224_i2c_addr << 1) | 0)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 0xFF;
    }

    // 发送寄存器地址
    if (I2C_MasterSendByte(reg_addr)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 0xFF;
    }

    I2C_MasterStart();                      // 重复起始信号

    // 发送设备地址(读) = (7位地址 << 1) | 1
    if (I2C_MasterSendByte((ch224_i2c_addr << 1) | 1)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 0xFF;
    }

    // 读取数据，最后发送NAK
    _data = I2C_MasterReadByte(1);           // 1表示发送NAK

    I2C_MasterStop();                       // 停止信号

    return _data;
}

////////////////////////////////////////
// 写入CH224寄存器
// 入口参数: reg_addr - 寄存器地址
//            value    - 要写入的值
// 函数返回: 0-成功，1-失败
////////////////////////////////////////
BOOL CH224_WriteRegister(uint8_t reg_addr, uint8_t value) {
    I2C_MasterStart();                      // 起始信号
    // 发送设备地址(写) = (7位地址 << 1) | 0
    if (I2C_MasterSendByte((ch224_i2c_addr << 1) | 0)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 1;
    }
    // 发送寄存器地址
    if (I2C_MasterSendByte(reg_addr)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 1;
    }
    // 发送数据
    if (I2C_MasterSendByte(value)) {
        I2C_MasterStop();                   // 通信失败，停止
        return 1;
    }
    I2C_MasterStop();                       // 停止信号
    return 0;                               // 成功
}

//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码

////////////////////////////////////////
// 示例: 检测CH224芯片是否连接
// 入口参数: 无
// 函数返回: 1-已连接，0-未连接
////////////////////////////////////////
uint8_t CH224_CheckConnection(void) {
    uint8_t status;

    status = CH224_ReadStatus();

    // 如果读取成功且不是0xFF(默认值)或0x00
    if (status != 0xFF && status != 0x00) {
        return 1;  // 芯片已连接
    }

    return 0;  // 芯片未连接
}

////////////////////////////////////////
// 示例: 请求特定电压
// 入口参数: voltage_mv - 请求电压(单位: mV)
// 函数返回: 0-成功，1-失败
////////////////////////////////////////

uint8_t CH224_RequestVoltage(uint16_t voltage_mv) {
    CH224_Voltage_t voltage_sel;

    // 根据电压值选择档位
    switch (voltage_mv) {
        case 5000:
            voltage_sel = CH224_VOLTAGE_5V;
            break;
        case 9000:
            voltage_sel = CH224_VOLTAGE_9V;
            break;
        case 1200:
            voltage_sel = CH224_VOLTAGE_12V;
            break;
        case 1500:
            voltage_sel = CH224_VOLTAGE_15V;
            break;
        case 2000:
            voltage_sel = CH224_VOLTAGE_20V;
            break;
//        case 2800:
//            voltage_sel = CH224_VOLTAGE_28V;
//            break;
        default:
            return 1;  // 不支持的电压值
    }

    return CH224_SetVoltage(voltage_sel);
}

////////////////////////////////////////
// 启用并设置PPS电压
// 入口参数: voltage_mv - PPS电压值(单位: mV)
// 函数返回: PPS设置结果
// 说明: 电压范围: 3300-21000mV (3.3V-21V)
//        步进: 100mV
////////////////////////////////////////
CH224_PPS_Result_t CH224_EnablePPS(uint16_t voltage_mv) {
    uint8_t pps_value;
    uint8_t status;
    if (voltage_mv < 3300 || voltage_mv > 21000) {
        return CH224_PPS_PARAM_ERROR;  // 电压超出PPS范围
    }
    if ((voltage_mv % 100) != 0) {
        return CH224_PPS_PARAM_ERROR;  // 电压不是100mV的整数倍
    }
    status = CH224_ReadStatus();
    if (!(status & CH224_STATUS_PD_ACTIVE)) {
        return CH224_PPS_NO_PD;  // PD协议未激活
    }
    pps_value = (uint8_t)(voltage_mv / 100);  // 单位: 100mV
    if (CH224_WriteRegister(CH224Q_REG_PPS, pps_value)) {
        return CH224_PPS_I2C_ERROR;
    }
    if (CH224_WriteRegister(CH224_REG_VOLTAGE, CH224Q_VOLTAGE_PPS)) {
        return CH224_PPS_I2C_ERROR;
    }

    return CH224_PPS_OK;
}

////////////////////////////////////////
// 仅设置PPS电压（已处于PPS模式时使用）
// 入口参数: voltage_mv - PPS电压值(单位: mV)
// 函数返回: PPS设置结果
// 说明: 用于在PPS模式下调整电压，不需要重复设置模式
////////////////////////////////////////
CH224_PPS_Result_t CH224_SetPPSVoltage(uint16_t voltage_mv) {
    uint8_t pps_value;
    if (voltage_mv < 3300 || voltage_mv > 21000) {
        return CH224_PPS_PARAM_ERROR;
    }
    if ((voltage_mv % 100) != 0) {
        return CH224_PPS_PARAM_ERROR;
    }
    pps_value = (uint8_t)(voltage_mv / 100);
    if (CH224_WriteRegister(CH224Q_REG_PPS, pps_value)) {
        return CH224_PPS_I2C_ERROR;
    }
    return CH224_PPS_OK;
}

////////////////////////////////////////
// 读取当前PPS电压设置
// 入口参数: voltage_mv - 输出PPS电压值(单位: mV)
// 函数返回: 0-成功，1-失败
////////////////////////////////////////
uint8_t CH224_ReadPPSVoltage(uint16_t *voltage_mv) {
    uint8_t pps_value;
    pps_value = CH224_ReadRegister(CH224Q_REG_PPS);
    if (pps_value == 0xFF) {
        return 1;  // 读取失败
    }
    *voltage_mv = (uint16_t) pps_value * 100;
    return 0;
}

////////////////////////////////////////
// 检查是否处于PPS模式
// 入口参数: 无
// 函数返回: 0-非PPS模式，1-PPS模式
////////////////////////////////////////
uint8_t CH224_IsPPSMode(void) {
    uint8_t voltage_reg = CH224_ReadRegister(CH224_REG_VOLTAGE);
    if (voltage_reg == CH224Q_VOLTAGE_PPS) {
        return 1;  // PPS模式
    }
    return 0;  // 非PPS模式
}

//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>

