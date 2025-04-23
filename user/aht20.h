#ifndef AHT20_H
#define AHT20_H


#include <stdio.h>
#include <stdint.h>



#define AHT20_SLAVE_ADDR			0x38
#define AHT20_SLAVE_ADDR_7BIT			(AHT20_SLAVE_ADDR << 1)

void aht20_init(void);
void aht20_test(void);
void aht20_process(void);


#endif
