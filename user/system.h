#ifndef SYSTEM_H
#define SYSTEM_H

#include "n32l40x.h"
#include "stdint.h"


#define tick_type	uint64_t

#define SYSTEM_TICK_PERIOD_MS			(5U)

#define system_get_tick_cnt_ms()				system_tick_counter_ms
#define system_add_tick_cnt_ms(x)		do{system_tick_counter_ms += (x);}while(0)

extern volatile tick_type system_tick_counter_ms;



void system_tick_init(void);
void system_tick_deinit(void);


#endif

