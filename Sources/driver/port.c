//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: port.c
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
// P0口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT0_Init(void)
{
    SetP0nInitLevelHigh(PIN_ALL);       //设置P0初始化电平
    SetP0nQuasiMode(PIN_7 | PIN_6 | PIN_5 | PIN_4); //设置P0.7,P0.6,P0.5,P0.4为准双向口模式
    SetP0nHighZInputMode(PIN_3 | PIN_2 | PIN_1 | PIN_0); //设置P0.3,P0.2,P0.1,P0.0为高阻输入模式

    DisableP0nPullUp(PIN_ALL);          //关闭P0内部上拉电阻
    DisableP0nSchmitt(PIN_ALL);         //使能P0施密特触发
    SetP0nSlewRateNormal(PIN_ALL);      //设置P0一般翻转速度
    SetP0nDrivingNormal(PIN_ALL);       //设置P0一般驱动能力
    SetP0nDigitalInput(PIN_ALL);        //使能P0数字信号输入功能

    //<<AICUBE_USER_PORT0_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT0_INITIAL_END>>
}

////////////////////////////////////////
// P1口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT1_Init(void)
{
    SetP1nInitLevelHigh(PIN_7 | PIN_6 | PIN_3 | PIN_2 | PIN_1 | PIN_0); //设置P1.7,P1.6,P1.3,P1.2,P1.1,P1.0初始化电平
    SetP1nInitLevelLow(PIN_5 | PIN_4);  //设置P1.5,P1.4初始化电平
    SetP1nQuasiMode(PIN_4 | PIN_2 | PIN_1 | PIN_0); //设置P1.4,P1.2,P1.1,P1.0为准双向口模式
    SetP1nPushPullMode(PIN_5);          //设置P1.5为推挽输出模式
    SetP1nHighZInputMode(PIN_7 | PIN_6 | PIN_3); //设置P1.7,P1.6,P1.3为高阻输入模式

    EnableP1nPullUp(PIN_5 | PIN_4);     //使能P1.5,P1.4内部上拉电阻
    DisableP1nPullUp(PIN_7 | PIN_6 | PIN_3 | PIN_2 | PIN_1 | PIN_0); //关闭P1.7,P1.6,P1.3,P1.2,P1.1,P1.0内部上拉电阻
    DisableP1nSchmitt(PIN_ALL);         //使能P1施密特触发
    SetP1nSlewRateNormal(PIN_ALL);      //设置P1一般翻转速度
    SetP1nDrivingNormal(PIN_ALL);       //设置P1一般驱动能力
    SetP1nDigitalInput(PIN_ALL);        //使能P1数字信号输入功能

    //<<AICUBE_USER_PORT1_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT1_INITIAL_END>>
}

////////////////////////////////////////
// P2口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT2_Init(void)
{
    SetP2nInitLevelHigh(PIN_5 | PIN_4 | PIN_2 | PIN_1 | PIN_0); //设置P2.5,P2.4,P2.2,P2.1,P2.0初始化电平
    SetP2nInitLevelLow(PIN_7 | PIN_6 | PIN_3); //设置P2.7,P2.6,P2.3初始化电平
    SetP2nQuasiMode(PIN_5 | PIN_4 | PIN_2 | PIN_1 | PIN_0); //设置P2.5,P2.4,P2.2,P2.1,P2.0为准双向口模式
    SetP2nPushPullMode(PIN_7 | PIN_6 | PIN_3); //设置P2.7,P2.6,P2.3为推挽输出模式

    EnableP2nPullUp(PIN_5 | PIN_4);     //使能P2.5,P2.4内部上拉电阻
    DisableP2nPullUp(PIN_7 | PIN_6 | PIN_3 | PIN_2 | PIN_1 | PIN_0); //关闭P2.7,P2.6,P2.3,P2.2,P2.1,P2.0内部上拉电阻
    DisableP2nSchmitt(PIN_ALL);         //使能P2施密特触发
    SetP2nSlewRateNormal(PIN_ALL);      //设置P2一般翻转速度
    SetP2nDrivingNormal(PIN_ALL);       //设置P2一般驱动能力
    SetP2nDigitalInput(PIN_ALL);        //使能P2数字信号输入功能

    //<<AICUBE_USER_PORT2_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT2_INITIAL_END>>
}

////////////////////////////////////////
// P3口初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void PORT3_Init(void)
{
    SetP3nInitLevelHigh(PIN_ALL);       //设置P3初始化电平
    SetP3nQuasiMode(PIN_3 | PIN_2);     //设置P3.3,P3.2为准双向口模式
    SetP3nPushPullMode(PIN_7 | PIN_6 | PIN_5 | PIN_4); //设置P3.7,P3.6,P3.5,P3.4为推挽输出模式
    SetP3nHighZInputMode(PIN_1 | PIN_0); //设置P3.1,P3.0为高阻输入模式

    DisableP3nPullUp(PIN_ALL);          //关闭P3内部上拉电阻
    DisableP3nSchmitt(PIN_ALL);         //使能P3施密特触发
    SetP3nSlewRateNormal(PIN_ALL);      //设置P3一般翻转速度
    SetP3nDrivingNormal(PIN_ALL);       //设置P3一般驱动能力
    SetP3nDigitalInput(PIN_ALL);        //使能P3数字信号输入功能

    //<<AICUBE_USER_PORT3_INITIAL_BEGIN>>
    // 在此添加用户初始化代码  
    //<<AICUBE_USER_PORT3_INITIAL_END>>
}



//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码  
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


