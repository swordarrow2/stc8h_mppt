//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: timer.c
// 文件描述: 
// 文件版本: V1.0
// 修改记录:
//   1. (2025-12-03) 创建文件
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>

#include "../inc/config.h"

extern bit screen_dirty;
extern bit logic_dirty;
extern uint16_t main_loop_time;

////////////////////////////////////////
// 定时器0初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER0_Init(void) {
#define T0_PSCR                 (0)
#define T0_RELOAD               (65536 - (float)SYSCLK / 12 / (T0_PSCR + 1) * 10 / 1000)

    TIMER0_TimerMode();                 //设置定时器0为定时模式
    TIMER0_12TMode();                   //设置定时器0为12T模式
    TIMER0_Mode0();                     //设置定时器0为模式0 (16位自动重载模式)
    TIMER0_DisableGateINT0();           //禁止定时器0门控
    TIMER0_SetIntPriority(0);           //设置中断为最低优先级
    TIMER0_EnableInt();                 //使能定时器0中断
    TIMER0_SetPrescale(T0_PSCR);        //设置定时器0的8位预分频
    TIMER0_SetReload16(T0_RELOAD);      //设置定时器0的16位重载值
    TIMER0_Run();                       //定时器0开始运行
}

////////////////////////////////////////
// 定时器1初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER1_Init(void)
{
#define T1_PSCR                 (0)
#define T1_RELOAD               (65536 - (float)SYSCLK / (T1_PSCR + 1) * 1000 / 1000000)

    TIMER1_TimerMode();                 //设置定时器1为定时模式
    TIMER1_1TMode();                    //设置定时器1为1T模式
    TIMER1_Mode0();                     //设置定时器1为模式0 (16位自动重载模式)
    TIMER1_DisableGateINT1();           //禁止定时器1门控
    TIMER1_SetIntPriority(0);           //设置中断为最低优先级
    TIMER1_EnableInt();                 //使能定时器1中断
    TIMER1_SetPrescale(T1_PSCR);        //设置定时器1的8位预分频
    TIMER1_SetReload16(T1_RELOAD);      //设置定时器1的16位重载值
    TIMER1_Run();                       //定时器1开始运行

    //<<AICUBE_USER_TIMER1_INITIAL_BEGIN>>
    // 在此添加用户初始化代码
    //<<AICUBE_USER_TIMER1_INITIAL_END>>
}


////////////////////////////////////////
// 定时器1中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER1_ISR(void) interrupt TMR1_VECTOR
{//1KHz = 1ms
    ++main_loop_time;
}

////////////////////////////////////////
// 定时器2初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER2_Init(void) {
#define T2_PSCR                 (0)
#ifdef BAUDRATE
#undef BAUDRATE
#endif
#define BAUDRATE                (115200)
#define T2_RELOAD               (65536 - (SYSCLK / BAUDRATE + 2) / 4)

    TIMER2_TimerMode();                 //设置定时器2为定时模式
    TIMER2_1TMode();                    //设置定时器2为1T模式
    TIMER2_SetPrescale(T2_PSCR);        //设置定时器2的8位预分频
    TIMER2_SetReload16(T2_RELOAD);      //设置定时器2的16位重载值
    TIMER2_Run();                       //定时器2开始运行
}

////////////////////////////////////////
// 定时器0中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void TIMER0_ISR(void) interrupt TMR0_VECTOR
{ //100Hz
    static uint8_t flag = 0;
    logic_dirty = 1;
    ++flag;
    if (flag == 100) {
        screen_dirty = 1;
        flag = 0;
    }
}
