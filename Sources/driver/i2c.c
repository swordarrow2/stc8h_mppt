//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息
// 文件名称: i2c.c
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


BOOL fI2CMasterBusy;                    //I2C主模式忙标志

////////////////////////////////////////
// I2C初始化函数
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void I2C_Init(void)
{
    I2C_SwitchP2425();                  //选择I2C数据口: SCL(P2.5), SDA(P2.4)

    I2C_MasterMode();                   //设置I2C为主机模式
    I2C_SetClockDivider(63);            //设置I2C为主机模式时钟 13:400K  63:93K

    I2C_SetIntPriority(0);              //设置中断为最低优先级
    I2C_EnableMasterInt();              //设置I2C主机模式中断
    fI2CMasterBusy = 0;                 //清除主机模式忙标志位

    I2C_Enable();                       //使能I2C功能

    //<<AICUBE_USER_I2C_INITIAL_BEGIN>>
    // 在此添加用户初始化代码
    //<<AICUBE_USER_I2C_INITIAL_END>>
}

////////////////////////////////////////
// 主机模式等待命令完成
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void I2C_MasterWait(void)
{
    while (fI2CMasterBusy);             //等待忙标志为0
}

////////////////////////////////////////
// 主机模式发送起始信号
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void I2C_MasterStart(void)
{
    I2C_Start();                        //触发主机模式起始命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成
}

////////////////////////////////////////
// 主机模式发送停止信号
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void I2C_MasterStop(void)
{
    I2C_Stop();                         //触发主机模式停止命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成
}

////////////////////////////////////////
// 主机模式发送1字节数据
// 入口参数: dat (待发送的字节数据)
// 函数返回: 0   (接收的应答信号为ACK)
//           1   (接收的应答信号为NAK)
////////////////////////////////////////
BOOL I2C_MasterSendByte(uint8_t dat)
{
    I2C_WriteData(dat);                 //将数据写入I2C数据寄存器
    I2C_SendData();                     //触发主机模式写数据命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成
    I2C_RecvACK();                      //触发主机模式接收应答命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成

    return I2C_MasterReadACK();         //读取并返回应答信号
}

////////////////////////////////////////
// 主机模式接收1字节数据
// 入口参数: ack (待发送的应答信号)
// 函数返回:     (接收的字节数据)
////////////////////////////////////////
uint8_t I2C_MasterReadByte(BOOL ack)
{
    uint8_t dat;

    I2C_RecvData();                     //触发主机模式读数据命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成
    dat = I2C_ReadData();               //读取接收的数据
    if (!ack)
        I2C_MasterSetACK();             //将ACK数据写入寄存器
    else
        I2C_MasterSetNAK();             //将NAK数据写入寄存器
    I2C_SendACK();                      //触发主机模式发送应答命令
    fI2CMasterBusy = 1;                 //设置忙标志
    I2C_MasterWait();                   //等待命令完成

    return dat;                         //返回接收的数据
}


////////////////////////////////////////
// I2C中断服务程序
// 入口参数: 无
// 函数返回: 无
////////////////////////////////////////
void I2C_ISR(void) interrupt I2C_VECTOR
{
    //<<AICUBE_USER_I2C_ISR_CODE1_BEGIN>>
    // 在此添加中断函数用户代码
    if (I2C_CheckMasterFlag())          //判断I2C主机中断
    {
        I2C_ClearMasterFlag();          //清除I2C主机中断标志
        fI2CMasterBusy = 0;             //清除主机模式忙标志位
    }
    //<<AICUBE_USER_I2C_ISR_CODE1_END>>
}


//<<AICUBE_USER_FUNCTION_IMPLEMENT_BEGIN>>
// 在此添加用户函数实现代码
//<<AICUBE_USER_FUNCTION_IMPLEMENT_END>>


