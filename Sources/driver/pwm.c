//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: pwm.c
// 文件描述: 
// 文件版本: V1.0
// 修改记录:
//   1. (2025-12-03) 创建文件
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#include "../inc/config.h"

#define PWMA_BRK PWMA_BKR
#define PWMB_BRK PWMB_BKR

//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


//<<AICUBE_USER_GLOBAL_DEFINE_BEGIN>>
// 在此添加用户全局变量定义、用户宏定义以及函数声明  
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// PWMA初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PWMA_Init(void)
{
    PWMA_C3SwitchP1415();               //设置PWMA通道3数据端口: PWM3P (P1.4), PWM3N (P1.5)

    PWMA_InternalClockMode();           //选择内部时钟模式
    PWMA_SetClockDivider(0);            //设置16位预分频
    PWMA_SetReload16(249);              //设置16位重载值
    PWMA_BufferARR();                   //使能重载值寄存器预装载功能
    PWMA_SetCounter(0);                 //初始化计数值

    PWMA_CC3PDisable();                 //关闭通道
    PWMA_CC3Output();                   //使能通道输出功能
    PWMA_CC3PEnable();                  //打开通道
    PWM_UpdateDuty(PWMA_CH3, 50.000f, PWM_MODE1); //设置通道的PWM占空比
    PWMA_BufferCCR3();                  //使能CCRn预装载功能
    PWMA_CC3PNonInverted();             //设置正极通道输出高电平有效
    PWMA_EnablePWM3POutput();           //使能正极通道输出
    PWMA_DisablePWM3NOutput();          //关闭负极通道输出

    PWMA_EnableMainOutput();            //使能PWM主输出

    PWMA_Run();                         //PWMA开始运行

    //<<AICUBE_USER_PWM0_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PWM0_INITIAL_END>>
}

////////////////////////////////////////
// PWMB初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PWMB_Init(void)
{
    PWMB_C5SwitchP20();                 //设置PWMB通道5数据端口: PWM5 (P2.0)

    PWMB_InternalClockMode();           //选择内部时钟模式
    PWMB_SetClockDivider(3);            //设置16位预分频
    PWMB_SetReload16(999);              //设置16位重载值
    PWMB_BufferARR();                   //使能重载值寄存器预装载功能
    PWMB_SetCounter(0);                 //初始化计数值

    PWMB_CC5PDisable();                 //关闭通道
    PWMB_CC5Output();                   //使能通道输出功能
    PWMB_CC5PEnable();                  //打开通道
    PWM_UpdateDuty(PWMB_CH5, 10.000f, PWM_MODE1); //设置通道的PWM占空比
    PWMB_BufferCCR5();                  //使能CCRn预装载功能
    PWMB_CC5PNonInverted();             //设置正极通道输出高电平有效
    PWMB_EnablePWM5POutput();           //使能正极通道输出

    PWMB_EnableMainOutput();            //使能PWM主输出

    PWMB_Run();                         //PWMB开始运行

    //<<AICUBE_USER_PWM1_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PWM1_INITIAL_END>>
}

