#ifndef LCD_H
#define LCD_H


#include "n32l40x.h"
#include "stdint.h"


/* LCD显存寄存器编号 */
typedef enum {
	lcd_reg_ram1_com0			= (0x00000000U),
	lcd_reg_ram2_com0			= (0x00000001U),
	lcd_reg_ram1_com1			= (0x00000002U),
	lcd_reg_ram2_com1			= (0x00000003U),
	lcd_reg_ram1_com2			= (0x00000004U),
	lcd_reg_ram2_com2			= (0x00000005U),
	lcd_reg_ram1_com3			= (0x00000006U),
	lcd_reg_ram2_com3			= (0x00000007U),
	lcd_reg_ram1_com4			= (0x00000008U),
	lcd_reg_ram2_com4			= (0x00000009U),
	lcd_reg_ram1_com5			= (0x0000000AU),
	lcd_reg_ram2_com5			= (0x0000000BU),
	lcd_reg_ram1_com6			= (0x0000000CU),
	lcd_reg_ram2_com6			= (0x0000000DU),
	lcd_reg_ram1_com7			= (0x0000000EU),
	lcd_reg_ram2_com7			= (0x0000000FU)
} lcd_register_index;


/* 寄存器每个位定义 */
#define LCD_BIT0 ((uint32_t)0x00000001) /*!< 0 */
#define LCD_BIT1 ((uint32_t)0x00000002) /*!< 1 */
#define LCD_BIT2 ((uint32_t)0x00000004) /*!< 2 */
#define LCD_BIT3 ((uint32_t)0x00000008) /*!< 3 */
#define LCD_BIT4 ((uint32_t)0x00000010) /*!< 4 */
#define LCD_BIT5 ((uint32_t)0x00000020) /*!< 5 */
#define LCD_BIT6 ((uint32_t)0x00000040) /*!< 6 */
#define LCD_BIT7 ((uint32_t)0x00000080) /*!< 7 */
#define LCD_BIT8 ((uint32_t)0x00000100) /*!< 8 */
#define LCD_BIT9 ((uint32_t)0x00000200) /*!< 9 */
#define LCD_BIT10 ((uint32_t)0x00000400) /*!< 10 */
#define LCD_BIT11 ((uint32_t)0x00000800) /*!< 11 */
#define LCD_BIT12 ((uint32_t)0x00001000) /*!< 12 */
#define LCD_BIT13 ((uint32_t)0x00002000) /*!< 13 */
#define LCD_BIT14 ((uint32_t)0x00004000) /*!< 14 */
#define LCD_BIT15 ((uint32_t)0x00008000) /*!< 15 */
#define LCD_BIT16 ((uint32_t)0x00010000) /*!< 16 */
#define LCD_BIT17 ((uint32_t)0x00020000) /*!< 17 */
#define LCD_BIT18 ((uint32_t)0x00040000) /*!< 18 */
#define LCD_BIT19 ((uint32_t)0x00080000) /*!< 19 */
#define LCD_BIT20 ((uint32_t)0x00100000) /*!< 20 */
#define LCD_BIT21 ((uint32_t)0x00200000) /*!< 21 */
#define LCD_BIT22 ((uint32_t)0x00400000) /*!< 22 */
#define LCD_BIT23 ((uint32_t)0x00800000) /*!< 23 */
#define LCD_BIT24 ((uint32_t)0x01000000) /*!< 24 */
#define LCD_BIT25 ((uint32_t)0x02000000) /*!< 25 */
#define LCD_BIT26 ((uint32_t)0x04000000) /*!< 26 */
#define LCD_BIT27 ((uint32_t)0x08000000) /*!< 27 */
#define LCD_BIT28 ((uint32_t)0x10000000) /*!< 24 */
#define LCD_BIT29 ((uint32_t)0x20000000) /*!< 25 */
#define LCD_BIT30 ((uint32_t)0x40000000) /*!< 26 */
#define LCD_BIT31 ((uint32_t)0x80000000) /*!< 27 */


/* 一个像素信息，包含哪个寄存器，寄存器中的哪个位 */
typedef struct {
	lcd_register_index reg_index;
	uint32_t bit_map;
} lcd_pixel_typdef;



void lcd_num_display(uint8_t num_index, uint8_t num_dis);
void lcd_num_turnoff(uint8_t num_index);
void lcd_time_display(uint8_t hour, uint8_t min);
void lcd_colon_config(uint8_t enable);
void lcd_date_display(uint8_t mon, uint8_t day);
void lcd_week_display(uint8_t week);
void lcd_temper_humid_display(uint8_t temper, uint8_t humid);
void lcd_HCHO_display(uint16_t ug_m3);
void lcd_HCHO_turnoff(void);
void lcd_battery_config(uint8_t enable);
void lcd_bluetooth_config(uint8_t enable);
void lcd_init(void);


#endif
