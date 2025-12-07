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

// 输入输出限压限流值（单位：mV, mA）
static uint16_t input_current_limit = 3000;   // 默认3A
static uint16_t output_voltage_limit = 4200; // 默认4.2V
static uint16_t output_current_limit = 200;  // 默认0.2A

static void DCDC_Update_Normal(void);

static void DCDC_Update_MPPT(void);

static const DCDC_UpdateFunc_t mode_update_funcs[] = {
        DCDC_Update_Normal,
        DCDC_Update_MPPT
};

static void DCDC_IncDuty(void) {
    if (duty < MAX_DUTY) {
        duty += 0.75f;
        EG2104_SetDuty(duty);
    }
}

static void DCDC_DecDuty(void) {
    if (duty > 0) {
        duty -= 0.75f;
        EG2104_SetDuty(duty);
    }
}

float DCDC_GetDuty(void) {
    return duty;
}

static void DCDC_SetDuty(float value) {
    if (value <= MAX_DUTY && 0 < value) {
        duty = value;
        EG2104_SetDuty(value);
    }
}

// 普通模式更新函数 - 根据输入输出限压限流调整占空比
static void DCDC_Update_Normal(void) {
    uint16_t input_current = DCDC_GetInputCurrent();
    uint16_t output_voltage = BAT_GetVoltage();
    uint16_t output_current = BAT_GetCurrent();

    if (input_current < input_current_limit &&
        output_voltage < output_voltage_limit &&
        output_current < output_current_limit) {
        DCDC_IncDuty();
    } else {
        DCDC_DecDuty();
    }
}

// MPPT模式更新函数 - 扰动观察法
static void DCDC_Update_MPPT(void) {
    static float last_power = 0;
    static float last_duty = 0;
    float voltage, current, power;

    voltage = DCDC_GetInputVoltage() / 1000.0f;  // 转换为V
    current = DCDC_GetInputCurrent() / 1000.0f;  // 转换为A
    power = voltage * current;

    // 扰动观察法(P&O)
    if (power > last_power) {
        if (duty > last_duty) {
            DCDC_IncDuty();
        } else {
            DCDC_DecDuty();
        }
    } else {
        if (duty > last_duty) {
            DCDC_DecDuty();
        } else {
            DCDC_IncDuty();
        }
    }
    last_power = power;
    last_duty = duty;
}

// 主更新函数 - 由外部调度器调用
void DCDC_Update(void) {
    if (SOLAR_GetVoltage() > PD_GetActualVoltage()) {
        DCDC_SetMode(DCDC_MODE_MPPT);
    } else {
        DCDC_SetMode(DCDC_MODE_NORMAL);
    }

    if (current_update_func != NULL) {
        current_update_func();
    }
}

void DCDC_Init(void) {
    EG2104_Init();
    current_update_func = mode_update_funcs[current_mode];
    BAT_Init();
    SOLAR_Init();
    duty = 0.0f;
    DCDC_Enable();
}

void DCDC_Enable(void) {
    BAT_EnableOutput();
    EG2104_Enable();
}

void DCDC_Disable(void) {
    BAT_DisableOutput();
    EG2104_Disable();
}

uint16_t DCDC_GetInputVoltage(void) { // mV
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_DCDC_IN_U) * 11);
}

uint16_t DCDC_GetInputCurrent(void) { // mA
    return (uint16_t) (vcc / 4095.0 * ADC_Convert(ADC_CHANNEL_DCDC_IN_I) * 2);
}

void DCDC_SetMode(DCDC_Mode_t mode) {
    if (mode == current_mode) {
        return;
    }
    BAT_DisableOutput();
    DCDC_Disable();
    switch (current_mode) {
        case DCDC_MODE_MPPT:
            SOLAR_DisableInput();
            break;
        case DCDC_MODE_NORMAL:
            SOLAR_EnableInput();
            break;
        default:
            break;
    }
    current_mode = mode;
    current_update_func = mode_update_funcs[mode];
    switch (mode) {
        case DCDC_MODE_NORMAL:
            SOLAR_EnableInput();
            DCDC_SetDuty(0.0f);  // 普通模式初始占空比为0
            break;
        case DCDC_MODE_MPPT:
            SOLAR_EnableInput();
            DCDC_SetDuty(10.0f);  // MPPT模式初始占空比50%
            break;
    }
    DCDC_Enable();
    BAT_EnableOutput();
}

DCDC_Mode_t DCDC_GetMode(void) {
    return current_mode;
}

const char *DCDC_GetModeString(void) {
    switch (current_mode) {
        case DCDC_MODE_NORMAL:
            return "NORMAL";
        case DCDC_MODE_MPPT:
            return "MPPT";
        default:
            return "Unknown";
    }
}

void DCDC_SetInputCurrentLimit(uint16_t current_ma) {
    input_current_limit = current_ma;
}

void DCDC_SetOutputVoltageLimit(uint16_t voltage_mv) {
    output_voltage_limit = voltage_mv;
}

void DCDC_SetOutputCurrentLimit(uint16_t current_ma) {
    output_current_limit = current_ma;
}

uint16_t DCDC_GetInputCurrentLimit(void) {
    return input_current_limit;
}

uint16_t DCDC_GetOutputVoltageLimit(void) {
    return output_voltage_limit;
}

uint16_t DCDC_GetOutputCurrentLimit(void) {
    return output_current_limit;
}