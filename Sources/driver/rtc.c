//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: rtc.c
// 文件描述: 
// 文件版本: V1.0
// 修改记录:
//   1. (2025-12-03) 创建文件
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#include "../inc/config.h"


//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


//<<AICUBE_USER_GLOBAL_DEFINE_BEGIN>>
// 在此添加用户全局变量定义、用户宏定义以及函数声明  
//<<AICUBE_USER_GLOBAL_DEFINE_END>>



////////////////////////////////////////
// RTC初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void RTC_Init(void)
{
    RTC_CLK_LIRC();                     //RTC使用内部低速LIRC当作时钟源

    RTC_SetYear(25);                    //设置RTC的年
    RTC_SetMonth(12);                   //设置RTC的月
    RTC_SetDay(3);                      //设置RTC的日
    RTC_SetHour(8);                     //设置RTC的小时
    RTC_SetMinute(20);                  //设置RTC的分钟
    RTC_SetSecond(19);                  //设置RTC的秒
    RTC_SetSSecond(0);                  //初始化RTC的次秒

    RTC_Run();                          //启动RTC
    RTC_SyncInitial();                  //同步RTC的初始值
    while (RTC_IsSyncing());            //等待同步完成

    RTC_SetIntPriority(0);              //设置中断为最低优先级
    RTC_EnableSecondInt();              //使能RTC秒中断

    //<<AICUBE_USER_RTC_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_RTC_INITIAL_END>>
}


////////////////////////////////////////
// RTC中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void RTC_ISR(void) interrupt RTC_VECTOR
{
    //<<AICUBE_USER_RTC_ISR_CODE1_BEGIN>>
    // 在此添加中断函数用户代码  
    if (RTC_CheckSecondFlag())          //判断是否是RTC秒中断
    {
        RTC_ClearSecondFlag();          //清除中断请求标志位
    }
    //<<AICUBE_USER_RTC_ISR_CODE1_END>>
}


//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


