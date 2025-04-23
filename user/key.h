#ifndef KEY_H
#define KEY_H


#include "n32l40x.h"
#include "stdint.h"
#include "system.h"


/* gpio define : key_up int enable */
#define KEY_UP_GPIO_PORT			GPIOB
#define KEY_UP_GPIO_PIN				GPIO_PIN_2
#define KEY_UP_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);}while(0)
#define KEY_UP_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, DISABLE);}while(0)
/* key_up int EXIT */
#define KEY_UP_EXIT_SOURCE_PORT		GPIOB_PORT_SOURCE
#define KEY_UP_EXIT_SOURCE_PIN		GPIO_PIN_SOURCE2
#define KEY_UP_EXIT_LINE			EXTI_LINE2
/* key_up int : Configure the NVIC Preemption Priority Bits */
#define KEY_UP_NVIC_IRQ_CHANNEL		EXTI2_IRQn
/* key_up gpio exti call define */
#define key_up_irq_call						exti2_irqhandler_call



#define KEY_DOWN_GPIO_PORT			GPIOD
#define KEY_DOWN_GPIO_PIN				GPIO_PIN_14
#define KEY_DOWN_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);}while(0)
#define KEY_DOWN_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, DISABLE);}while(0)
/* key_down EXIT */
#define KEY_DOWN_EXIT_SOURCE_PORT		GPIOD_PORT_SOURCE
#define KEY_DOWN_EXIT_SOURCE_PIN		GPIO_PIN_SOURCE14
#define KEY_DOWN_EXIT_LINE			EXTI_LINE14
/* key_up int : Configure the NVIC Preemption Priority Bits */
#define KEY_DOWN_NVIC_IRQ_CHANNEL		EXTI15_10_IRQn
/* key_up gpio exti call define */
#define key_down_irq_call						exti14_irqhandler_call



/* 按键类型定义 */
typedef enum {
	key_up = 0,
	key_down,
	key_set,
} key_id_def;

/* 按键事件定义 */
typedef enum {
	key_status_press,
	key_status_release,
} key_status_def;

typedef enum{
	key_event_press,
	key_event_release,
	key_event_hold_3s,
} key_event_def;


/* 按键扫描周期 */
#define KEY_SCAN_PERIOD_MS		SYSTEM_TICK_PERIOD_MS

/* 按键数量 */
#define KEY_MAX_NUM		(3)


/* sta_bits中确认按键触发的值，或者说消抖 */
#define KEY_TRIGGER_MASK			(0x3F)

/* 按键结构体 */
struct key_str {
	key_status_def status;
	uint8_t sta_bits;
	uint16_t hold_cnt;
};


void key_init(void);
uint8_t key_get_event(key_id_def *id, key_event_def *event);

#endif
