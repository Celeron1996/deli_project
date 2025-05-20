#include "gasmodule.h"


/******************************************************
B1|B2|B3|B4|B5|B6|B7|B8|B9|

B1`B2:模组地址
B3:TVOC高字节
B4:TVOC低字节
B5:HCHO高字节
B6:HCHO低字节
B7:CO2高字节
B8:CO2低字节
B9:校验和：(B1+B2+B3+B4+B5+B6+B7+B8)
污染气体浓度值(mg/m³)：(高字节*256 + 低字节) * 0.001
*******************************************************/

static volatile uint8_t gas_rx_buffer[16] = {0};
static volatile uint8_t gas_rx_cnt = 0;
static volatile uint8_t gas_rx_process_flag = 0;
static uint8_t gasmodule_exist_flag = 0;			/* 模块是否存在标志 */
static uint8_t gasmodule_power_flag = 0;				/* 模块运行标志，或者说供电标志 */
static uint16_t TVOC_ug_m3 = 0;	/* 总有机挥发物，0~2.000mg/m³ */
static uint16_t CO2_ug_m3 = 0;		/* 二氧化碳，350~2000pp(模拟值) */
static uint16_t HCHO_ug_m3 = 0;		/* 甲醛，0~1.000mg/m³ */

static void gasmodule_gpio_init(void);
static void gasmodule_exist_detect(void);
static void gasmodule_uart_init(void);
static void gasmodule_uart_deinit(void);
static void gasmodule_power_enable(uint8_t enable);


void gasmodule_init(void)
{
	for (uint8_t i = 0; i < sizeof(gas_rx_buffer); i++){
		gas_rx_buffer[i] = 0;
	}
	gas_rx_cnt = 0;
	gasmodule_exist_flag = 0;
	
	gasmodule_gpio_init();

	gasmodule_power_enable(1);

	gasmodule_exist_detect();
	if (gasmodule_exist_flag){
		lcd_HCHO_display(0);
	}
	else{
		lcd_HCHO_turnoff();
	}

	gasmodule_power_enable(0);
}




static void gasmodule_gpio_init(void)
{
	GPIO_InitType gpio_init;

	/* gpio init */
	GASMODULE_GPIO_CLK_ENABLE();
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= GASMODULE_GPIO_PIN;
	gpio_init.GPIO_Pull		= GPIO_No_Pull;
	gpio_init.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GASMODULE_GPIO_PORT, &gpio_init);

	gasmodule_ctr(0);
}




static void gasmodule_uart_init(void)
{
	NVIC_InitType nvic_init;
	GPIO_InitType gpio_init;
	USART_InitType usart_init;

	/* step 1 enable gpio rx / tx clk */
	GASMODULE_RX_GPIO_CLK_ENABLE();
	GASMODULE_TX_GPIO_CLK_ENABLE();

	/* step 2 enable clk */
	GASMODULE_UART_CLK_ENABLE();

	/* step 3 Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* step 4 Enable the nvic config */
	nvic_init.NVIC_IRQChannel					= GASMODULE_UART_NVIC_IRQ_CHANNEL;
	//nvic_init.NVIC_IRQChannelPreemptionPriority	= 0;
	nvic_init.NVIC_IRQChannelSubPriority		= 0;
	nvic_init.NVIC_IRQChannelCmd				= ENABLE;
	NVIC_Init(&nvic_init);	

	/* step 5 gpio config */
	/* Initialize gpio_init */
	GPIO_InitStruct(&gpio_init);

	/* Configure USARTy Tx as alternate function push-pull */
	gpio_init.Pin            = GASMODULE_TX_GPIO_PIN;    
	gpio_init.GPIO_Mode      = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Alternate = GASMODULE_TX_GPIO_AF;
	GPIO_InitPeripheral(GASMODULE_TX_GPIO_PORT, &gpio_init);

	/* Configure USARTz Tx as alternate function push-pull */
	gpio_init.Pin            = GASMODULE_RX_GPIO_PIN;
	gpio_init.GPIO_Alternate = GASMODULE_RX_GPIO_AF;
	GPIO_InitPeripheral(GASMODULE_RX_GPIO_PORT, &gpio_init);

	/* step 6 uart config */
	USART_StructInit(&usart_init);
	usart_init.BaudRate            = GASMODULE_UART_BAUDRATE;
	usart_init.WordLength          = USART_WL_8B;
	usart_init.StopBits            = USART_STPB_1;
	usart_init.Parity              = USART_PE_NO;
	usart_init.HardwareFlowControl = USART_HFCTRL_NONE;
	usart_init.Mode                = USART_MODE_RX | USART_MODE_TX;

	USART_Init(GASMODULE_UART, &usart_init);

	/* step 6.1 uart interrupt config */
	USART_ConfigInt(GASMODULE_UART, USART_INT_RXDNE, ENABLE);

	/* step 6.2 uart enable */
	USART_Enable(GASMODULE_UART, ENABLE);
}

