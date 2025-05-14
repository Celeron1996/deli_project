#ifndef TOUCH_H
#define TOUCH_H


#include "n32l40x.h"
#include "stdint.h"
#include "system.h"
#include "backlight.h"

/* gpio define : TOUCH int enable */
#define TOUCH_GPIO_PORT			GPIOD
#define TOUCH_GPIO_PIN				GPIO_PIN_15
#define TOUCH_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);}while(0)
#define TOUCH_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, DISABLE);}while(0)
/* TOUCH int EXIT */
#define TOUCH_EXIT_SOURCE_PORT		GPIOD_PORT_SOURCE
#define TOUCH_EXIT_SOURCE_PIN		GPIO_PIN_SOURCE15
#define TOUCH_EXIT_LINE			EXTI_LINE15
/* TOUCH int : Configure the NVIC Preemption Priority Bits */
#define TOUCH_NVIC_IRQ_CHANNEL		EXTI15_10_IRQn
/* TOUCH gpio exti call define */
#define touch_irq_call						exti15_irqhandler_call


void touch_init(void);
void task_touch(void);


#endif
