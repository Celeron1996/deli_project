#ifndef SLEEP_H
#define SLEEP_h



#include "n32l40x.h"
#include "stdint.h"
#include "system.h"



#define WAKEUP_SOURCE_RTC			((uint8_t)(0x01))
#define WAKEUP_SOURCE_EXTI		((uint8_t)(0x02))


#define SLEEP_COUNTER_TIMEOUT_MS		(8000u)

#define sleep_set_wakeup_source(x)	do{wakeup_source_mask |= (x);}while(0)
#define sleep_counter_reload()			do{sleep_counter_ms = system_get_tick_cnt_ms() + SLEEP_COUNTER_TIMEOUT_MS;}while(0)



extern volatile uint8_t wakeup_source_mask;
extern tick_type sleep_counter_ms;


#endif

