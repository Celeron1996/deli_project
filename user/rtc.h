#ifndef RTC_H
#define RTC_H

#include <stdio.h>
#include <stdint.h>
#include "n32l40x.h"


#define    RTC_LSE_TRY_COUNT              250


typedef enum {
    RTC_CLK_SRC_TYPE_HSE_DIV32=0x01,
    RTC_CLK_SRC_TYPE_LSE=0x02,
    RTC_CLK_SRC_TYPE_LSI=0x03,
}RTC_CLK_SRC_TYPE;


void RTC_set_date_time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_PrescalerConfig(void);
void RTC_config(void);
void RTC_Interrupt_Config(uint8_t enable);
void EXTI20_RTCWKUP_Configuration(FunctionalState Cmd);
ErrorStatus RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE Clk_Src_Type, bool Is_First_Cfg_RCC);
uint8_t RTC_get_weekday_math(int year, int month, int day);




#endif

