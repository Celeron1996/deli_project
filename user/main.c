#include <stdio.h>
#include <stdint.h>
#include "n32l40x.h"

#include "aht20.h"
#include "lcd.h"
#include "delay.h"

void printf_init(void);



int main(void)
{
	//printf_init();

	//printf("\n\r init printf! \n\r");
	
	
	//aht20_test();
	
	lcd_init();
	
		uint8_t min = 0;
		uint8_t hour = 0;
		

		lcd_time_display(12, 0);
		lcd_colon_config(1);
		lcd_date_display(4, 21);
		lcd_week_display(1);
		lcd_temper_humid_display(28, 75);
		lcd_HCHO_display(234);
		lcd_battery_config(1);
		lcd_bluetooth_config(1);
		
		
		while (1)
		{
			lcd_time_display(hour, min);
			delay_ms(1000);
			if (min++ >= 60)
			{
				if (hour++ >= 24)
				{
					hour = 0;
				}
				min = 0;
			}
		}
	
	return 0;
}








#define USARTx            USART1
#define USARTx_GPIO       GPIOA
#define USARTx_CLK        RCC_APB2_PERIPH_USART1
#define USARTx_GPIO_CLK   RCC_APB2_PERIPH_GPIOA
#define USARTx_RxPin      GPIO_PIN_10
#define USARTx_TxPin      GPIO_PIN_9
#define USARTx_Rx_GPIO_AF GPIO_AF4_USART1
#define USARTx_Tx_GPIO_AF GPIO_AF4_USART1
#define GPIO_APBxClkCmd   RCC_EnableAPB2PeriphClk
#define USART_APBxClkCmd  RCC_EnableAPB2PeriphClk
USART_InitType USART_InitStructure;
void printf_init(void)
{
	
    /* Enable GPIO clock */
    GPIO_APBxClkCmd(USARTx_GPIO_CLK, ENABLE);
    /* Enable USARTx Clock */
    USART_APBxClkCmd(USARTx_CLK, ENABLE);
	
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);

    /* Configure USARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Tx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);
    
    /* Configure USARTx Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = USARTx_RxPin;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Rx_GPIO_AF;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);
	
	
    /* USARTy and USARTz configuration ------------------------------------------------------*/
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    /* Configure USARTx */
    USART_Init(USARTx, &USART_InitStructure);
    /* Enable the USARTx */
    USART_Enable(USARTx, ENABLE);
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE* f)
{
    USART_SendData(USARTx, (uint8_t)ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET)
        ;

    return (ch);
}