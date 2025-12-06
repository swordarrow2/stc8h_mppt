//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: i2c.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __I2C_H__
#define __I2C_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义  
//<<AICUBE_USER_DEFINE_END>>


void I2C_Init(void);
void I2C_MasterWait(void);
void I2C_MasterStart(void);
void I2C_MasterStop(void);
BOOL I2C_MasterSendByte(uint8_t dat);
uint8_t I2C_MasterReadByte(BOOL ack);

extern BOOL fI2CMasterBusy;


//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明  
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
