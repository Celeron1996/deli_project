#ifndef IWDG_H__
#define IWDG_H__

#include <stdio.h>
#include <stdint.h>
#include "n32l40x.h"


void iwdg_init(void);

#define IWDG_RELOAD()	do{IWDG->KEY = 0xAAAA;}while(0);

#endif