static void gasmodule_uart_deinit(void)
{
	GPIO_InitType gpio_init;
	NVIC_InitType nvic_init;

	/* step 1 uart disable */
	USART_Enable(GASMODULE_UART, DISABLE);

	/* step 2 uart interrupt disable */
	USART_ConfigInt(GASMODULE_UART, USART_INT_RXDNE, DISABLE);

	/* step 3 uart reset config */
	USART_DeInit(GASMODULE_UART);

	/* step 4 gpio reset config */
	GPIO_InitStruct(&gpio_init);
	gpio_init.Pin			= GASMODULE_RX_GPIO_PIN;
	gpio_init.GPIO_Current	= GPIO_DC_2mA;
	gpio_init.GPIO_Pull		= GPIO_No_Pull;
	gpio_init.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GASMODULE_RX_GPIO_PORT, &gpio_init);

	gpio_init.Pin			= GASMODULE_TX_GPIO_PIN;
	GPIO_InitPeripheral(GASMODULE_TX_GPIO_PORT, &gpio_init);

	GPIO_WriteBit(GASMODULE_RX_GPIO_PORT, GASMODULE_RX_GPIO_PIN, Bit_RESET);
	GPIO_WriteBit(GASMODULE_TX_GPIO_PORT, GASMODULE_TX_GPIO_PIN, Bit_RESET);

	/* step 5 uart nvic reset config */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	nvic_init.NVIC_IRQChannel					= GASMODULE_UART_NVIC_IRQ_CHANNEL;
	//nvic_init.NVIC_IRQChannelPreemptionPriority	= 0;
	nvic_init.NVIC_IRQChannelSubPriority		= 0;
	nvic_init.NVIC_IRQChannelCmd				= DISABLE;
	NVIC_Init(&nvic_init);	

	/* step 6 uart clk disable */
	GASMODULE_UART_CLK_DISABLE();
}





void GASMODULE_UART_IRQHANDLER(void)
{
	if (USART_GetIntStatus(GASMODULE_UART, USART_INT_RXDNE) != RESET)
	{
	    if ((gas_rx_cnt < sizeof(gas_rx_buffer)) && (!gas_rx_process_flag)){
	    	gas_rx_buffer[gas_rx_cnt++] = USART_ReceiveData(GASMODULE_UART);
	    	if (gas_rx_cnt >= 9){
	    		gas_rx_process_flag = 1;
	    	}
	    }
	    else{
	    	USART_ReceiveData(GASMODULE_UART);
	    }
	}

  if(USART_GetIntStatus(GASMODULE_UART, USART_INT_OREF) != RESET)
  {
      /*Read the STS register first,and the read the DAT 
      register to clear the overflow interrupt*/
      (void)GASMODULE_UART->STS;
      (void)GASMODULE_UART->DAT;
  }
}


