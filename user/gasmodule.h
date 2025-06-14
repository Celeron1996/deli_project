#ifndef GASMODULE_H
#define GASMODULE_H


#include "n32l40x.h"
#include "stdint.h"
#include "system.h"
#include "delay.h"
#include "lcd.h"
#include "sleep.h"

#define GASMODULE_GPIO_PORT			GPIOA
#define GASMODULE_GPIO_PIN				GPIO_PIN_12
#define GASMODULE_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);}while(0)
#define GASMODULE_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, DISABLE);}while(0)



#define gasmodule_on()	do{GPIO_WriteBit(GASMODULE_GPIO_PORT, GASMODULE_GPIO_PIN, Bit_RESET);}while(0)
#define gasmodule_off()	do{GPIO_WriteBit(GASMODULE_GPIO_PORT, GASMODULE_GPIO_PIN, Bit_SET);}while(0)
#define gasmodule_ctr(cmd)	do{\
if (cmd){gasmodule_on();}\
else{gasmodule_off();}\
}while(0)





/* gpio define : uart rx */
#define GASMODULE_RX_GPIO_PORT					GPIOA
#define GASMODULE_RX_GPIO_PIN					GPIO_PIN_5
#define GASMODULE_RX_GPIO_CLK_ENABLE()			do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);}while(0)
#define GASMODULE_RX_GPIO_CLK_DISABLE()			do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, DISABLE);}while(0)
#define GASMODULE_RX_GPIO_AF						GPIO_AF4_USART1

/* gpio define : uart tx */
#define GASMODULE_TX_GPIO_PORT					GPIOA
#define GASMODULE_TX_GPIO_PIN					GPIO_PIN_4
#define GASMODULE_TX_GPIO_CLK_ENABLE()			do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);}while(0)
#define GASMODULE_TX_GPIO_CLK_DISABLE()			do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, DISABLE);}while(0)
#define GASMODULE_TX_GPIO_AF						GPIO_AF1_USART1

/* uart config define */
#define GASMODULE_UART							USART1
#define GASMODULE_UART_BAUDRATE					9600u
#define GASMODULE_UART_CLK_ENABLE()				do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);}while(0)
#define GASMODULE_UART_CLK_DISABLE()				do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, DISABLE);}while(0)
/* uart interrupt config */
#define GASMODULE_UART_NVIC_IRQ_CHANNEL			USART1_IRQn

/* uart interrupt handler */
#define GASMODULE_UART_IRQHANDLER				USART1_IRQHandler




void gasmodule_init(void);
void task_gasmodule(void);


#endif

