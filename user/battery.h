#ifndef BATTERY_H
#define BATTERY_H



#include "n32l40x.h"
#include "stdint.h"
#include "system.h"
#include "myadc.h"
#include "delay.h"
#include "lcd.h"
#include "sleep.h"

/* gpio define : key_up int enable */
#define CHARGE_STATE_GPIO_PORT			GPIOD
#define CHARGE_STATE_GPIO_PIN				GPIO_PIN_2
#define CHARGE_STATE_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);}while(0)
#define CHARGE_STATE_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, DISABLE);}while(0)

/* key_up int EXIT */
#define CHARGE_STATE_EXIT_SOURCE_PORT		GPIOD_PORT_SOURCE
#define CHARGE_STATE_EXIT_SOURCE_PIN		GPIO_PIN_SOURCE2
#define CHARGE_STATE_EXIT_LINE			EXTI_LINE2
/* key_up int : Configure the NVIC Preemption Priority Bits */
#define CHARGE_STATE_NVIC_IRQ_CHANNEL		EXTI2_IRQn
/* key_up gpio exti call define */

#define BATTERY_READ_GPIO_IS_CHARGING()			((GPIO_ReadInputDataBit(CHARGE_STATE_GPIO_PORT, CHARGE_STATE_GPIO_PIN) == Bit_RESET)?(1):(0))


#define BATTERY_VOLTAGE_ADC_CHANNEL		MYADC_CHAN0

#define BATTERY_VOLTAGE_LOWPOWER			(3500u)


void battery_init(void);
void task_battery_charge_flag_refresh(void);
void task_battery_voltage_refresh(void);
uint8_t battery_is_charging(void);
void task_battery_lowpower_refresh(void);



#endif

