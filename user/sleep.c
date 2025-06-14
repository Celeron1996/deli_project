#include "sleep.h"



volatile uint8_t wakeup_source_mask = 0;
tick_type sleep_counter_ms = SLEEP_COUNTER_TIMEOUT_MS;



void task_sleep(void)
{
	if (system_get_tick_cnt_ms() >= sleep_counter_ms)	/* into sleep */
	{
		PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI,PWR_CTRL3_RAM1RET|PWR_CTRL3_RAM2RET);
		system_add_tick_cnt_ms(1000);
	}
}


