#ifndef SYSTEM_H
#define SYSTEM_H

#include "n32l40x.h"
#include "stdint.h"


#define tick_type	uint64_t

#define SYSTEM_TICK_PERIOD_MS			(5U)

#define system_get_tick_cnt_ms()				system_tick_counter_ms
#define system_add_tick_cnt_ms(x)		do{system_tick_counter_ms += (x);}while(0)

extern uint8_t system_tick_timer_run_flag;
#define system_tick_timer_is_run()		system_tick_timer_run_flag

extern volatile tick_type system_tick_counter_ms;
extern volatile uint32_t exti_trigger_mask;
#define system_get_exti_trigger_flag(x)	(exti_trigger_mask & x)
#define system_set_exti_trigger_flag(x)	do{exti_trigger_mask |= x;}while(0);
#define system_clear_exti_trigger_flag(x)	do{exti_trigger_mask &= (~x);}while(0);


void system_tick_init(void);
void system_tick_deinit(void);


#endif

