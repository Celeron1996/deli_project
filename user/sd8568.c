#include <sd8568.h>
#include "myi2c.h"


/*** i2c defined ****/
typedef enum
{
	I2C_OK = 0,     			
	I2C_START_FAIL,								
	I2C_NO_ACK					
}i2c_exit_code_e;

i2c_exit_code_e i2c_write_multi_bytes(unsigned char dev_addr, unsigned char reg_addr, unsigned char *buf, unsigned char length);
i2c_exit_code_e i2c_read_multi_bytes(unsigned char dev_addr,unsigned char reg_addr, unsigned char *buf, unsigned char length);
/********************************************************/

#define SD8568_DEV_ADDR 			 0xA2   //sd8568 addr

#define SD8568_REG_CTR1 			 0x00
#define SD8568_REG_CTR2 			 0x01

#define SD8568_REG_CLKOUT	    	 0x0D
#define SD8568_REG_TIMER_CTR		 0x0E
#define SD8568_REG_COUNT_DOWN  		 0x0F

#define SD8568_REG_SEC 		    	 0x02
#define SD8568_REG_MIN				 0x03
#define SD8568_REG_HOUR			     0x04
#define SD8568_REG_DAY				 0x05
#define SD8568_REG_WEEK			     0x06
#define SD8568_REG_MONTH		     0x07
#define SD8568_REG_YEAR			     0x08

#define SD8568_REG_ALARM_MIN         0x09
#define SD8568_REG_ALARM_HOUR        0x0A
#define SD8568_REG_ALARM_DAY	     0x0B
#define SD8568_REG_ALARM_WEEK        0x0C

#define BIT(x) (1 << x)
/*CTR1*/
#define SD8568_TEST_MODE      		BIT(7)
#define SD8568_PROTECT         		BIT(6)
#define SD8568_CLK_STOP			    BIT(5)
#define SD8568_TESTC           		BIT(3)
/*CTR2*/
#define SD8568_IM					BIT(4)
#define SD8568_INTAF				BIT(3)
#define SD8568_INTDF				BIT(2)
#define SD8568_INTAE				BIT(1)
#define SD8568_INTDE				BIT(0)
/*CLKOUT*/
#define SD8568_FE					BIT(7)
/*COUNT DOWN*/
#define SD8568_DE					BIT(7)

#define SD8568_OSF				BIT(7)
#define SD8568_CENTURY				BIT(7)
#define SD8568_MIN_AL				BIT(7)
#define SD8568_HOUR_AL				BIT(7)
#define SD8568_DAY_AL				BIT(7)
#define SD8568_WEEK_AL				BIT(7)


/**
 * @brief read time hex to bcd
 */
static unsigned char bcd_hex_convert(unsigned char bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0f);
}

/**
 * @brief write time bcd to hex
 */
static unsigned char hex_bcd_convert(unsigned char hex)
{
	return ((hex / 10) << 4) | (hex % 10);
}

/**
 * @brief write protect enable()   close protect
 */
sd8568_exit_code_e sd8568_write_enable()
{
	unsigned char buf;
	
	if(i2c_read_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;
	}
	buf &= ~0x7C; 
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;
	}
	buf |= 0x70;	
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}
	buf &= ~0x7C;
	buf |=  0x0C; 
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}
	buf &= ~0x7C;
	buf |=  0x38;  	 
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}	
	return SD8568_EXIT_OK;
}

/**
 * @brief write protect disable   open protect
 */
sd8568_exit_code_e sd8568_write_disable()
{
	unsigned char buf;
	
	if(i2c_read_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;
	}
	buf &= ~0x7C; 
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}
	buf |= 0x54;	
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}
	buf &= ~0x7C;
	buf |=  0x28;  
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}
	buf &= ~0x7C;
	buf |=  0x5C;   
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, SD8568_REG_TIMER_CTR, &buf, 1) != I2C_OK)
	{
		return SD8568_ERR;	
	}		
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568_read_reg
 */