/******************************************************
B1|B2|B3|B4|B5|B6|B7|B8|B9|

B1`B2:模组地址
B3:TVOC高字节
B4:TVOC低字节
B5:HCHO高字节
B6:HCHO低字节
B7:CO2高字节
B8:CO2低字节
B9:校验和：(B1+B2+B3+B4+B5+B6+B7+B8)
污染气体浓度值(mg/m³)：(高字节*256 + 低字节) * 0.001
*******************************************************/
void gasmodule_data_process(void)
{
	uint8_t sum = 0;

	if (gas_rx_process_flag && gasmodule_exist_flag){
		for (uint8_t i = 0; i < 8; i++){
			sum += gas_rx_buffer[i];
		}

		if (sum == gas_rx_buffer[8]){

			TVOC_ug_m3 = gas_rx_buffer[2] * 256 + gas_rx_buffer[3];
			HCHO_ug_m3 = gas_rx_buffer[4] * 256 + gas_rx_buffer[5];
			CO2_ug_m3 = gas_rx_buffer[6] * 256 + gas_rx_buffer[7];
		}

		for (uint8_t i = 0; i < sizeof(gas_rx_buffer); i++){
			gas_rx_buffer[i] = 0;
		}
		gas_rx_cnt = 0;
		gas_rx_process_flag = 0;
	}
}


/* 模组每隔一秒发送一帧数据，等待五秒是否接收完整的帧数据 */
static void gasmodule_exist_detect(void)
{
	uint16_t delay_cnt = 0;
	uint8_t sum = 0;

	gasmodule_exist_flag = 0;
	
	while (delay_cnt++ < 5000)
	{
		if (gas_rx_process_flag){
			for (uint8_t i = 0; i < 8; i++){
				sum += gas_rx_buffer[i];
			}

			if (sum == gas_rx_buffer[8]){
				gasmodule_exist_flag = 1;
				return;
			}

			for (uint8_t i = 0; i < sizeof(gas_rx_buffer); i++){
				gas_rx_buffer[i] = 0;
			}
			gas_rx_cnt = 0;
			gas_rx_process_flag = 0;
		}

		delay_ms(1);
	}
}



static void gasmodule_power_enable(uint8_t enable)
{
	for (uint8_t i = 0; i < sizeof(gas_rx_buffer); i++){
		gas_rx_buffer[i] = 0;
	}
	gas_rx_cnt = 0;
	gas_rx_process_flag = 0;

	if (enable){
		gasmodule_ctr(enable);
		gasmodule_uart_init();
		gasmodule_power_flag = enable;
	}
	else{
		gasmodule_uart_deinit();
		gasmodule_ctr(enable);
		gasmodule_power_flag = enable;
	}
}



/* 每隔十分钟打开一次，每次运行一分钟，因为模块十分耗电60ma，并且需要预热一分钟 */
#define GASMODULE_SLEEP_TIME			(10*60*1000u)			/* 睡眠时间宏定义，单位ms */
#define GASMODULE_RUN_TIME				(1*60*1000u)			/* 运行时间宏定义，单位ms */
void task_gasmodule(void)
{
	static tick_type gas_next_step_time = 0;				/* 到了这个时间后执行下一个步骤 */
	static tick_type gas_step_run_tick_time = 0;		/* 用于run step的tick计数 */
	
	static enum {
		gas_step_init,
		gas_step_run,
		gas_step_sleep
	} step = gas_step_init;

	if (!gasmodule_exist_flag){
		return;
	}

	switch (step)
	{
		case gas_step_init:
		{
			gas_next_step_time = system_get_tick_cnt_ms() + GASMODULE_RUN_TIME;
			step = gas_step_run;
			break;
		}
		case gas_step_run:
		{
			if (!gasmodule_power_flag){
				gasmodule_power_enable(1);
			}

			gasmodule_data_process();		/* 处理串口接收的数据 */
			
			if ((system_get_tick_cnt_ms() - gas_step_run_tick_time) > 1000){	/* 每秒更定数据到lcd */
				lcd_HCHO_display(CO2_ug_m3);
				gas_step_run_tick_time = system_get_tick_cnt_ms();
			}

			if (system_get_tick_cnt_ms() > gas_next_step_time){
				gasmodule_power_enable(0);
				step = gas_step_sleep;
				gas_next_step_time = system_get_tick_cnt_ms() + GASMODULE_SLEEP_TIME;
			}
			break;
		}
		case gas_step_sleep:
		{
			if (system_get_tick_cnt_ms() > gas_next_step_time){
				step = gas_step_run;
				gas_next_step_time = system_get_tick_cnt_ms() + GASMODULE_RUN_TIME;
			}
			break;
		}
		default:break;
	}
}

