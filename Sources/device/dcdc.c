//
// Created by SJF on 2025/12/6.
//

#include "dcdc.h"

#define MAX_DUTY 95.0f

#define ADC_CHANNEL_DCDC_IN_U 7
#define ADC_CHANNEL_DCDC_IN_I 10

static float duty = 0;
static DCDC_Mode_t current_mode = DCDC_MODE_NORMAL;
static DCDC_UpdateFunc_t current_update_func = NULL;

bit logic_dirty = 0;

static void DCDC_Update_Normal(void);

static void DCDC_Update_PD(void);

static void DCDC_Update_MPPT(void);

static const DCDC_UpdateFunc_t mode_update_funcs[] = {
        DCDC_Update_Normal,
        DCDC_Update_PD,
        DCDC_Update_MPPT
};

// 普通电源模式更新函数
static void DCDC_Update_Normal(void) {
    // 普通电源模式：固定输出电压或电流
    // 这里可以实现固定电压/电流控制逻辑

}

// PD模式更新函数
static void DCDC_Update_PD(void) {
    if (BAT_GetVoltage() < 7500) {
        DCDC_IncDuty();
    } else {
        DCDC_DecDuty();
    }

}

// MPPT模式更新函数
static void DCDC_Update_MPPT(void) {
    // MPPT模式：最大功率点跟踪
    static float last_power = 0;
    static float last_duty = 0;
    float voltage, current, power;
    // 假设每100ms执行一次MPPT算法
    // if (GetCurrentTime() - last_mppt_time >= 100) {
    //     last_mppt_time = GetCurrentTime();

    // 获取当前输入电压电流
    voltage = DCDC_GetInputVoltage() / 1000.0f;  // 转换为V
    current = DCDC_GetInputCurrent() / 1000.0f;  // 转换为A
    power = voltage * current;

    // 扰动观察法(P&O)实现
    if (power > last_power) {
        // 功率增加，继续同方向扰动
        if (duty > last_duty) {
            DCDC_IncDuty();
        } else {
            DCDC_DecDuty();
        }
    } else {
        // 功率减小，改变扰动方向
        if (duty > last_duty) {
            DCDC_DecDuty();
        } else {
            DCDC_IncDuty();
        }
    }

    last_power = power;
    last_duty = duty;
    // }
}

// 主更新函数 - 直接调用当前模式的函数指针
void DCDC_Update(void) {
    if (PD_Is_Connecting()) {
        DCDC_SetMode(DCDC_MODE_PD);
        BAT_EnableOutput();
        DCDC_Enable();
    } else if (SOLAR_GetVoltage() > PD_GetActualVoltage()) {
        DCDC_SetMode(DCDC_MODE_PD);
        BAT_EnableOutput();
        DCDC_Enable();
    }
//TODO:MPPT
    if (current_update_func != NULL) {
        current_update_func();
    }
}

void DCDC_Init(void) {
    EG2104_Init();
    current_update_func = mode_update_funcs[current_mode];
    BAT_Init();
    PD_Init();
    SOLAR_Init();
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
            SOLAR_DisableInput();
            break;
        case DCDC_MODE_PD:
            // 退出PD模式时的清理工作
            PD_DisableInput();
        case DCDC_MODE_NORMAL:
            // 退出普通模式时的清理工作
            SOLAR_DisableInput();
            break;
        default:
            break;
    }
    // 设置新模式
    current_mode = mode;
    current_update_func = mode_update_funcs[mode];
    DCDC_SetDuty(0.0f);
    // 模式切换后的初始化
    switch (mode) {
        case DCDC_MODE_NORMAL:
            // 普通模式：设置默认参数
            SOLAR_EnableInput();
            break;

        case DCDC_MODE_PD:
            // PD模式：初始化PD协议
            // 这里可以添加PD协议初始化
            PD_EnableInput();
            DCDC_SetDuty(0.0f);
            break;

        case DCDC_MODE_MPPT:
            // MPPT模式：初始化MPPT算法
            // 可以根据当前输入电压电流初始化占空比
            SOLAR_EnableInput();
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