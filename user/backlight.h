#ifndef BACKLIGHT_H
#define BACKLIGHT_H


#include "n32l40x.h"
#include "stdint.h"
#include "system.h"


#define BACKLIGHT_GPIO_PORT			GPIOA
#define BACKLIGHT_GPIO_PIN				GPIO_PIN_11
#define BACKLIGHT_GPIO_CLK_ENABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);}while(0)
#define BACKLIGHT_GPIO_CLK_DISABLE()	do{RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, DISABLE);}while(0)



#define backlight_on()	do{GPIO_WriteBit(BACKLIGHT_GPIO_PORT, BACKLIGHT_GPIO_PIN, Bit_SET);}while(0)
#define backlight_off()	do{GPIO_WriteBit(BACKLIGHT_GPIO_PORT, BACKLIGHT_GPIO_PIN, Bit_RESET);}while(0)
#define backlight_ctr(cmd)	do{\
if (cmd){backlight_on();}\
else{backlight_off();}\
}while(0)



void backlight_init(void);


#endif

