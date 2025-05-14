#include "touch.h"


/*
SC01T 触摸感应器可以用平均电容值作为基准检测感应点的电容变化。它可以通过任何非导电介质来感
应电容变化。这样感应模块就可以很好的跟水和灰尘隔离。SC01T有更强的抗干扰性和更好的一致性。这个芯
片可以工作在低功耗的环境下，当电源为5v时，工作电流为43uA，待机电流为4ua以下，也适用于电池应用。


工作模式设置端口。
1：当MD悬空时，芯片工作在省电模式下，输出模式为直接输出模式，当检测到手指触摸，输出低
电平。
2：当MD接VDD时，芯片工作在省电模式下，输出模式是锁存输出模式：每次检测到手指触摸，输出
电平翻转，状态锁存。
3、当MD接GND时，芯片工作在正常模式下，输出模式为直接输出模式，当检测到手指触摸，输出
低电平。
ps:目前用模式1，即悬空


芯片复位之后会读取外部电容值做为判断基准值。此过程大约300ms左右。

*/



static void touch_gpio_exti_config(uint8_t exti);
static volatile uint8_t touch_int_flag = 0;

/* touch gpio init */
void touch_init(void)
{
	GPIO_InitType gpio_init;
	EXTI_InitType exti_init;
	NVIC_InitType nvic_init;

	/* TOUCH int config */
	TOUCH_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= TOUCH_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_Pull_Up;
	gpio_init.GPIO_Mode		= GPIO_Mode_IT_Falling;
	GPIO_InitPeripheral(TOUCH_GPIO_PORT, &gpio_init);
	GPIO_ConfigEXTILine(TOUCH_EXIT_SOURCE_PORT, TOUCH_EXIT_SOURCE_PIN);

	/*Configure TOUCH int EXTI line*/
	EXTI_InitStruct(&exti_init);
	exti_init.EXTI_Line    = TOUCH_EXIT_LINE;
	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_InitPeripheral(&exti_init);

	/*Set TOUCH int interrupt priority*/
	nvic_init.NVIC_IRQChannel                   = TOUCH_NVIC_IRQ_CHANNEL;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x05;
	nvic_init.NVIC_IRQChannelSubPriority        = 0x0F;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init);
	
}


void touch_irq_call(void)
{
	if(RESET != EXTI_GetITStatus(TOUCH_EXIT_LINE))
	{
		EXTI_ClrITPendBit(TOUCH_EXIT_LINE);
		touch_int_flag = 1;
	}
}




void task_touch(void)
{
	static tick_type touch_tick_backlight_off = 0;		/* 下一次灭灯的时间 */
	static uint8_t backlight_flag = 0;			/* 背光亮灭标志位 */
	
	if (touch_int_flag){
		touch_gpio_exti_config(0);
		touch_tick_backlight_off = system_get_tick_cnt_ms() + 5000;
		backlight_on();
		backlight_flag = 1;
		touch_int_flag = 0;
	}

	if (backlight_flag){
		if (system_get_tick_cnt_ms() >= touch_tick_backlight_off){
			backlight_off();
			touch_gpio_exti_config(1);
			backlight_flag = 0;
		}
	}
}


static void touch_gpio_exti_config(FunctionalState cmd)
{
	EXTI_InitType exti_init;

	/*Configure TOUCH int EXTI line*/
	EXTI_InitStruct(&exti_init);
	exti_init.EXTI_Line    = TOUCH_EXIT_LINE;
	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = cmd;
	EXTI_InitPeripheral(&exti_init);
}