sd8568_exit_code_e sd8568_read_reg(unsigned char reg_addr, unsigned char *read_buf, unsigned char length)
{
	if(i2c_read_multi_bytes(SD8568_DEV_ADDR, reg_addr, read_buf, length) != I2C_OK)
		return SD8568_EXIT_NO_ACK;
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568_write_reg
 */
sd8568_exit_code_e sd8568_write_reg(unsigned char reg_addr, unsigned char *write_buf, unsigned char length)
{  
	sd8568_write_enable();
	if(i2c_write_multi_bytes(SD8568_DEV_ADDR, reg_addr, write_buf, length) != I2C_OK)
	{
		return SD8568_EXIT_NO_ACK;
	}
	sd8568_write_disable();
	return SD8568_EXIT_OK;
}


/**
 * @brief sd8568 read time
 */
sd8568_exit_code_e sd8568_read_time(sd8568_time_t *time)
{
	unsigned char buf[7];
	
	if(sd8568_read_reg(SD8568_REG_SEC, buf, 7) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	time->second = bcd_hex_convert(buf[0] & 0x7f);
	time->minute = bcd_hex_convert(buf[1] & 0x7f);
	time->hour   = bcd_hex_convert(buf[2] & 0x3f);
	time->day    = bcd_hex_convert(buf[3] & 0x3f);
	time->week   = bcd_hex_convert(buf[4] & 0x07);
	time->month  = bcd_hex_convert(buf[5] & 0x1f);
	if(buf[5] & 0x80)   
	{
		time->century_type =CENTURY_19;
	}
	else
	{
		time->century_type = CENTURY_20;
	}
	time->year   = bcd_hex_convert(buf[6] & 0xff);
	
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 write time
 */
sd8568_exit_code_e sd8568_write_time(sd8568_time_t *time)
{
	unsigned char buf[7];
	
	buf[0] = hex_bcd_convert(time->second);
	buf[1] = hex_bcd_convert(time->minute);
	buf[2] = hex_bcd_convert(time->hour  );
	buf[3] = hex_bcd_convert(time->day   );
	buf[4] = hex_bcd_convert(time->week  );			
	buf[5] = hex_bcd_convert(time->month ); 
	if(time->century_type == CENTURY_19)     
	{
		buf[5] |=  SD8568_CENTURY;
	}
	else
	{
		buf[5] &=  ~SD8568_CENTURY;
	}		
	buf[6] = hex_bcd_convert(time->year);
	
	if(sd8568_write_reg(SD8568_REG_SEC, buf, 7) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 get ctr2 flag state
 */
sd8568_exit_code_e sd8568_get_ctr2_flag(ctr2_flag_e flag, flag_enable_e *enable)
{	
	unsigned char buf;
	
	if(sd8568_read_reg(SD8568_REG_CTR2, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	switch(flag)
	{
		case CTR2_IM:
			buf &= 0x10;
		break;
		case CTR2_INTAF:
			buf &= 0x08;
		break;
		case CTR2_INTDF:
			buf &= 0x04;
		break;
		case CTR2_INTAE:
			buf &= 0x02;
		break;
		case CTR2_INTDE:
			buf &= 0x01;
		break;
		default: 
			return SD8568_ERR;		
	}
	if(buf)
		*enable = SD8568_ENABLE;
	else
		*enable = SD8568_DISABLE;
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 set ctr2 flag
 */
sd8568_exit_code_e sd8568_set_ctr2_flag(ctr2_flag_e flag, flag_enable_e enable)
{
	unsigned char buf;

	if(sd8568_read_reg(SD8568_REG_CTR2, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	if(enable == SD8568_ENABLE)
	{
		switch(flag)
		{
			case CTR2_IM:
				buf |= 0x10;
			break;
			case CTR2_INTAF:
				buf |= 0x08;
			break;
			case CTR2_INTDF:
				buf |= 0x04;
			break;
			case CTR2_INTAE:
				buf |= 0x02;
			break;
			case CTR2_INTDE:
				buf |= 0x01;
			break;
			default: 
				return SD8568_ERR;		
		}
	}
	else
	{
		switch(flag)
		{
			case CTR2_IM:
				buf &= ~0x10;
			break;
			case CTR2_INTAF:
				buf &= ~0x08;
			break;
			case CTR2_INTDF:
				buf &= ~0x04;
			break;
			case CTR2_INTAE:
				buf &= ~0x02;
			break;
			case CTR2_INTDE:
				buf &= ~0x01;
			break;
			default: 
				return SD8568_ERR;		
		}
	}
	
	if(sd8568_write_reg(SD8568_REG_CTR2, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 get osf bit
 */
sd8568_exit_code_e sd8568_get_osf_flag(sd8568_osf_flag_e *osf_flag)
{
	unsigned char buf;
	
	if(sd8568_read_reg(SD8568_REG_SEC, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	if(buf & SD8568_OSF)
	{
		*osf_flag = OSF_H;
	}
	else
	{
		*osf_flag = OSF_L;
	}
	
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 clear osf bit
 */
sd8568_exit_code_e sd8568_clear_osf_flag(void)
{
	unsigned char buf;
	
	if(sd8568_read_reg(SD8568_REG_SEC, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	buf &= ~SD8568_OSF;
	
	if(sd8568_write_reg(SD8568_REG_SEC, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 set clkout
 */
sd8568_exit_code_e sd8568_set_clk_out(clk_out_e freq, flag_enable_e enable)
{
	unsigned char buf;
	
	switch(freq)
	{
		case CLK_OUT_32KHZ:
			buf &= 0xfc;
			buf |= CLK_OUT_32KHZ;
		break;
		case CLK_OUT_1024HZ:
			buf &= 0xfc;
			buf |= CLK_OUT_1024HZ;
		break;
		case CLK_OUT_32HZ:
			buf &= 0xfc;
			buf |= CLK_OUT_32HZ;
		break;
		case CLK_OUT_1HZ:
			buf &= 0xfc;
			buf |= CLK_OUT_1HZ;
		break;
		default: 
			return SD8568_ERR;			
	}
	
	if(enable == SD8568_ENABLE)
	{
		buf |= SD8568_FE;
	}
	else
	{
		buf &= ~SD8568_FE;
	}
	
	if(sd8568_write_reg(SD8568_REG_CLKOUT, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 set count dowm vaule
 */
sd8568_exit_code_e sd8568_set_count_down_vaule(unsigned char vaule)
{
	if(vaule < 0 || vaule > 0xff)
	{
		return SD8568_ERR;
	}
	if(sd8568_write_reg(SD8568_REG_COUNT_DOWN, &vaule, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 get count dowm vaule
 */
sd8568_exit_code_e sd8568_get_count_down_vaule(unsigned char *vaule)
{
	unsigned char buf;
	
	if(sd8568_read_reg(SD8568_REG_COUNT_DOWN, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	*vaule = buf;
	return SD8568_EXIT_OK;
}

/**
 * @brief sd8568 set count dowm
 */
sd8568_exit_code_e sd8568_set_count_down(count_down_e freq, flag_enable_e enable)
{
	unsigned char buf;
	
	switch(freq)
	{
		case COUNT_DOWN_4096HZ:
			buf &= 0xfc;  
			buf |= COUNT_DOWN_4096HZ;
		break;
		case COUNT_DOWN_64HZ:
			buf &= 0xfc;
			buf |= COUNT_DOWN_64HZ;
		break;
		case COUNT_DOWN_1HZ:
			buf &= 0xfc;
			buf |= COUNT_DOWN_1HZ;
		break;
		case COUNT_DOWN_1_60HZ:
			buf &= 0xfc;
			buf |= COUNT_DOWN_1_60HZ;
		break;
		default: 
			return SD8568_ERR;			
	}
	
	if(enable == SD8568_ENABLE)
	{
		buf |= SD8568_DE;
	}
	else
	{
		buf &= ~SD8568_DE;
	}
	
	if(sd8568_write_reg(SD8568_REG_TIMER_CTR, &buf, 1) != SD8568_EXIT_OK)
	{
		return SD8568_ERR;
	}
	
	return SD8568_EXIT_OK;
}



i2c_exit_code_e i2c_write_multi_bytes(unsigned char dev_addr, unsigned char reg_addr, unsigned char *buf, unsigned char length)
{
	return i2c_master_write_reg(dev_addr,  reg_addr, buf, length);
}
i2c_exit_code_e i2c_read_multi_bytes(unsigned char dev_addr,unsigned char reg_addr, unsigned char *buf, unsigned char length)
{
	return i2c_master_read_reg(dev_addr,  reg_addr, buf, length);
}

