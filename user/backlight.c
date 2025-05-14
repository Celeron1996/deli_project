#include "backlight.h"




/* backlight gpio init */
void backlight_init(void)
{
	GPIO_InitType gpio_init;

	/* gpio init */
	BACKLIGHT_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= BACKLIGHT_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_No_Pull;
	gpio_init.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(BACKLIGHT_GPIO_PORT, &gpio_init);

	backlight_ctr(1);
}


