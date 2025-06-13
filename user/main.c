#include <stdio.h>
#include <stdint.h>
#include "n32l40x.h"

#include "aht20.h"
#include "lcd.h"
#include "delay.h"
#include "system.h"
#include "key.h"
#include "rtc.h"
#include "myi2c.h"
#include "backlight.h"
#include "touch.h"
#include "gasmodule.h"
#include "myadc.h"
#include "battery.h"
#include "sd8568.h"



#define RTC_TYPE_MCU				0
#define RTC_TYPE_SD8568			1
#define RTC_TYPE						(RTC_TYPE_SD8568)

void printf_init(void);

uint8_t task_date_time_set(void);
void task_refresh_date_time(void);
void task_colon_flicker(void);

void my_get_date_time(uint16_t *year, uint8_t *mon, uint8_t *day, uint8_t *week, uint8_t *hour, uint8_t *min, uint8_t *sec);
void my_set_date_time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);


int main(void)
{
	system_tick_init();

	RTC_config();

	i2c_master_init();

	myadc_init();
	
	lcd_init();

	backlight_init();
	touch_init();

	key_init();
	
	aht20_init();
	gasmodule_init();
	battery_init();

	lcd_time_display(12, 0);
	lcd_colon_config(1);
	lcd_date_display(4, 21);
	lcd_week_display(1);
	lcd_temper_humid_display(28, 75);
	lcd_battery_config(1);
	lcd_bluetooth_config(0);
	
	while (1)
	{

		if (!task_date_time_set()){
			task_refresh_date_time();
			task_colon_flicker();
		}
		task_aht20();
		task_touch();
		task_gasmodule();
		task_battery_charge_flag_refresh();
		task_battery_voltage_refresh();
		task_battery_lowpower_refresh();
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
    //USART_SendData(USARTx, (uint8_t)ch);
   // while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);

    return (ch);
}








uint8_t task_date_time_set(void)
{
	static enum {
		step_init,
		step_set_year_l,
		step_set_mon,
		step_set_day,
		step_set_hour,
		step_set_min,
		step_set_final,
	} step = step_init;
	static tick_type flick_tick = 0;
	static uint8_t flick_flag = 0;
	static uint16_t year = 2025;
	static uint8_t mon = 1;
	static uint8_t day = 1;
	static uint8_t hour = 12;
	static uint8_t min = 12;
	static uint8_t sec = 0;
	static uint8_t week = 0;
	static uint8_t set_flag = 0;
	key_id_def key_id;
	key_event_def key_event;


	if (!set_flag)
	{
		if (!key_get_event(&key_id, &key_event))
		{
			return set_flag;
		}

		if (!((key_id == key_set) && (key_event == key_event_hold_3s)))
		{
			return set_flag;
		}

		set_flag = 1;
	}


	switch (step)
	{
		case step_init:
		{
			lcd_colon_config(0);
			my_get_date_time(&year, &mon, &day, &week, &hour, &min, &sec);
			lcd_time_display((uint8_t)(year/100), (uint8_t)(year%100));
			lcd_date_display(mon, day);
			lcd_week_display(RTC_get_weekday_math(year, mon, day));
			step = step_set_year_l;
			break;
		}
		case step_set_year_l:
		{
			if (key_get_event(&key_id, &key_event))
			{
				if ((key_id == key_up)&&(key_event == key_event_press))
				{
					if (year < 2099){year++;}
					lcd_time_display((uint8_t)(year/100), (uint8_t)(year%100));
				}
				else if ((key_id == key_down)&&(key_event == key_event_press))
				{
					if (year > 2000){year--;}
					lcd_time_display((uint8_t)(year/100), (uint8_t)(year%100));
				}
				else if ((key_id == key_set)&&(key_event == key_event_press))
				{
					lcd_time_display((uint8_t)(year/100), (uint8_t)(year%100));
					flick_tick = 0;
					flick_flag = 1;
					step = step_set_mon;
					break;
				}
				lcd_week_display(RTC_get_weekday_math(year, mon, day));
			}
			else
			{
			}
			if ((system_get_tick_cnt_ms() - flick_tick) >= 500)
			{
				if (flick_flag){
					lcd_num_turnoff(3);
					lcd_num_turnoff(4);
					flick_flag = 0;
				}
				else{
					lcd_time_display((uint8_t)(year/100), (uint8_t)(year%100));
					flick_flag = 1;
				}
				flick_tick = system_get_tick_cnt_ms();
			}
			break;
		}
		case step_set_mon:
		{
			if (key_get_event(&key_id, &key_event))
			{
				if ((key_id == key_up)&&(key_event == key_event_press))
				{
					if (mon < 12){mon++;}
					lcd_date_display(mon, day);
				}
				else if ((key_id == key_down)&&(key_event == key_event_press))
				{
					if (mon > 1){mon--;}
					lcd_date_display(mon, day);
				}
				else if ((key_id == key_set)&&(key_event == key_event_press))
				{
					lcd_date_display(mon, day);
					flick_tick = 0;
					flick_flag = 1;
					step = step_set_day;
					break;
				}
				lcd_week_display(RTC_get_weekday_math(year, mon, day));
			}
			else
			{
			}
			if ((system_get_tick_cnt_ms() - flick_tick) >= 500)
			{
				if (flick_flag){
					lcd_num_turnoff(15);
					lcd_num_turnoff(16);
					flick_flag = 0;
				}
				else{
					lcd_date_display(mon, day);
					flick_flag = 1;
				}
				flick_tick = system_get_tick_cnt_ms();
			}
			break;
		}
		case step_set_day:
		{
			if (key_get_event(&key_id, &key_event))
			{
				if ((key_id == key_up)&&(key_event == key_event_press))
				{
					if (day < 31){day++;}
					lcd_date_display(mon, day);
				}
				else if ((key_id == key_down)&&(key_event == key_event_press))
				{
					if (day > 1){day--;}
					lcd_date_display(mon, day);
				}
				else if ((key_id == key_set)&&(key_event == key_event_press))
				{
					lcd_date_display(mon, day);
					flick_tick = 0;
					flick_flag = 1;
					step = step_set_hour;
					lcd_time_display(hour, min);
					lcd_colon_config(1);
					break;
				}
				lcd_week_display(RTC_get_weekday_math(year, mon, day));
			}
			else
			{
			}
			if ((system_get_tick_cnt_ms() - flick_tick) >= 500)
			{
				if (flick_flag){
					lcd_num_turnoff(13);
					lcd_num_turnoff(14);
					flick_flag = 0;
				}
				else{
					lcd_date_display(mon, day);
					flick_flag = 1;
				}
				flick_tick = system_get_tick_cnt_ms();
			}
			break;
		}
		case step_set_hour:
		{
			if (key_get_event(&key_id, &key_event))
			{
				if ((key_id == key_up)&&(key_event == key_event_press))
				{
					if (hour < 23){hour++;}
					lcd_time_display(hour, min);
				}
				else if ((key_id == key_down)&&(key_event == key_event_press))
				{
					if (hour > 1){hour--;}
					lcd_time_display(hour, min);
				}
				else if ((key_id == key_set)&&(key_event == key_event_press))
				{
					lcd_time_display(hour, min);
					flick_tick = 0;
					flick_flag = 1;
					step = step_set_min;
					break;
				}
			}
			else
			{
			}
			if ((system_get_tick_cnt_ms() - flick_tick) >= 500)
			{
				if (flick_flag){
					lcd_num_turnoff(1);
					lcd_num_turnoff(2);
					flick_flag = 0;
				}
				else{
					lcd_time_display(hour, min);
					flick_flag = 1;
				}
				flick_tick = system_get_tick_cnt_ms();
			}
			break;
		}
		case step_set_min:
		{
			if (key_get_event(&key_id, &key_event))
			{
				if ((key_id == key_up)&&(key_event == key_event_press))
				{
					if (min < 59){min++;}
					lcd_time_display(hour, min);
				}
				else if ((key_id == key_down)&&(key_event == key_event_press))
				{
					if (min > 1){min--;}
					lcd_time_display(hour, min);
				}
				else if ((key_id == key_set)&&(key_event == key_event_press))
				{
					lcd_time_display(hour, min);
					flick_tick = 0;
					flick_flag = 1;
					step = step_set_final;
					break;
				}
			}
			else
			{
			}
			if ((system_get_tick_cnt_ms() - flick_tick) >= 500)
			{
				if (flick_flag){
					lcd_num_turnoff(3);
					lcd_num_turnoff(4);
					flick_flag = 0;
				}
				else{
					lcd_time_display(hour, min);
					flick_flag = 1;
				}
				flick_tick = system_get_tick_cnt_ms();
			}
			break;
		}
		case step_set_final:
		{
			my_set_date_time(year, mon, day, hour, min, 0);

			lcd_time_display(hour, min);
			lcd_date_display(mon, day);
			lcd_week_display(RTC_get_weekday_math(year, mon, day));
			step = step_init;
			set_flag = 0;
			break;
		}
		default:break;
	}

	return set_flag;
}



/* 显示时间日期 */
void task_refresh_date_time(void)
{
	static tick_type tick_date_time = 0;
	static uint16_t year;
	static uint8_t mon, day, hour, min, sec, week;
	static uint8_t mon_old, day_old, hour_old, min_old;

	if ((system_get_tick_cnt_ms() - tick_date_time) >= 1000)
	{
		my_get_date_time(&year, &mon, &day, &week, &hour, &min, &sec);

		if ((hour != hour_old) || (min != min_old))
		{
			lcd_time_display(hour, min);
		}
		
		if ((mon != mon_old) || (day != day_old))
		{
			lcd_date_display(mon, day);
			lcd_week_display(week);
		}

		hour_old = hour;
		min_old = min;
		mon_old = mon;
		day_old = day;
		
		tick_date_time = system_get_tick_cnt_ms();
	}
}



void task_colon_flicker(void)
{
	static tick_type tick_colon_flicker = 0;
	static uint8_t flick_flag = 0;

	if ((system_get_tick_cnt_ms() - tick_colon_flicker) > 1000)
	{
		lcd_colon_config(flick_flag);
		
		if (flick_flag){
			flick_flag = 0;
		}
		else{
			flick_flag = 1;
		}
		tick_colon_flicker = system_get_tick_cnt_ms();
	}
}


void my_get_date_time(uint16_t *year, uint8_t *mon, uint8_t *day, uint8_t *week, uint8_t *hour, uint8_t *min, uint8_t *sec)
{
	#if (RTC_TYPE == RTC_TYPE_MCU)
	RTC_get_date_time(year, mon, day, week, hour, min, sec);
	#elif	(RTC_TYPE == RTC_TYPE_SD8568)
	
	sd8568_time_t time;

	if (sd8568_read_time(&time) == SD8568_EXIT_OK)
	{
		*year = time.year + 2000;
		*mon = time.month;
		*day = time.day;
		*week = time.week;
		if (*week == 0){
			*week = 7;			/* sd8568 的星期天等于0 */
		}
		*hour = time.hour;
		*min = time.minute;
		*sec = time.second;
	}
	else
	{
		*year = 2025;
		*mon = 6;
		*day = 13;
		*week = 5;
		*hour = 6;
		*min = 0;
		*sec = 0;
	}
	#endif
}


void my_set_date_time(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
	#if (RTC_TYPE == RTC_TYPE_MCU)
	RTC_set_date_time(year, mon, day, hour, min, sec);
	#elif	(RTC_TYPE == RTC_TYPE_SD8568)

	sd8568_time_t time;

	time.year = year - 2000;
	time.month = mon;
	time.day = day;
	time.hour = hour;
	time.minute = min;
	time.second = sec;
	time.week = RTC_get_weekday_math(year, mon, day);
	if (time.week == 7){
		time.week = 0;	/* sd8568 的星期天等于0 */
	}

	sd8568_write_time(&time);
	
	#endif
}

