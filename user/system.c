#include "system.h"


/* 单位是毫秒 */
volatile tick_type system_tick_counter_ms = 0;

/* tick timer 运行标志 */
uint8_t system_tick_timer_run_flag = 0;

/* 使用基本定时器6进行tick计数，需要产生5ms中断的tick
	 APB1_PERIPH 过来给到定时器的频率最大16MHZ
	 根据数据手册，APB1 的分频系数不为1时，给到TIM6时频率是两倍
	 即 TIM6 最大时钟频率是32MHZ
	 综上：预分频系数设为32，即1微秒记一次
	 然后 period周期值为 5000，所以为5ms中断一次
*/
TIM_TimeBaseInitType TIM_TimeBaseStructure;
void system_tick_init(void)
{
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);

	NVIC_InitType NVIC_InitStructure;

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.Period    = 5000;
	TIM_TimeBaseStructure.Prescaler = 0;
	TIM_TimeBaseStructure.ClkDiv    = 0;
	TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

	TIM_InitTimeBase(TIM6, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	TIM_ConfigPrescaler(TIM6, 32, TIM_PSC_RELOAD_MODE_IMMEDIATE);

	/* TIM6 enable update irq */
	TIM_ConfigInt(TIM6, TIM_INT_UPDATE, ENABLE);

	/* TIM6 enable counter */
	TIM_Enable(TIM6, ENABLE);

	system_tick_timer_run_flag = 1;
}


/* 关闭tick定时器 */
void system_tick_deinit(void)
{
	NVIC_InitType NVIC_InitStructure;
	
	TIM_Enable(TIM6, DISABLE);

	TIM_ConfigInt(TIM6, TIM_INT_UPDATE, DISABLE);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, DISABLE);

	system_tick_timer_run_flag = 0;
}



