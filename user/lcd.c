#include "lcd.h"


static void lcd_bsp_init(void);
static void lcd_gpio_init(void);



const lcd_pixel_typdef lcd_pixel_1AGED = {lcd_reg_ram1_com0, LCD_BIT0};
const lcd_pixel_typdef lcd_pixel_2D = {lcd_reg_ram1_com0, LCD_BIT1};
const lcd_pixel_typdef lcd_pixel_COL = {lcd_reg_ram1_com0, LCD_BIT2};
const lcd_pixel_typdef lcd_pixel_3D = {lcd_reg_ram1_com0, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_T11 = {lcd_reg_ram1_com0, LCD_BIT4};
const lcd_pixel_typdef lcd_pixel_5D = {lcd_reg_ram1_com0, LCD_BIT6};
const lcd_pixel_typdef lcd_pixel_5C = {lcd_reg_ram1_com0, LCD_BIT7};
const lcd_pixel_typdef lcd_pixel_6D = {lcd_reg_ram1_com0, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_6C = {lcd_reg_ram1_com0, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_T4 = {lcd_reg_ram1_com0, LCD_BIT10};
const lcd_pixel_typdef lcd_pixel_7A = {lcd_reg_ram1_com0, LCD_BIT11};
const lcd_pixel_typdef lcd_pixel_T3 = {lcd_reg_ram1_com0, LCD_BIT12};
const lcd_pixel_typdef lcd_pixel_8A = {lcd_reg_ram1_com0, LCD_BIT13};
const lcd_pixel_typdef lcd_pixel_T5 = {lcd_reg_ram1_com0, LCD_BIT14};
const lcd_pixel_typdef lcd_pixel_9A = {lcd_reg_ram1_com0, LCD_BIT15};
const lcd_pixel_typdef lcd_pixel_T10 = {lcd_reg_ram1_com0, LCD_BIT16};
const lcd_pixel_typdef lcd_pixel_4D = {lcd_reg_ram1_com0, LCD_BIT17};
const lcd_pixel_typdef lcd_pixel_10A = {lcd_reg_ram1_com0, LCD_BIT18};
const lcd_pixel_typdef lcd_pixel_T9 = {lcd_reg_ram1_com0, LCD_BIT19};
const lcd_pixel_typdef lcd_pixel_11A = {lcd_reg_ram1_com0, LCD_BIT20};
const lcd_pixel_typdef lcd_pixel_T6 = {lcd_reg_ram1_com0, LCD_BIT21};
const lcd_pixel_typdef lcd_pixel_12A = {lcd_reg_ram1_com0, LCD_BIT22};
const lcd_pixel_typdef lcd_pixel_TT = {lcd_reg_ram1_com0, LCD_BIT23};
const lcd_pixel_typdef lcd_pixel_T14 = {lcd_reg_ram1_com0, LCD_BIT24};
const lcd_pixel_typdef lcd_pixel_13A = {lcd_reg_ram1_com0, LCD_BIT25};
const lcd_pixel_typdef lcd_pixel_T13 = {lcd_reg_ram1_com0, LCD_BIT26};
const lcd_pixel_typdef lcd_pixel_14A = {lcd_reg_ram1_com0, LCD_BIT27};

const lcd_pixel_typdef lcd_pixel_16A = {lcd_reg_ram2_com0, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_T15 = {lcd_reg_ram2_com0, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_15A = {lcd_reg_ram2_com0, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_T12 = {lcd_reg_ram2_com0, LCD_BIT10};


const lcd_pixel_typdef lcd_pixel_1C = {lcd_reg_ram1_com1, LCD_BIT0};
const lcd_pixel_typdef lcd_pixel_2E = {lcd_reg_ram1_com1, LCD_BIT1};
const lcd_pixel_typdef lcd_pixel_2C = {lcd_reg_ram1_com1, LCD_BIT2};
const lcd_pixel_typdef lcd_pixel_3E = {lcd_reg_ram1_com1, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_3C = {lcd_reg_ram1_com1, LCD_BIT4};
const lcd_pixel_typdef lcd_pixel_4C = {lcd_reg_ram1_com1, LCD_BIT5};
const lcd_pixel_typdef lcd_pixel_5E = {lcd_reg_ram1_com1, LCD_BIT6};
const lcd_pixel_typdef lcd_pixel_5G = {lcd_reg_ram1_com1, LCD_BIT7};
const lcd_pixel_typdef lcd_pixel_6E = {lcd_reg_ram1_com1, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_6G = {lcd_reg_ram1_com1, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_7B = {lcd_reg_ram1_com1, LCD_BIT10};
const lcd_pixel_typdef lcd_pixel_7F = {lcd_reg_ram1_com1, LCD_BIT11};
const lcd_pixel_typdef lcd_pixel_8B = {lcd_reg_ram1_com1, LCD_BIT12};
const lcd_pixel_typdef lcd_pixel_8F = {lcd_reg_ram1_com1, LCD_BIT13};
const lcd_pixel_typdef lcd_pixel_9B = {lcd_reg_ram1_com1, LCD_BIT14};
const lcd_pixel_typdef lcd_pixel_9F = {lcd_reg_ram1_com1, LCD_BIT15};
const lcd_pixel_typdef lcd_pixel_10B = {lcd_reg_ram1_com1, LCD_BIT16};
const lcd_pixel_typdef lcd_pixel_4E = {lcd_reg_ram1_com1, LCD_BIT17};
const lcd_pixel_typdef lcd_pixel_10F = {lcd_reg_ram1_com1, LCD_BIT18};
const lcd_pixel_typdef lcd_pixel_11B = {lcd_reg_ram1_com1, LCD_BIT19};
const lcd_pixel_typdef lcd_pixel_11F = {lcd_reg_ram1_com1, LCD_BIT20};
const lcd_pixel_typdef lcd_pixel_12B = {lcd_reg_ram1_com1, LCD_BIT21};
const lcd_pixel_typdef lcd_pixel_12F = {lcd_reg_ram1_com1, LCD_BIT22};
const lcd_pixel_typdef lcd_pixel_T8 = {lcd_reg_ram1_com1, LCD_BIT23};
const lcd_pixel_typdef lcd_pixel_13B = {lcd_reg_ram1_com1, LCD_BIT24};
const lcd_pixel_typdef lcd_pixel_13F = {lcd_reg_ram1_com1, LCD_BIT25};
const lcd_pixel_typdef lcd_pixel_14B = {lcd_reg_ram1_com1, LCD_BIT26};
const lcd_pixel_typdef lcd_pixel_14F = {lcd_reg_ram1_com1, LCD_BIT27};


const lcd_pixel_typdef lcd_pixel_16F = {lcd_reg_ram2_com1, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_15B = {lcd_reg_ram2_com1, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_15F = {lcd_reg_ram2_com1, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_16B = {lcd_reg_ram2_com1, LCD_BIT10};


const lcd_pixel_typdef lcd_pixel_1B = {lcd_reg_ram1_com2, LCD_BIT0};
const lcd_pixel_typdef lcd_pixel_2F = {lcd_reg_ram1_com2, LCD_BIT1};
const lcd_pixel_typdef lcd_pixel_2G = {lcd_reg_ram1_com2, LCD_BIT2};
const lcd_pixel_typdef lcd_pixel_3F = {lcd_reg_ram1_com2, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_3G = {lcd_reg_ram1_com2, LCD_BIT4};
const lcd_pixel_typdef lcd_pixel_4G = {lcd_reg_ram1_com2, LCD_BIT5};
const lcd_pixel_typdef lcd_pixel_5F = {lcd_reg_ram1_com2, LCD_BIT6};
const lcd_pixel_typdef lcd_pixel_5B = {lcd_reg_ram1_com2, LCD_BIT7};
const lcd_pixel_typdef lcd_pixel_6F = {lcd_reg_ram1_com2, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_6B = {lcd_reg_ram1_com2, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_7G = {lcd_reg_ram1_com2, LCD_BIT10};
const lcd_pixel_typdef lcd_pixel_7E = {lcd_reg_ram1_com2, LCD_BIT11};
const lcd_pixel_typdef lcd_pixel_8G = {lcd_reg_ram1_com2, LCD_BIT12};
const lcd_pixel_typdef lcd_pixel_8E = {lcd_reg_ram1_com2, LCD_BIT13};
const lcd_pixel_typdef lcd_pixel_9G = {lcd_reg_ram1_com2, LCD_BIT14};
const lcd_pixel_typdef lcd_pixel_9E = {lcd_reg_ram1_com2, LCD_BIT15};
const lcd_pixel_typdef lcd_pixel_10G = {lcd_reg_ram1_com2, LCD_BIT16};
const lcd_pixel_typdef lcd_pixel_4F = {lcd_reg_ram1_com2, LCD_BIT17};
const lcd_pixel_typdef lcd_pixel_10E = {lcd_reg_ram1_com2, LCD_BIT18};
const lcd_pixel_typdef lcd_pixel_11G = {lcd_reg_ram1_com2, LCD_BIT19};
const lcd_pixel_typdef lcd_pixel_11E = {lcd_reg_ram1_com2, LCD_BIT20};
const lcd_pixel_typdef lcd_pixel_12G = {lcd_reg_ram1_com2, LCD_BIT21};
const lcd_pixel_typdef lcd_pixel_12E = {lcd_reg_ram1_com2, LCD_BIT22};
const lcd_pixel_typdef lcd_pixel_T7 = {lcd_reg_ram1_com2, LCD_BIT23};
const lcd_pixel_typdef lcd_pixel_13G = {lcd_reg_ram1_com2, LCD_BIT24};
const lcd_pixel_typdef lcd_pixel_13E = {lcd_reg_ram1_com2, LCD_BIT25};
const lcd_pixel_typdef lcd_pixel_14G = {lcd_reg_ram1_com2, LCD_BIT26};
const lcd_pixel_typdef lcd_pixel_14E = {lcd_reg_ram1_com2, LCD_BIT27};

const lcd_pixel_typdef lcd_pixel_16E = {lcd_reg_ram2_com2, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_15G = {lcd_reg_ram2_com2, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_15E = {lcd_reg_ram2_com2, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_16G = {lcd_reg_ram2_com2, LCD_BIT10};


const lcd_pixel_typdef lcd_pixel_2A = {lcd_reg_ram1_com3, LCD_BIT1};
const lcd_pixel_typdef lcd_pixel_2B = {lcd_reg_ram1_com3, LCD_BIT2};
const lcd_pixel_typdef lcd_pixel_3A = {lcd_reg_ram1_com3, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_3B = {lcd_reg_ram1_com3, LCD_BIT4};
const lcd_pixel_typdef lcd_pixel_4B = {lcd_reg_ram1_com3, LCD_BIT5};
const lcd_pixel_typdef lcd_pixel_5A = {lcd_reg_ram1_com3, LCD_BIT6};
const lcd_pixel_typdef lcd_pixel_T1 = {lcd_reg_ram1_com3, LCD_BIT7};
const lcd_pixel_typdef lcd_pixel_6A = {lcd_reg_ram1_com3, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_T2 = {lcd_reg_ram1_com3, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_7C = {lcd_reg_ram1_com3, LCD_BIT10};
const lcd_pixel_typdef lcd_pixel_7D = {lcd_reg_ram1_com3, LCD_BIT11};
const lcd_pixel_typdef lcd_pixel_8C = {lcd_reg_ram1_com3, LCD_BIT12};
const lcd_pixel_typdef lcd_pixel_8D = {lcd_reg_ram1_com3, LCD_BIT13};
const lcd_pixel_typdef lcd_pixel_9C = {lcd_reg_ram1_com3, LCD_BIT14};
const lcd_pixel_typdef lcd_pixel_9D = {lcd_reg_ram1_com3, LCD_BIT15};
const lcd_pixel_typdef lcd_pixel_10C = {lcd_reg_ram1_com3, LCD_BIT16};
const lcd_pixel_typdef lcd_pixel_4A = {lcd_reg_ram1_com3, LCD_BIT17};
const lcd_pixel_typdef lcd_pixel_10D = {lcd_reg_ram1_com3, LCD_BIT18};
const lcd_pixel_typdef lcd_pixel_11C = {lcd_reg_ram1_com3, LCD_BIT19};
const lcd_pixel_typdef lcd_pixel_11D = {lcd_reg_ram1_com3, LCD_BIT20};
const lcd_pixel_typdef lcd_pixel_12C = {lcd_reg_ram1_com3, LCD_BIT21};
const lcd_pixel_typdef lcd_pixel_12D = {lcd_reg_ram1_com3, LCD_BIT22};
const lcd_pixel_typdef lcd_pixel_13C = {lcd_reg_ram1_com3, LCD_BIT24};
const lcd_pixel_typdef lcd_pixel_13D = {lcd_reg_ram1_com3, LCD_BIT25};
const lcd_pixel_typdef lcd_pixel_14C = {lcd_reg_ram1_com3, LCD_BIT26};
const lcd_pixel_typdef lcd_pixel_14D = {lcd_reg_ram1_com3, LCD_BIT27};


const lcd_pixel_typdef lcd_pixel_16D = {lcd_reg_ram2_com3, LCD_BIT3};
const lcd_pixel_typdef lcd_pixel_15C = {lcd_reg_ram2_com3, LCD_BIT8};
const lcd_pixel_typdef lcd_pixel_15D = {lcd_reg_ram2_com3, LCD_BIT9};
const lcd_pixel_typdef lcd_pixel_16C = {lcd_reg_ram2_com3, LCD_BIT10};







/* 屏幕上第一个数字的所有像素点集合 */
/* 每个数字的像素按如下顺序排列：A,B,C,D,E,F,G */
const lcd_pixel_typdef *lcd_num1_pixel_arr[] = {

	&lcd_pixel_1AGED,		// A
	&lcd_pixel_1B,			// B
	&lcd_pixel_1C,			// C
	&lcd_pixel_1AGED,		// D
	&lcd_pixel_1AGED,		// E
	0,									// F
	&lcd_pixel_1AGED,		// G
};

const lcd_pixel_typdef *lcd_num2_pixel_arr[] = {
	&lcd_pixel_2A,		// A
	&lcd_pixel_2B,		// B
	&lcd_pixel_2C,		// C
	&lcd_pixel_2D,		// D
	&lcd_pixel_2E,		// E
	&lcd_pixel_2F,		// F
	&lcd_pixel_2G,		// G
};

const lcd_pixel_typdef *lcd_num3_pixel_arr[] = {
	&lcd_pixel_3A,		// A
	&lcd_pixel_3B,		// B
	&lcd_pixel_3C,		// C
	&lcd_pixel_3D,		// D
	&lcd_pixel_3E,		// E
	&lcd_pixel_3F,		// F
	&lcd_pixel_3G,		// G
};

const lcd_pixel_typdef *lcd_num4_pixel_arr[] = {
	&lcd_pixel_4A,		// A
	&lcd_pixel_4B,		// B
	&lcd_pixel_4C,		// C
	&lcd_pixel_4D,		// D
	&lcd_pixel_4E,		// E
	&lcd_pixel_4F,		// F
	&lcd_pixel_4G,		// G
};

const lcd_pixel_typdef *lcd_num5_pixel_arr[] = {
	&lcd_pixel_5A,		// A
	&lcd_pixel_5B,		// B
	&lcd_pixel_5C,		// C
	&lcd_pixel_5D,		// D
	&lcd_pixel_5E,		// E
	&lcd_pixel_5F,		// F
	&lcd_pixel_5G,		// G
};

const lcd_pixel_typdef *lcd_num6_pixel_arr[] = {
	&lcd_pixel_6A,		// A
	&lcd_pixel_6B,		// B
	&lcd_pixel_6C,		// C
	&lcd_pixel_6D,		// D
	&lcd_pixel_6E,		// E
	&lcd_pixel_6F,		// F
	&lcd_pixel_6G,		// G
};

const lcd_pixel_typdef *lcd_num7_pixel_arr[] = {
	&lcd_pixel_7A,		// A
	&lcd_pixel_7B,		// B
	&lcd_pixel_7C,		// C
	&lcd_pixel_7D,		// D
	&lcd_pixel_7E,		// E
	&lcd_pixel_7F,		// F
	&lcd_pixel_7G,		// G
};

const lcd_pixel_typdef *lcd_num8_pixel_arr[] = {
	&lcd_pixel_8A,		// A
	&lcd_pixel_8B,		// B
	&lcd_pixel_8C,		// C
	&lcd_pixel_8D,		// D
	&lcd_pixel_8E,		// E
	&lcd_pixel_8F,		// F
	&lcd_pixel_8G,		// G
};

const lcd_pixel_typdef *lcd_num9_pixel_arr[] = {
	&lcd_pixel_9A,		// A
	&lcd_pixel_9B,		// B
	&lcd_pixel_9C,		// C
	&lcd_pixel_9D,		// D
	&lcd_pixel_9E,		// E
	&lcd_pixel_9F,		// F
	&lcd_pixel_9G,		// G
};

const lcd_pixel_typdef *lcd_num10_pixel_arr[] = {
	&lcd_pixel_10A,		// A
	&lcd_pixel_10B,		// B
	&lcd_pixel_10C,		// C
	&lcd_pixel_10D,		// D
	&lcd_pixel_10E,		// E
	&lcd_pixel_10F,		// F
	&lcd_pixel_10G,		// G
};

const lcd_pixel_typdef *lcd_num11_pixel_arr[] = {
	&lcd_pixel_11A,		// A
	&lcd_pixel_11B,		// B
	&lcd_pixel_11C,		// C
	&lcd_pixel_11D,		// D
	&lcd_pixel_11E,		// E
	&lcd_pixel_11F,		// F
	&lcd_pixel_11G,		// G
};

const lcd_pixel_typdef *lcd_num12_pixel_arr[] = {
	&lcd_pixel_12A,		// A
	&lcd_pixel_12B,		// B
	&lcd_pixel_12C,		// C
	&lcd_pixel_12D,		// D
	&lcd_pixel_12E,		// E
	&lcd_pixel_12F,		// F
	&lcd_pixel_12G,		// G
};

const lcd_pixel_typdef *lcd_num13_pixel_arr[] = {
	&lcd_pixel_13A,		// A
	&lcd_pixel_13B,		// B
	&lcd_pixel_13C,		// C
	&lcd_pixel_13D,		// D
	&lcd_pixel_13E,		// E
	&lcd_pixel_13F,		// F
	&lcd_pixel_13G,		// G
};

const lcd_pixel_typdef *lcd_num14_pixel_arr[] = {
	&lcd_pixel_14A,		// A
	&lcd_pixel_14B,		// B
	&lcd_pixel_14C,		// C
	&lcd_pixel_14D,		// D
	&lcd_pixel_14E,		// E
	&lcd_pixel_14F,		// F
	&lcd_pixel_14G,		// G
};

const lcd_pixel_typdef *lcd_num15_pixel_arr[] = {
	&lcd_pixel_15A,		// A
	&lcd_pixel_15B,		// B
	&lcd_pixel_15C,		// C
	&lcd_pixel_15D,		// D
	&lcd_pixel_15E,		// E
	&lcd_pixel_15F,		// F
	&lcd_pixel_15G,		// G
};

const lcd_pixel_typdef *lcd_num16_pixel_arr[] = {
	&lcd_pixel_16A,		// A
	&lcd_pixel_16B,		// B
	&lcd_pixel_16C,		// C
	&lcd_pixel_16D,		// D
	&lcd_pixel_16E,		// E
	&lcd_pixel_16F,		// F
	&lcd_pixel_16G,		// G
};


const lcd_pixel_typdef **lcd_num_arr[] = {
	&lcd_num1_pixel_arr[0],
	&lcd_num2_pixel_arr[0],
	&lcd_num3_pixel_arr[0],
	&lcd_num4_pixel_arr[0],
	&lcd_num5_pixel_arr[0],
	&lcd_num6_pixel_arr[0],
	&lcd_num7_pixel_arr[0],
	&lcd_num8_pixel_arr[0],
	&lcd_num9_pixel_arr[0],
	&lcd_num10_pixel_arr[0],
	&lcd_num11_pixel_arr[0],
	&lcd_num12_pixel_arr[0],
	&lcd_num13_pixel_arr[0],
	&lcd_num14_pixel_arr[0],
	&lcd_num15_pixel_arr[0],
	&lcd_num16_pixel_arr[0],
};


#define SEGA			0U
#define SEGB			1U
#define SEGC			2U
#define SEGD			3U
#define SEGE			4U
#define SEGF			5U
#define SEGG			6U
#define SEGNULL		0xFFU
const uint8_t lcd_num_display_index[10][7] = {
{SEGA, SEGB, SEGC, SEGD, SEGE, SEGF, SEGNULL},		// 0 ---> ABCDEF
{SEGB, SEGC, SEGNULL, SEGNULL, SEGNULL, SEGNULL, SEGNULL},		// 1 ---> BC
{SEGA, SEGB, SEGG, SEGE, SEGD, SEGNULL, SEGNULL},		// 2 ---> ABGED
{SEGA, SEGB, SEGC, SEGD, SEGG, SEGNULL, SEGNULL},		// 3 ---> ABCDG
{SEGB, SEGC, SEGF, SEGG, SEGNULL, SEGNULL, SEGNULL},		//  4 ---> BCFG
{SEGA, SEGF, SEGG, SEGC, SEGD, SEGNULL, SEGNULL},		//  5 ---> AFGCD
{SEGA, SEGF, SEGE, SEGD, SEGC, SEGG, SEGNULL},		//  6 ---> AFEDCG
{SEGA, SEGB, SEGC, SEGNULL, SEGNULL, SEGNULL, SEGNULL},		//  7 ---> ABC
{SEGA, SEGB, SEGC, SEGD, SEGE, SEGF, SEGG},		//  8 ---> ABCDEFG
{SEGA, SEGB, SEGC, SEGD, SEGF, SEGG, SEGNULL},		//  9 ---> ABCDFG
};


/* 显示某个数字 */
void lcd_num_display(uint8_t num_index, uint8_t num_dis)
{
	lcd_pixel_typdef **lcd_num_pixel_arr;
	lcd_pixel_typdef *p_lcd_num_pixel;
	uint8_t i;
	
	if ((num_index > (sizeof(lcd_num_arr)/sizeof(void *))) || (num_index == 0) || (num_dis > 9))
	{
		return ;
	}

	lcd_num_pixel_arr = (lcd_pixel_typdef **)lcd_num_arr[num_index-1];

	for (i = 0; i < 7; i++)
	{
		p_lcd_num_pixel = lcd_num_pixel_arr[lcd_num_display_index[8][i]];
		
		if (p_lcd_num_pixel != 0)
		{
			LCD_Write(p_lcd_num_pixel->reg_index, ~(p_lcd_num_pixel->bit_map), ~(p_lcd_num_pixel->bit_map));
		}
	}

	for (i = 0; i < 7; i++)
	{
		if (lcd_num_display_index[num_dis][i] == SEGNULL)	{break;}
		
		p_lcd_num_pixel = lcd_num_pixel_arr[lcd_num_display_index[num_dis][i]];
		if (p_lcd_num_pixel != 0)
		{
			LCD_Write(p_lcd_num_pixel->reg_index, ~(p_lcd_num_pixel->bit_map), p_lcd_num_pixel->bit_map);
		}
	}
	
	LCD_UpdateDisplayRequest();
}


/* 关闭某个数字显示 */
void lcd_num_turnoff(uint8_t num_index)
{
	lcd_pixel_typdef **lcd_num_pixel_arr;
	lcd_pixel_typdef *p_lcd_num_pixel;
	uint8_t i;
	
	if ((num_index > (sizeof(lcd_num_arr)/sizeof(void *))) || (num_index == 0))
	{
		return ;
	}

	lcd_num_pixel_arr = (lcd_pixel_typdef **)lcd_num_arr[num_index-1];

	for (i = 0; i < 7; i++)
	{
		p_lcd_num_pixel = lcd_num_pixel_arr[lcd_num_display_index[8][i]];
		
		if (p_lcd_num_pixel != 0)
		{
			LCD_Write(p_lcd_num_pixel->reg_index, ~(p_lcd_num_pixel->bit_map), ~(p_lcd_num_pixel->bit_map));
		}
	}
	
	LCD_UpdateDisplayRequest();
}


/* 设置时间，小时和分钟 */
void lcd_time_display(uint8_t hour, uint8_t min)
{

	if ((hour >= 24) || (min >= 60)){return;}


	if (hour <= 9)
	{
		lcd_num_turnoff(1);
	}
	else
	{
		lcd_num_display(1, hour/10);
	}

	lcd_num_display(2, hour%10);
	lcd_num_display(3, min/10);
	lcd_num_display(4, min%10);
}

/* 小时分钟之间的冒号，12:30 */
void lcd_colon_config(uint8_t enable)
{
	if (enable)
	{
		LCD_Write(lcd_pixel_COL.reg_index, ~(lcd_pixel_COL.bit_map), (lcd_pixel_COL.bit_map));
	}
	else
	{
		LCD_Write(lcd_pixel_COL.reg_index, ~(lcd_pixel_COL.bit_map), ~(lcd_pixel_COL.bit_map));
	}
	LCD_UpdateDisplayRequest();
}


/* 月和日，还有中间的斜杆 */
void lcd_date_display(uint8_t mon, uint8_t day)
{
	LCD_Write(lcd_pixel_T15.reg_index, ~(lcd_pixel_T15.bit_map), (lcd_pixel_T15.bit_map));

	lcd_num_display(16, mon/10);
	lcd_num_display(15, mon%10);

	lcd_num_display(14, day/10);
	lcd_num_display(13, day%10);
}


/* 设置星期，1~7：星期一到星期日 */
void lcd_week_display(uint8_t week)
{
	if (week > 7){return;}
	
	LCD_Write(lcd_pixel_TT.reg_index, ~(lcd_pixel_TT.bit_map), (lcd_pixel_TT.bit_map));

	LCD_Write(lcd_pixel_T14.reg_index, ~(lcd_pixel_T14.bit_map), ~(lcd_pixel_T14.bit_map));
	LCD_Write(lcd_pixel_T13.reg_index, ~(lcd_pixel_T13.bit_map), ~(lcd_pixel_T13.bit_map));
	LCD_Write(lcd_pixel_T12.reg_index, ~(lcd_pixel_T12.bit_map), ~(lcd_pixel_T12.bit_map));
	LCD_Write(lcd_pixel_T11.reg_index, ~(lcd_pixel_T11.bit_map), ~(lcd_pixel_T11.bit_map));
	LCD_Write(lcd_pixel_T10.reg_index, ~(lcd_pixel_T10.bit_map), ~(lcd_pixel_T10.bit_map));
	LCD_Write(lcd_pixel_T9.reg_index, ~(lcd_pixel_T9.bit_map), ~(lcd_pixel_T9.bit_map));
	LCD_Write(lcd_pixel_T8.reg_index, ~(lcd_pixel_T8.bit_map), ~(lcd_pixel_T8.bit_map));

	switch (week)
	{
		case 1:
		{
			LCD_Write(lcd_pixel_T14.reg_index, ~(lcd_pixel_T14.bit_map), (lcd_pixel_T14.bit_map));
			break;
		}
		case 2:
		{
			LCD_Write(lcd_pixel_T13.reg_index, ~(lcd_pixel_T13.bit_map), (lcd_pixel_T13.bit_map));
			break;
		}
		case 3:
		{
			LCD_Write(lcd_pixel_T12.reg_index, ~(lcd_pixel_T12.bit_map), (lcd_pixel_T12.bit_map));
			break;
		}
		case 4:
		{
			LCD_Write(lcd_pixel_T11.reg_index, ~(lcd_pixel_T11.bit_map), (lcd_pixel_T11.bit_map));
			break;
		}
		case 5:
		{
			LCD_Write(lcd_pixel_T10.reg_index, ~(lcd_pixel_T10.bit_map), (lcd_pixel_T10.bit_map));
			break;
		}
		case 6:
		{
			LCD_Write(lcd_pixel_T9.reg_index, ~(lcd_pixel_T9.bit_map), (lcd_pixel_T9.bit_map));
			break;
		}
		case 7:
		{
			LCD_Write(lcd_pixel_T8.reg_index, ~(lcd_pixel_T8.bit_map), (lcd_pixel_T8.bit_map));
			break;
		}
		default:break;
	}
	LCD_UpdateDisplayRequest();
}


/* 温度和空气湿度，空气湿度是百分比，顺便设置温度和湿度两个图标 */
void lcd_temper_humid_display(uint8_t temper, uint8_t humid)
{
	
	LCD_Write(lcd_pixel_T3.reg_index, ~(lcd_pixel_T3.bit_map), (lcd_pixel_T3.bit_map));
	LCD_Write(lcd_pixel_T4.reg_index, ~(lcd_pixel_T4.bit_map), (lcd_pixel_T4.bit_map));

	lcd_num_display(5, temper/10);
	lcd_num_display(6, temper%10);

	lcd_num_display(8, humid/10);
	lcd_num_display(7, humid%10);
}

/* 甲醛显示，参数是μg/m³，显示是mg/m³ */
void lcd_HCHO_display(uint16_t ug_m3)
{
	LCD_Write(lcd_pixel_T7.reg_index, ~(lcd_pixel_T7.bit_map), (lcd_pixel_T7.bit_map));
	LCD_Write(lcd_pixel_T6.reg_index, ~(lcd_pixel_T6.bit_map), (lcd_pixel_T6.bit_map));
	LCD_Write(lcd_pixel_T5.reg_index, ~(lcd_pixel_T5.bit_map), (lcd_pixel_T5.bit_map));

	lcd_num_display(9, ug_m3%10);
	lcd_num_display(10, (ug_m3/10)%10);
	lcd_num_display(11, (ug_m3/100)%10);
	lcd_num_display(12, ug_m3/1000);
}

void lcd_HCHO_turnoff(void)
{
	LCD_Write(lcd_pixel_T7.reg_index, ~(lcd_pixel_T7.bit_map), ~(lcd_pixel_T7.bit_map));
	LCD_Write(lcd_pixel_T6.reg_index, ~(lcd_pixel_T6.bit_map), ~(lcd_pixel_T6.bit_map));
	LCD_Write(lcd_pixel_T5.reg_index, ~(lcd_pixel_T5.bit_map), ~(lcd_pixel_T5.bit_map));

	lcd_num_turnoff(9);
	lcd_num_turnoff(10);
	lcd_num_turnoff(11);
	lcd_num_turnoff(12);
}

/* 显示或不显示电池 */
void lcd_battery_config(uint8_t enable)
{
	if (enable)
	{
		LCD_Write(lcd_pixel_T1.reg_index, ~(lcd_pixel_T1.bit_map), (lcd_pixel_T1.bit_map));
	}
	else
	{
		LCD_Write(lcd_pixel_T1.reg_index, ~(lcd_pixel_T1.bit_map), ~(lcd_pixel_T1.bit_map));
	}
	LCD_UpdateDisplayRequest();
}

/* 显示或不显示蓝牙 */
void lcd_bluetooth_config(uint8_t enable)
{
	if (enable)
	{
		LCD_Write(lcd_pixel_T2.reg_index, ~(lcd_pixel_T2.bit_map), (lcd_pixel_T2.bit_map));
	}
	else
	{
		LCD_Write(lcd_pixel_T2.reg_index, ~(lcd_pixel_T2.bit_map), ~(lcd_pixel_T2.bit_map));
	}
	LCD_UpdateDisplayRequest();
}


/* lcd init */
void lcd_init(void)
{
    LCD_InitType Init = {0};
    /*LCD parameter config*/
    
    Init.Divider          = LCD_DIV_25;
    Init.Prescaler        = LCD_PRESCALER_4;
    Init.Duty             = LCD_DUTY_1_4;
    Init.Bias             = LCD_BIAS_1_3;
    Init.VoltageSource    = LCD_VOLTAGESOURCE_EXTERNAL;
    Init.Contrast         = LCD_CONTRASTLEVEL_5;
    Init.DeadTime         = LCD_DEADTIME_0;
    Init.PulseOnDuration  = LCD_PULSEONDURATION_1;
    Init.HighDrive        = LCD_HIGHDRIVE_DISABLE;
    Init.HighDriveBuffer  = LCD_HIGHDRIVEBUFFER_ENABLE;
    Init.BlinkMode        = LCD_BLINKMODE_OFF;
    Init.BlinkFreq        = LCD_BLINKFREQ_DIV_8;
    Init.MuxSegment       = LCD_MUXSEGMENT_DISABLE;

    /* Initialize the LCD clk and used gpio*/
    lcd_bsp_init();

    /*config and start LCD controller*/
    LCD_Init(&Init);
}

/* lcd gpio init */
static void lcd_gpio_init(void)
{
    GPIO_InitType  gpioinitstruct;

    /*Enable LCD GPIO Clocks*/
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB|RCC_APB2_PERIPH_GPIOC|RCC_APB2_PERIPH_GPIOD, ENABLE);

    /*Init GPIO init struct*/
    GPIO_InitStruct(&gpioinitstruct);

    /*Configure peripheral GPIO output for LCD*/
    /* Port A : 6 */
    /*  PA1: SEG0   PA2: SEG1   PA3: SEG2   PA6: SEG3
        PA7: SEG4   PA15: SEG17*/
    gpioinitstruct.Pin    =   GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |  GPIO_PIN_6 
                            | GPIO_PIN_7 | GPIO_PIN_15;
    gpioinitstruct.GPIO_Mode      = GPIO_Mode_Analog;
    gpioinitstruct.GPIO_Pull      = GPIO_No_Pull;
    gpioinitstruct.GPIO_Alternate = GPIO_AF10_LCD;
    GPIO_InitPeripheral(GPIOA, &gpioinitstruct);

    /* Port B 12 */
    /*  PB0: SEG5   PB1: SEG6   
				PB3:SEG7  PB4:SEG8  PB5:SEG9    
				PB10:SEG10  PB11:SEG11
        PB12:SEG12  PB13:SEG13  PB14:SEG14   
				PB15:SEG15    PB8:SEG16    
				
				*/
    gpioinitstruct.Pin    =   GPIO_PIN_0  | GPIO_PIN_1
														| GPIO_PIN_3  | GPIO_PIN_4 | GPIO_PIN_5
														| GPIO_PIN_10   | GPIO_PIN_11 
                            | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
														| GPIO_PIN_15 | GPIO_PIN_8;
    GPIO_InitPeripheral(GPIOB, &gpioinitstruct);

    /* Port C 14 */
    /*  PC0: SEG18  PC1: SEG19  PC2: SEG20  PC3: SEG21  
        PC4: SEG22  PC5: SEG23  PC6: SEG24  PC7: SEG25  
				PC8: SEG26  PC9: SEG27 
				PC10:SEG40   PC11:SEG41   PC12:SEG42 
				PC13:SEG35 
				*/
    gpioinitstruct.Pin    =   GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_3 
                            | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6  | GPIO_PIN_7
														| GPIO_PIN_8 | GPIO_PIN_9
														| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12
														| GPIO_PIN_13; 
    GPIO_InitPeripheral(GPIOC, &gpioinitstruct);


		/* PA8~PA10 : COM0~COM2 PB9:COM3 */
    gpioinitstruct.Pin    =   GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10; 
    GPIO_InitPeripheral(GPIOA, &gpioinitstruct);
		
		gpioinitstruct.Pin    =    GPIO_PIN_9; 
    GPIO_InitPeripheral(GPIOB, &gpioinitstruct);
}


static void lcd_bsp_init(void)
{
	LCD_ClockConfig(LCD_CLK_SRC_LSE);
	
	lcd_gpio_init();
}
