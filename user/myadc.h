#ifndef MYADC_H
#define MYADC_H

#include "n32l40x.h"
#include "stdint.h"

#define MYADC_REFERENCE_VCC			(3300u)


#define MYADC_CHAN0			ADC_CH_1_PA0




void myadc_init(void);
uint16_t myadc_get_voltage(uint8_t channel);



#endif

