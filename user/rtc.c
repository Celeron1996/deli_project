#include "rtc.h"
#include "system.h"
#include "delay.h"


uint32_t SynchPrediv, AsynchPrediv;
RTC_InitType  RTC_InitStructure;

/*
#define IS_RTC_MONTH(MONTH) (((MONTH) >= 1) && ((MONTH) <= 12))
#define IS_RTC_DATE(DATE)   (((DATE) >= 1) && ((DATE) <= 31))
*/
void RTC_set_date_time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
	RTC_DateType  RTC_DateStructure;
	RTC_TimeType  RTC_TimeStructure;

	RTC_DateStructure.Year = year - 2000;
	RTC_DateStructure.Month = mon;
	RTC_DateStructure.Date = day;
	RTC_DateStructure.WeekDay = RTC_get_weekday_math(year, mon, day);

	RTC_TimeStructure.Hours = hour;
	RTC_TimeStructure.Minutes = min;
	RTC_TimeStructure.Seconds = sec;

	if (RTC_SetDate(RTC_FORMAT_BIN, &RTC_DateStructure) == ERROR)
	{
		printf("\r\n set rtc date error!\r\n");
		while (1);
	}

	if (RTC_ConfigTime(RTC_FORMAT_BIN, &RTC_TimeStructure) == ERROR)
	{
		printf("\r\n set rtc time error!\r\n");
		while (1);
	}
}

/*
#define IS_RTC_MONTH(MONTH) (((MONTH) >= 1) && ((MONTH) <= 12))
#define IS_RTC_DATE(DATE)   (((DATE) >= 1) && ((DATE) <= 31))
#define RTC_WEEKDAY_MONDAY    ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY   ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY  ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY    ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY  ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY    ((uint8_t)0x07)
*/
void RTC_get_date_time(uint16_t *year, uint8_t *mon, uint8_t *day, uint8_t *week, uint8_t *hour, uint8_t *min, uint8_t *sec)
{
	RTC_DateType RTC_DateStruct;
	RTC_TimeType RTC_TimeStruct;

	RTC_GetDate(RTC_FORMAT_BIN, &RTC_DateStruct);
	RTC_GetTime(RTC_FORMAT_BIN, &RTC_TimeStruct);

	*year = RTC_DateStruct.Year + 2000;
	*mon = RTC_DateStruct.Month;
	*day = RTC_DateStruct.Date;
	*week = RTC_DateStruct.WeekDay;

	*hour = RTC_TimeStruct.Hours;
	*min = RTC_TimeStruct.Minutes;
	*sec = RTC_TimeStruct.Seconds;
}


/**
 * @brief  RTC prescaler config.
 */
void RTC_PrescalerConfig(void)
{
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv  = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat   = RTC_24HOUR_FORMAT;
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
        printf("\r\n //******* RTC Prescaler Config failed **********// \r\n");
    }
}


void RTC_config(void)
{
	RTC_EnableWriteProtection(DISABLE);  
	
	/* Enable the PWR clock */
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
	
  /* Allow access to RTC */
  PWR_BackupAccessEnable(ENABLE);

  /* RTC clock source select */
  if(SUCCESS==RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE_LSE, true))
  {
		RTC_PrescalerConfig();
		/* Adjust time by values entered by the user on the hyperterminal */
		RTC_set_date_time(2025, 4, 26, 0, 0, 0);
		/* wake up clock select */
		RTC_ConfigWakeUpClock(RTC_WKUPCLK_CK_SPRE_16BITS);
		/* wake up timer value */
		RTC_SetWakeUpCounter(4);
		printf("\r\n RTC Init Success\r\n");
  }
  else
  {
      printf("\r\n RTC Init Faile\r\n");
      while(1);
  }
  RTC_Interrupt_Config(ENABLE);
}


void RTC_Interrupt_Config(uint8_t enable)
{
	if (enable)
	{
		EXTI_ClrITPendBit(EXTI_LINE20);
		EXTI20_RTCWKUP_Configuration(ENABLE);
		/* Enable the RTC Wakeup Interrupt */
		RTC_ClrIntPendingBit(RTC_INT_WUT);
		RTC_ConfigInt(RTC_INT_WUT, ENABLE);
		RTC_EnableWakeUp(ENABLE);
		DBG_ConfigPeriph(DBG_STOP, ENABLE);
	}
	else
	{
		DBG_ConfigPeriph(DBG_STOP, DISABLE);
		RTC_EnableWakeUp(DISABLE);
		RTC_ConfigInt(RTC_INT_WUT, DISABLE);
		RTC_ClrIntPendingBit(RTC_INT_WUT);
		EXTI20_RTCWKUP_Configuration(DISABLE);
		EXTI_ClrITPendBit(EXTI_LINE20);
	}
}


/**
 * @brief  Config RTC wake up Interrupt.
 */
void EXTI20_RTCWKUP_Configuration(FunctionalState Cmd)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    EXTI_ClrITPendBit(EXTI_LINE20);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);
    /* Enable the RTC WakeUp Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = Cmd;
    NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  Configures the RTC Source Clock Type.
 * @param Clk_Src_Type specifies RTC Source Clock Type.
 *   This parameter can be on of the following values:
 *     @arg RTC_CLK_SRC_TYPE_HSE128
 *     @arg RTC_CLK_SRC_TYPE_LSE
 *     @arg RTC_CLK_SRC_TYPE_LSI
 * @param Is_First_Cfg_RCC specifies Is First Config RCC Module.
 *   This parameter can be on of the following values:
 *     @arg true
 *     @arg false
 * @return An ErrorStatus enumeration value:
 *          - SUCCESS: RTC clock configure success
 *          - ERROR: RTC clock configure failed
 */
