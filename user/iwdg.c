#include "iwdg.h"





void iwdg_init(void)
{
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_IWDGRSTF) != RESET)
	{
		 /* IWDGRST flag set */
		 /* Clear reset flags */
		 RCC_ClrFlag();
	}

	/* Enable the LSI OSC */
	RCC_EnableLsi(ENABLE);
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET)
	{
	}

	/* IWDG timeout equal to 26 s (MAX) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteConfig(IWDG_WRITE_ENABLE);
	/* IWDG counter clock: LSI/256 */
	IWDG_SetPrescalerDiv(IWDG_PRESCALER_DIV256);
	
	IWDG_CntReload(0x0FFF);
	/* Reload IWDG counter */
	IWDG_ReloadKey();
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}