////////////////////////////////////////
// 设置PWM通道输出占空比
// 入口参数: PWMx: (目标PWM组和通道索引)
//           fDuty: (占空比值/百分比值,范围：0.0f~100.0f)
//           iMode: (PWM输出模式：PWM_MODE1/PWM_MODE2)
// 函数返回: 无
////////////////////////////////////////
void PWM_UpdateDuty(uint8_t PWMx, float fDuty, uint8_t iMode)
{
    if (fDuty <= (0.0f + FLT_EPSILON))
    {
        //设置输出模式为强制无效电平（0%占空比）
        switch (PWMx)
        {
        case PWMA_CH1:  PWMA_OC1REFForceLow();  break;
        case PWMA_CH2:  PWMA_OC2REFForceLow();  break;
        case PWMA_CH3:  PWMA_OC3REFForceLow();  break;
        case PWMA_CH4:  PWMA_OC4REFForceLow();  break;
        case PWMB_CH5:  PWMB_OC5REFForceLow();  break;
        case PWMB_CH6:  PWMB_OC6REFForceLow();  break;
        case PWMB_CH7:  PWMB_OC7REFForceLow();  break;
        case PWMB_CH8:  PWMB_OC8REFForceLow();  break;
        }
    }
    else if (fDuty >= (100.0f - FLT_EPSILON))
    {
        //设置输出模式为强制有效电平（100%占空比）
        switch (PWMx)
        {
        case PWMA_CH1:  PWMA_OC1REFForceHigh(); break;
        case PWMA_CH2:  PWMA_OC2REFForceHigh(); break;
        case PWMA_CH3:  PWMA_OC3REFForceHigh(); break;
        case PWMA_CH4:  PWMA_OC4REFForceHigh(); break;
        case PWMB_CH5:  PWMB_OC5REFForceHigh(); break;
        case PWMB_CH6:  PWMB_OC6REFForceHigh(); break;
        case PWMB_CH7:  PWMB_OC7REFForceHigh(); break;
        case PWMB_CH8:  PWMB_OC8REFForceHigh(); break;
        }
    }
    else
    {
        uint16_t arr;
        uint16_t val;

        //获取周期值
        if (IS_PWMA(PWMx))
            arr = PWMA_ReadReload16();
        else if (IS_PWMB(PWMx))
            arr = PWMB_ReadReload16();
        else
            arr = 0;

        //根据周期和占空比计算比较值
        val = (arr ? ((arr + 1) * fDuty / 100.0f) : (0));

        switch (PWMx)
        {
        case PWMA_CH1:
            PWMA_SetCCR1Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMA_OC1REFPWMMode1();  //设置输出PWM模式1
            else
                PWMA_OC1REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMA_CH2:
            PWMA_SetCCR2Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMA_OC2REFPWMMode1();  //设置输出PWM模式1
            else
                PWMA_OC2REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMA_CH3:
            PWMA_SetCCR3Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMA_OC3REFPWMMode1();  //设置输出PWM模式1
            else
                PWMA_OC3REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMA_CH4:
            PWMA_SetCCR4Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMA_OC4REFPWMMode1();  //设置输出PWM模式1
            else
                PWMA_OC4REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMB_CH5:
            PWMB_SetCCR5Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMB_OC5REFPWMMode1();  //设置输出PWM模式1
            else
                PWMB_OC5REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMB_CH6:
            PWMB_SetCCR6Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMB_OC6REFPWMMode1();  //设置输出PWM模式1
            else
                PWMB_OC6REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMB_CH7:
            PWMB_SetCCR7Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMB_OC7REFPWMMode1();  //设置输出PWM模式1
            else
                PWMB_OC7REFPWMMode2();  //设置输出PWM模式2
            break;
        case PWMB_CH8:
            PWMB_SetCCR8Value(val);     //设置通道比较值
            if (iMode == PWM_MODE1)
                PWMB_OC8REFPWMMode1();  //设置输出PWM模式1
            else
                PWMB_OC8REFPWMMode2();  //设置输出PWM模式2
            break;
        }
    }
}

////////////////////////////////////////
// 读取PWM通道捕获值
// 入口参数: PWMx: (目标PWM组和通道索引)
// 函数返回: 捕获值
////////////////////////////////////////
uint16_t PWM_ReadCapture(uint8_t PWMx)
{
    uint16_t cap;

    switch (PWMx)
    {
    case PWMA_CH1:  cap = PWMA_ReadCCR1Value(); break;
    case PWMA_CH2:  cap = PWMA_ReadCCR2Value(); break;
    case PWMA_CH3:  cap = PWMA_ReadCCR3Value(); break;
    case PWMA_CH4:  cap = PWMA_ReadCCR4Value(); break;
    case PWMB_CH5:  cap = PWMB_ReadCCR5Value(); break;
    case PWMB_CH6:  cap = PWMB_ReadCCR6Value(); break;
    case PWMB_CH7:  cap = PWMB_ReadCCR7Value(); break;
    case PWMB_CH8:  cap = PWMB_ReadCCR8Value(); break;
    default: cap = 0;
    }

    return cap;
}



//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