ErrorStatus RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE Clk_Src_Type, bool Is_First_Cfg_RCC)
{
    uint8_t lse_ready_count=0;
    ErrorStatus Status=SUCCESS;
    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessEnable(ENABLE);
    /* Disable RTC clock */
    RCC_EnableRtcClk(DISABLE);
    if (RTC_CLK_SRC_TYPE_HSE_DIV32 == Clk_Src_Type)
    {
       printf("\r\n RTC_ClkSrc Is Set HSE/32! \r\n");
       if (true == Is_First_Cfg_RCC )
       {
          /* Enable HSE */
          RCC_EnableLsi(DISABLE);
          RCC_ConfigHse(RCC_HSE_ENABLE);
          while (RCC_WaitHseStable() == ERROR)
          {
          }
          RCC_ConfigRtcClk(RCC_RTCCLK_SRC_HSE_DIV32);
       }
       else
       {
          RCC_EnableLsi(DISABLE);
          RCC_ConfigRtcClk(RCC_RTCCLK_SRC_HSE_DIV32);
          /* Enable HSE */
          RCC_ConfigHse(RCC_HSE_ENABLE);
          while (RCC_WaitHseStable() == ERROR)
          {
          }
       }
       SynchPrediv  = 0x7A0; // 8M/32 = 250KHz
       AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else if (RTC_CLK_SRC_TYPE_LSE == Clk_Src_Type)
    {
       printf("\r\n RTC_ClkSrc Is Set LSE! \r\n");
       if (true == Is_First_Cfg_RCC)
       {
          /* Enable the LSE OSC32_IN PC14 */
					RCC_EnableLsi(DISABLE); // LSI is turned off here to ensure that only one clock is turned on
					RCC_ConfigLse(RCC_LSE_ENABLE,0x1FF);

         lse_ready_count=0;
         /****Waite LSE Ready *****/
         while((RCC_GetFlagStatus(RCC_LDCTRL_FLAG_LSERD) == RESET) && (lse_ready_count<RTC_LSE_TRY_COUNT))
         {
             lse_ready_count++;
             delay_ms(10);
             /****LSE Ready failed or timeout*****/
             if(lse_ready_count>=RTC_LSE_TRY_COUNT)
             {
                Status = ERROR;
                printf("\r\n RTC_ClkSrc Set LSE Faile! \r\n");
                break;
             }
         }
         RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSE);
       }
       else
       {
          /* Enable the LSE OSC32_IN PC14 */
          RCC_EnableLsi(DISABLE);
          RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSE);
          RCC_ConfigLse(RCC_LSE_ENABLE,0x1FF);

          lse_ready_count=0;
          /****Waite LSE Ready *****/
          while((RCC_GetFlagStatus(RCC_LDCTRL_FLAG_LSERD) == RESET) && (lse_ready_count<RTC_LSE_TRY_COUNT))
          {
              lse_ready_count++;
              delay_ms(10);
              /****LSE Ready failed or timeout*****/
              if(lse_ready_count>=RTC_LSE_TRY_COUNT)
              {
                 Status = ERROR;
                 printf("\r\n RTC_ClkSrc Set LSE Faile! \r\n");
                 break;
              }
          }
       }
       SynchPrediv  = 0xFF; // 32.768KHz
       AsynchPrediv = 0x7F; // value range: 0-7F
    }
    else if (RTC_CLK_SRC_TYPE_LSI == Clk_Src_Type)
    {
       printf("\r\n RTC_ClkSrc Is Set LSI! \r\n");
       if (true == Is_First_Cfg_RCC)
       {
          /* Enable the LSI OSC */
          RCC_EnableLsi(ENABLE);
          while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET)
          {
          }
          RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
       }
       else
       {
          RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
          /* Enable the LSI OSC */
          RCC_EnableLsi(ENABLE);
          while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET)
          {
          }
       }
       SynchPrediv  = 0x14A; // 41828Hz
       AsynchPrediv = 0x7F;  // value range: 0-7F
    }
    else
    {
       printf("\r\n RTC_ClkSrc Value is error! \r\n");
    }
    /* Enable the RTC Clock */
    RCC_EnableRtcClk(ENABLE);
    RTC_WaitForSynchro();
    return Status;
}









// 示例：get_weekday_math(2000, 1, 1)      2000年1月1日 -> Saturday 
/*
#define RTC_WEEKDAY_MONDAY    ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY   ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY  ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY    ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY  ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY    ((uint8_t)0x07)
*/
uint8_t RTC_get_weekday_math(int year, int month, int day)
{
	enum{
	Saturday = 0,
	Sunday,
	Monday, 
	Tuesday,
	Wednesday, 
	Thursday, 
	Friday
	} week_;

	uint8_t n32l4_week_def[7] = {RTC_WEEKDAY_SATURDAY,
																RTC_WEEKDAY_SUNDAY,
																RTC_WEEKDAY_MONDAY,
																RTC_WEEKDAY_TUESDAY,
																RTC_WEEKDAY_WEDNESDAY,
																RTC_WEEKDAY_THURSDAY,
																RTC_WEEKDAY_FRIDAY};
	
	// Zeller公式：0=Saturday, 1=Sunday...6=Friday
	if (month < 3) {
	    month += 12;
	    year--;
	}
	
	int q = day;
	int m = month;
	int K = year % 100;
	int J = year / 100;
	int h = (q + 13*(m + 1)/5 + K + K/4 + J/4 + 5*J) % 7;

	return n32l4_week_def[h];
}

