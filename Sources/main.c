//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: main.c
// 文件描述: 
// 文件版本: V1.0
// 修改记录:
//   1. (2025-12-03) 创建文件
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#include "config.h"                     //默认已包含stdio.h、intrins.h等头文件
#include "./device/screen.h"
#include "./device/logic.h"

//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


//<<AICUBE_USER_GLOBAL_DEFINE_BEGIN>>
// 在此添加用户全局变量定义、用户宏定义以及函数声明  
//<<AICUBE_USER_GLOBAL_DEFINE_END>>

////////////////////////////////////////
// 项目主函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void main(void) {
//    uint8_t status;
//    uint16_t current_ma;
//    uint8_t ret;
//    char str_buffer[12];

    SYS_Init();

    // 显示内容
    SCREEN_Init();

//    status = CH224_ReadStatus();// 检查PD协议是否激活
//    if (status & CH224_STATUS_PD_ACTIVE) {
//        ret = CH224_SetVoltage(CH224_VOLTAGE_9V);
//        if (ret == 0) {
//            // 可以读取当前最大电流
//            ret = CH224_ReadCurrentValue(&current_ma);
//            if (ret == 0) {
//                // current_ma 现在包含最大电流值(单位: mA)
//                OLED_PutNumber(42, 10, current_ma);
//            }
//        }
//    }
//    result = CH224_EnablePPS(7700);
    while (1) {
        SCREEN_Update();
//        current_ma = PD_GetMaxAvailableCurrent();  // 可以读取当前最大电流
//
//        OLED_UI_SetRegionLabel(REGION_TOP_LEFT, PD_GetProtocolType());
//
//        sprintf(str_buffer, "%d", current_ma);

//        OLED_UI_SetRegionValue(REGION_MIDDLE_LEFT, str_buffer);
//        OLED_UI_SetRegionLabel(REGION_MIDDLE_LEFT, "PD MAX");
//        OLED_UI_UpdateDisplay();
        //<<AICUBE_USER_MAIN_LOOP_BEGIN>>
        // 在此添加主函数中用户主循环代码  
        //<<AICUBE_USER_MAIN_LOOP_END>>
        if (ADC_Convert(11) > 600) {
            P27 = 1;
            P15 = 1;
            PWM_UpdateDuty(PWMA_CH3, 50.000f, PWM_MODE1);
        } else {
            P27 = 0;
            P15 = 0;
            PWM_UpdateDuty(PWMA_CH3, 0.0f, PWM_MODE1);
        }
    }
}

////////////////////////////////////////
// 系统初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void SYS_Init(void) {
    EnableAccessXFR();                  //使能访问扩展XFR
    IAP_SetTimeBase();                  //设置IAP等待参数,产生1us时基

    P0M0 = 0x00;
    P0M1 = 0x00;           //初始化P0口为准双向口模式
    P1M0 = 0x00;
    P1M1 = 0x00;           //初始化P1口为准双向口模式
    P2M0 = 0x00;
    P2M1 = 0x00;           //初始化P2口为准双向口模式
    P3M0 = 0x00;
    P3M1 = 0x00;           //初始化P3口为准双向口模式
    P4M0 = 0x00;

    CLK_Init();                         //时钟模块初始化
    IAP_Init();                         //EEPROM初始化
    PORT0_Init();                       //P0口初始化
    PORT1_Init();                       //P1口初始化
    PORT2_Init();                       //P2口初始化
    PORT3_Init();                       //P3口初始化
    TIMER0_Init();                      //定时器0初始化
    TIMER2_Init();                      //定时器2初始化
    UART2_Init();                       //串口2初始化
    ADC_Init();                         //ADC初始化
    I2C_Init();                         //I2C初始化
    RTC_Init();                         //RTC初始化
    PWMA_Init();                        //高级PWMA初始化
    PWMB_Init();                        //高级PWMB初始化
    delay_ms(1);
    USBLIB_Init();                      //USB库初始化
    delay_ms(1);
    MATHLIB_Init();                     //MATH库初始化

    //<<AICUBE_USER_INITIAL_CODE_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_INITIAL_CODE_END>>

    EnableGlobalInt();                  //使能全局中断
}

////////////////////////////////////////
// 微秒延时函数
// 入口参数: us (设置延时的微秒值)
// 函数返回: 无
////////////////////////////////////////
void delay_us(uint16_t us) {
    do {
        NOP(14);                        //(MAIN_Fosc + 500000) / 1000000 - 10
    } while (--us);
}


////////////////////////////////////////
// 毫秒延时函数
// 入口参数: ms (设置延时的毫秒值)
// 函数返回: 无
////////////////////////////////////////
void delay_ms(uint16_t ms) {
    uint16_t i;

    do {
        i = MAIN_Fosc / 10000;
        while (--i);
    } while (--ms);
}


//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


