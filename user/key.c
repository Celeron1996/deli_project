#include "key.h"

static void key_gpio_init(void);
static void key_event_call(key_id_def id, key_event_def event);

static volatile uint8_t key_int_flag = 0;

static struct key_str key_handler[KEY_MAX_NUM];

static key_event_def key_event_buf;
static key_id_def key_event_id;
static uint8_t key_event_refresh = 0;

/* key gpio config */
static void key_gpio_init(void)
{
	GPIO_InitType gpio_init;
	EXTI_InitType exti_init;
	NVIC_InitType nvic_init;

	/* key_up int config */
	KEY_UP_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= KEY_UP_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_Pull_Up;
	gpio_init.GPIO_Mode		= GPIO_Mode_IT_Falling;
	GPIO_InitPeripheral(KEY_UP_GPIO_PORT, &gpio_init);
	GPIO_ConfigEXTILine(KEY_UP_EXIT_SOURCE_PORT, KEY_UP_EXIT_SOURCE_PIN);

	/*Configure key_up int EXTI line*/
	EXTI_InitStruct(&exti_init);
	exti_init.EXTI_Line    = KEY_UP_EXIT_LINE;
	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_InitPeripheral(&exti_init);

	/*Set key_up int interrupt priority*/
	nvic_init.NVIC_IRQChannel                   = KEY_UP_NVIC_IRQ_CHANNEL;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x05;
	nvic_init.NVIC_IRQChannelSubPriority        = 0x0F;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init);

	/* key_up int config */
	KEY_DOWN_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= KEY_DOWN_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_Pull_Up;
	gpio_init.GPIO_Mode		= GPIO_Mode_IT_Falling;
	GPIO_InitPeripheral(KEY_DOWN_GPIO_PORT, &gpio_init);
	GPIO_ConfigEXTILine(KEY_DOWN_EXIT_SOURCE_PORT, KEY_DOWN_EXIT_SOURCE_PIN);
	

	/*Configure key_up int EXTI line*/
	EXTI_InitStruct(&exti_init);
	exti_init.EXTI_Line    = KEY_DOWN_EXIT_LINE;
	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_InitPeripheral(&exti_init);

	/*Set key_up int interrupt priority*/
	nvic_init.NVIC_IRQChannel                   = KEY_DOWN_NVIC_IRQ_CHANNEL;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0x05;
	nvic_init.NVIC_IRQChannelSubPriority        = 0x0F;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init);
}

/* key init */
void key_init(void)
{
	key_gpio_init();

	
	/* init key static RAM */
	for (uint8_t i = 0; i < KEY_MAX_NUM; i++)
	{
		key_handler[i].status = key_status_release;
		key_handler[i].sta_bits = 0xFF;
		key_handler[i].hold_cnt = 0;
	}
	
}


void key_up_irq_call(void)
{
	if(RESET != EXTI_GetITStatus(KEY_UP_EXIT_LINE))
	{
		EXTI_ClrITPendBit(KEY_UP_EXIT_LINE);

		/* MY CODE BEGIN */
		key_int_flag += KEY_MAX_NUM;

		sleep_counter_reload();		/* 外部中断后应该保持运行一段时间 */
	}
}


void key_down_irq_call(void)
{
	if(RESET != EXTI_GetITStatus(KEY_DOWN_EXIT_LINE))
	{
		EXTI_ClrITPendBit(KEY_DOWN_EXIT_LINE);

		/* MY CODE BEGIN */
		key_int_flag += KEY_MAX_NUM;

		sleep_counter_reload();		/* 外部中断后应该保持运行一段时间 */
	}
}


