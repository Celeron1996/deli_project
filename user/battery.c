#include "battery.h"


static uint8_t battery_charge_flage = 0;
static uint8_t battery_voltage = 0;


static struct {
	uint8_t charge_flag;
	uint16_t voltage;
	uint8_t lowpower_flag;
	uint8_t lowpower_cnt;
} battery;


void battery_init(void)
{
	GPIO_InitType gpio_init;
	EXTI_InitType exti_init;
	NVIC_InitType nvic_init;

	/* key_up int config */
	CHARGE_STATE_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= CHARGE_STATE_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_Pull_Up;
	gpio_init.GPIO_Mode		= GPIO_Mode_IT_Rising_Falling;
	GPIO_InitPeripheral(CHARGE_STATE_GPIO_PORT, &gpio_init);
	GPIO_ConfigEXTILine(CHARGE_STATE_EXIT_SOURCE_PORT, CHARGE_STATE_EXIT_SOURCE_PIN);

	/*Configure  int EXTI line*/
	EXTI_InitStruct(&exti_init);
	exti_init.EXTI_Line    = CHARGE_STATE_EXIT_LINE;
	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_InitPeripheral(&exti_init);

	/*Set key_up int interrupt priority*/
	nvic_init.NVIC_IRQChannel                   = CHARGE_STATE_NVIC_IRQ_CHANNEL;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x05;
	nvic_init.NVIC_IRQChannelSubPriority        = 0x0F;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init);

	for (uint8_t i = 0; i < 4; i++)
	{
		delay_ms(5);
		if (BATTERY_READ_GPIO_IS_CHARGING()){
			battery.charge_flag = 1;
		}
		else{
			battery.charge_flag = 0;
			break;
		}
	}

	lcd_battery_config(battery.charge_flag);

	battery.voltage = myadc_get_voltage(BATTERY_VOLTAGE_ADC_CHANNEL) * 2;
	battery.lowpower_cnt = 0;
	battery.lowpower_flag = 0;
}




void task_battery_charge_flag_refresh(void)
{
	static tick_type battery_tick_charge = 0;

	if ((system_get_tick_cnt_ms() - battery_tick_charge) > 3000){
	
		for (uint8_t i = 0; i < 4; i++)
		{
			if (BATTERY_READ_GPIO_IS_CHARGING()){
			
				battery.charge_flag = 1;
			}
			else{
				battery.charge_flag = 0;
				
				break;
			}
			delay_ms(5);
		}

		lcd_battery_config(battery.charge_flag);

		battery_tick_charge = system_get_tick_cnt_ms();
	}
}



void task_battery_voltage_refresh(void)
{
	static tick_type battery_tick_voltage = 0;
	static tick_type run_period = 3000;
	uint16_t vol_sum = 0;


	if (battery_is_charging()){
		run_period = 1000;
	}
	else{
		run_period = 1000;
	}

	if ((system_get_tick_cnt_ms() - battery_tick_voltage) > run_period){

		for (uint8_t i = 0; i < 4; i++)
		{
			vol_sum += (myadc_get_voltage(BATTERY_VOLTAGE_ADC_CHANNEL) * 2);
			delay_ms(2);
		}
		battery.voltage = vol_sum/4 ;

		if (battery.voltage < BATTERY_VOLTAGE_LOWPOWER){
			battery.lowpower_cnt++;
			if (battery.lowpower_cnt >= 5){
			
				battery.lowpower_flag = 1;
				battery.lowpower_cnt = 0;
			}
		}
		else{
			battery.lowpower_cnt = 0;
			battery.lowpower_flag = 0;
		}
		battery_tick_voltage = system_get_tick_cnt_ms();
		
	}
}



void task_battery_lowpower_refresh(void)
{
	static tick_type battery_lowpower_tick = 0;
	static uint8_t icon_flag = 0;

	if (!battery.charge_flag)
	{
		if ((system_get_tick_cnt_ms() - battery_lowpower_tick) > 1000)
		{
			if (battery.lowpower_flag)
			{
				if (icon_flag){
					icon_flag = 0;
				}
				else{
					icon_flag = 1;
				}
				lcd_battery_config(icon_flag);
			}
			else
			{
				lcd_battery_config(0);
			}
			
			battery_lowpower_tick = system_get_tick_cnt_ms();
		}

	}
}



uint8_t battery_is_charging(void)
{
	return battery.charge_flag;
}

