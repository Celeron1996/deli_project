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

#define  BKP_DAT1     ((uint16_t)0x0000)
#define  BKP_DAT2     ((uint16_t)0x0004)
#define  BKP_DAT3     ((uint16_t)0x0008)
#define  BKP_DAT4     ((uint16_t)0x000C)
#define  BKP_DAT5     ((uint16_t)0x0010)
#define  BKP_DAT6     ((uint16_t)0x0014)
#define  BKP_DAT7     ((uint16_t)0x0018)
#define  BKP_DAT8     ((uint16_t)0x001C)
#define  BKP_DAT9     ((uint16_t)0x0020)
#define  BKP_DAT10    ((uint16_t)0x0024)
#define  BKP_DAT11    ((uint16_t)0x0028)
#define  BKP_DAT12    ((uint16_t)0x002C)
#define  BKP_DAT13    ((uint16_t)0x0030)
#define  BKP_DAT14    ((uint16_t)0x0034)
#define  BKP_DAT15    ((uint16_t)0x0038)
#define  BKP_DAT16    ((uint16_t)0x003C)
#define  BKP_DAT17    ((uint16_t)0x0040)
#define  BKP_DAT18    ((uint16_t)0x0044)
#define  BKP_DAT19    ((uint16_t)0x0048)
#define  BKP_DAT20    ((uint16_t)0x004C)

#define  IS_BKP_DAT(DAT)                                                                                    \
             (((DAT) == BKP_DAT1)  || ((DAT) == BKP_DAT2)  || ((DAT) == BKP_DAT3)  || ((DAT) == BKP_DAT4)   \
           || ((DAT) == BKP_DAT5)  || ((DAT) == BKP_DAT6)  || ((DAT) == BKP_DAT7)  || ((DAT) == BKP_DAT8)   \
           || ((DAT) == BKP_DAT9)  || ((DAT) == BKP_DAT10) || ((DAT) == BKP_DAT11) || ((DAT) == BKP_DAT12)  \
           || ((DAT) == BKP_DAT13) || ((DAT) == BKP_DAT14) || ((DAT) == BKP_DAT15) || ((DAT) == BKP_DAT16)  \
           || ((DAT) == BKP_DAT17) || ((DAT) == BKP_DAT18) || ((DAT) == BKP_DAT19) || ((DAT) == BKP_DAT20) )


void RTC_set_date_time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
void RTC_PrescalerConfig(void);
void RTC_config(void);
void RTC_Interrupt_Config(uint8_t enable);
void EXTI20_RTCWKUP_Configuration(FunctionalState Cmd);
ErrorStatus RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE Clk_Src_Type, bool Is_First_Cfg_RCC);
uint8_t RTC_get_weekday_math(int year, int month, int day);
void RTC_get_date_time(uint16_t *year, uint8_t *mon, uint8_t *day, uint8_t *week, uint8_t *hour, uint8_t *min, uint8_t *sec);




#endif

