//<<AICUBE_USER_HEADER_REMARK_BEGIN>>
////////////////////////////////////////
// 在此添加用户文件头说明信息  
// 文件名称: config.h
////////////////////////////////////////
//<<AICUBE_USER_HEADER_REMARK_END>>


#ifndef __CONFIG_H__
#define __CONFIG_H__


//<<AICUBE_USER_DEFINE_BEGIN>>
// 在此添加用户宏定义
//<<AICUBE_USER_DEFINE_END>>


#define __ENCODING              "GB2312" //DO NOT DELETE or MODIFY


#define HIRC                    24000000UL
#define FOSC                    24000000UL
#define SYSCLK                  FOSC
#define MAIN_Fosc               FOSC

#include <stc8h.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <intrins.h>

#include "def.h"
#include "stc8h_def.h"
#include "ai_usb.h"


//<<AICUBE_USER_INCLUDE_BEGIN>>
// 在此添加用户头文件包含  
//<<AICUBE_USER_INCLUDE_END>>


#include "clock.h"
#include "iap.h"
#include "port.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "pwm.h"
#include "usblib.h"
#include "mathlib.h"
//#include "ssd1306.h"
#include "ch224.h"
#include "oled.h"
#include "../device/dcdc.h"
#include "../device/pd.h"
#include "../device/battery.h"
#include "../device/solar.h"

void SYS_Init(void);

void delay_us(uint16_t us);

void delay_ms(uint16_t ms);



//<<AICUBE_USER_EXTERNAL_DECLARE_BEGIN>>
// 在此添加用户外部函数和外部变量声明
//<<AICUBE_USER_EXTERNAL_DECLARE_END>>


#endif
