#include "myadc.h"




static void myadc_chan1_gpio_init(void);



void myadc_init(void)
{
	ADC_InitType ADC_InitStructure;

	myadc_chan1_gpio_init();

	/* Enable ADC clocks */
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);

	/* RCC_ADCHCLK_DIV16*/
	ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB, RCC_ADCHCLK_DIV16);
	RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8);  //selsect HSI as RCC ADC1M CLK Source		

  ADC_DeInit(ADC);
  /* ADC configuration ------------------------------------------------------*/
  ADC_InitStructure.MultiChEn      = DISABLE;
  ADC_InitStructure.ContinueConvEn = DISABLE;
  ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
  ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
  ADC_InitStructure.ChsNumber      = 1;
  ADC_Init(ADC, &ADC_InitStructure);

  /* Enable ADC */
  ADC_Enable(ADC, ENABLE);
  /* Check ADC Ready */
  while(ADC_GetFlagStatusNew(ADC,ADC_FLAG_RDY) == RESET);
  
  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC);
  /* Check the end of ADC1 calibration */
  while (ADC_GetCalibrationStatus(ADC));

}



static void myadc_chan1_gpio_init(void)
{
  GPIO_InitType GPIO_InitStructure;
  
	/* Enable GPIOC clocks */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

  GPIO_InitStruct(&GPIO_InitStructure);
  GPIO_InitStructure.Pin       = GPIO_PIN_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
  GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}


uint16_t myadc_get_voltage(uint8_t channel)
{
    uint16_t dat;
    ADC_ConfigRegularChannel(ADC, channel, 1, ADC_SAMP_TIME_55CYCLES5);
    /* Start ADC Software Conversion */
    ADC_EnableSoftwareStartConv(ADC,ENABLE);
    while(ADC_GetFlagStatus(ADC,ADC_FLAG_ENDC)==0){
    }
    ADC_ClearFlag(ADC,ADC_FLAG_ENDC);
    ADC_ClearFlag(ADC,ADC_FLAG_STR);
    dat=ADC_GetDat(ADC);
    
    return (uint16_t)((dat * MYADC_REFERENCE_VCC) / 4096);
}