/* 按键扫描 */
void key_scanner(void)
{
	static tick_type key_tick = 0;
	uint8_t pin_state_key_up;
	uint8_t pin_state_key_down;
	uint8_t i;

	if ((key_int_flag) && (!system_tick_timer_is_run())){
		system_tick_init();
	}

	if (((system_get_tick_cnt_ms() - key_tick) >= KEY_SCAN_PERIOD_MS) && (key_int_flag))
	{
		pin_state_key_up = GPIO_ReadInputDataBit(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN);
		pin_state_key_down = GPIO_ReadInputDataBit(KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN);

		if ((pin_state_key_up == Bit_RESET) && (pin_state_key_down != Bit_RESET))
		{
			key_handler[key_up].sta_bits <<= 1;
			key_handler[key_down].sta_bits <<= 1;
			key_handler[key_set].sta_bits <<= 1;

			key_handler[key_up].sta_bits |= 0x00;
			key_handler[key_down].sta_bits |= 0x01;
			key_handler[key_set].sta_bits |= 0x01;
		}
		else if((pin_state_key_down == Bit_RESET) && (pin_state_key_up != Bit_RESET))
		{
			key_handler[key_up].sta_bits <<= 1;
			key_handler[key_down].sta_bits <<= 1;
			key_handler[key_set].sta_bits <<= 1;

			key_handler[key_up].sta_bits |= 0x01;
			key_handler[key_down].sta_bits |= 0x00;
			key_handler[key_set].sta_bits |= 0x01;
		}
		else if((pin_state_key_down == Bit_RESET) && (pin_state_key_up == Bit_RESET))
		{
			key_handler[key_up].sta_bits <<= 1;
			key_handler[key_down].sta_bits <<= 1;
			key_handler[key_set].sta_bits <<= 1;

			key_handler[key_up].sta_bits |= 0x01;
			key_handler[key_down].sta_bits |= 0x01;
			key_handler[key_set].sta_bits |= 0x00;
		}
		else
		{
			key_handler[key_up].sta_bits <<= 1;
			key_handler[key_down].sta_bits <<= 1;
			key_handler[key_set].sta_bits <<= 1;

			key_handler[key_up].sta_bits |= 0x01;
			key_handler[key_down].sta_bits |= 0x01;
			key_handler[key_set].sta_bits |= 0x01;
		}

		for (i = 0; i < KEY_MAX_NUM; i++)
		{
			switch (key_handler[i].sta_bits & KEY_TRIGGER_MASK)
			{
				case KEY_TRIGGER_MASK:	//release
				{
					if (key_handler[i].status != key_event_release)
					{
						key_handler[i].status = key_status_release;
						key_handler[i].hold_cnt = 0;
						key_event_call(i, key_event_release);
						
						if (key_int_flag){key_int_flag--;}
					}
					break;
				}
				case 0:
				{
					if (key_handler[i].status == key_status_release)		// press
					{
						key_handler[i].status = key_status_press;
						key_handler[i].hold_cnt = 0;
						key_event_call(i, key_event_press);
					}
					else if (key_handler[i].status == key_status_press)	// hold
					{
						key_handler[i].hold_cnt++;
						if (key_handler[i].hold_cnt == (3000/KEY_SCAN_PERIOD_MS))
						{
							key_event_call(i, key_event_hold_3s);
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
		
		key_tick = system_get_tick_cnt_ms();
	}

	if (system_tick_timer_is_run()){

		for (i = 0; i < KEY_MAX_NUM; i++){
			if (key_handler[i].status != key_event_release){
				break;
			}
		}

		if ((i >= KEY_MAX_NUM) && (key_int_flag == 0)){
			system_tick_deinit();
		}
		else{
			sleep_counter_reload();
		}
	}
}


/* 按键事件回调 */
static __INLINE void key_event_call(key_id_def id, key_event_def event)
{
	key_event_id = id;
	key_event_buf = event;
	key_event_refresh = 1;
}


/* 获取按键事件 */
uint8_t key_get_event(key_id_def *id, key_event_def *event)
{
	key_scanner();
	
	if (key_event_refresh)
	{
		*id = key_event_id;
		*event = key_event_buf;
		key_event_refresh = 0;
		return 1;
	}
	
	return 0;
}

