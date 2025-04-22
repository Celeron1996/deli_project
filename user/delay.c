#include "delay.h"



void delay_ms(uint32_t ms)
{
    uint32_t tcnt;
    while(ms--)
    {
        tcnt = 32000 / 9;
        while (tcnt--){;}
    }
}


