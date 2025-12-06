//
// Created by SJF on 2025/12/6.
//

#include "dcdc.h"

#define MAX_DUTY 95.0f

#define ADC_CHANNEL_DCDC_IN_U 7
#define ADC_CHANNEL_DCDC_IN_I 10

static float duty = 0;
static DCDC_Mode_t current_mode = DCDC_MODE_NORMAL;

void DCDC_Update(void) {
    //TODO - 根据模式执行不同的控制逻辑
    switch (current_mode) {
        case DCDC_MODE_NORMAL:
            // 普通电源模式：固定输出电压或电流
            // 这里可以添加固定输出控制逻辑
            break;

        case DCDC_MODE_PD:
            // PD模式：功率传输协议
            // 这里可以添加PD协议协商和控制逻辑
            break;

        case DCDC_MODE_MPPT:
            // MPPT模式：最大功率点跟踪
            // 这里添加MPPT算法逻辑
            // 例如扰动观察法(P&O)或电导增量法
            break;
    }
}

void DCDC_Init(void) {
    EG2104_Init();
}

void DCDC_Enable(void) {
    EG2104_Enable();
}

void DCDC_Disable(void) {
    EG2104_Disable();
}

uint16_t DCDC_GetInputVoltage(void) { //mV
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_DCDC_IN_U) * 11);
}

uint16_t DCDC_GetInputCurrent(void) { //mA
//I=U/R    U=ADC_u/100  R=0.005f
//((vcc / 1024.0 * ADCRead(ADC_CHANNAL_I1)) / 100.0 / 5*1000)
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_DCDC_IN_I) * 2);
}

void DCDC_IncDuty(void) {
    if (duty < MAX_DUTY) {
        duty += 1;
        EG2104_SetDuty(duty);
    }
}

void DCDC_DecDuty(void) {
    if (duty > 0) {
        duty -= 1;
        EG2104_SetDuty(duty);
    }
}

float DCDC_GetDuty(void) {
    return duty;
}

void DCDC_SetDuty(float value) {
    if (value <= MAX_DUTY && 0 < value) {
        duty = value;
        EG2104_SetDuty(value);
    }
}

// 添加模式设置函数
void DCDC_SetMode(DCDC_Mode_t mode) {
    // 模式切换前的处理
    switch (current_mode) {
        case DCDC_MODE_MPPT:
            // 退出MPPT模式时，停止MPPT跟踪
            break;
        case DCDC_MODE_PD:
            // 退出PD模式时的清理工作
        case DCDC_MODE_NORMAL:
            // 退出普通模式时的清理工作
            break;
        default:
            break;
    }
    // 设置新模式
    current_mode = mode;
    // 模式切换后的初始化
    switch (mode) {
        case DCDC_MODE_NORMAL:
            // 普通模式：设置默认参数
            DCDC_SetDuty(50.0f);  // 默认50%占空比
            break;

        case DCDC_MODE_PD:
            // PD模式：初始化PD协议
            // 这里可以添加PD协议初始化
            DCDC_SetDuty(0);  // 从0开始协商
            break;

        case DCDC_MODE_MPPT:
            // MPPT模式：初始化MPPT算法
            // 可以根据当前输入电压电流初始化占空比
            break;
    }
}

DCDC_Mode_t DCDC_GetMode(void) {
    return current_mode;
}

const char *DCDC_GetModeString(void) {
    switch (current_mode) {
        case DCDC_MODE_NORMAL:
            return "DC IN";
        case DCDC_MODE_PD:
            return "USB PD";
        case DCDC_MODE_MPPT:
            return "MPPT";
        default:
            return "Unknown";
    }
}