#ifndef SD8568_H
#define SD8568_H




typedef enum{
	SD8568_EXIT_OK = 0,
	SD8568_EXIT_NO_ACK = -1,
	SD8568_ERR = -2,
}sd8568_exit_code_e;

typedef enum{
	CENTURY_20 = 0,
	CENTURY_19 = 1,
}century_type_e;
	
typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char week;
	unsigned char day;
	unsigned char month;
	unsigned char year;	
	century_type_e century_type;
}sd8568_time_t;

typedef enum{
	MINUTE_ALARM_ENABLE = 0,  
	MINUTE_ALARM_DISABLE,
	HOUR_ALARM_ENABLE,
	HOUR_ALARM_DISABLE,
	DAY_ALARM_ENABLE,
	DAY_ALARM_DISABLE,
	WEEK_ALARM_ENABLE,
	WEEK_ALARM_DISABLE,
	ALL_ALARM_DISABLE
}alaem_enable_e;  

typedef struct{
	unsigned char minute_a;
	unsigned char hour_a;
	unsigned char day_a;
	unsigned char week_a;	
	alaem_enable_e state; 
}sd8568_time_alarm_t;

typedef enum{
	CTR2_IM = 0,
	CTR2_INTAF,
	CTR2_INTDF,
	CTR2_INTAE,
	CTR2_INTDE
}ctr2_flag_e; 

typedef enum{
	SD8568_DISABLE = 0,
	SD8568_ENABLE = 1,
}flag_enable_e; 

typedef enum{
	OSF_L = 0,
	OSF_H = 1,
}sd8568_osf_flag_e; 

typedef enum{
	CLK_OUT_32KHZ  = 0x00,
	CLK_OUT_1024HZ = 0x01,
	CLK_OUT_32HZ   = 0x02,
	CLK_OUT_1HZ    = 0x03,
}clk_out_e;

typedef enum{
	COUNT_DOWN_4096HZ = 0x00,
	COUNT_DOWN_64HZ   = 0x01,
	COUNT_DOWN_1HZ    = 0x02,
	COUNT_DOWN_1_60HZ = 0x03,
}count_down_e;

sd8568_exit_code_e sd8568_read_reg(unsigned char reg_addr, unsigned char *read_buf, unsigned char length);
sd8568_exit_code_e sd8568_write_reg(unsigned char reg_addr, unsigned char *write_buf, unsigned char length);

sd8568_exit_code_e sd8568_read_time(sd8568_time_t *time);
sd8568_exit_code_e sd8568_write_time(sd8568_time_t *time);

sd8568_exit_code_e sd8568_get_ctr2_flag(ctr2_flag_e flag, flag_enable_e *enable);
sd8568_exit_code_e sd8568_set_ctr2_flag(ctr2_flag_e flag, flag_enable_e enable);

sd8568_exit_code_e sd8568_get_osf_flag(sd8568_osf_flag_e *osf_flag);
sd8568_exit_code_e sd8568_clear_osf_flag(void);

sd8568_exit_code_e sd8568_set_clk_out(clk_out_e freq, flag_enable_e enable);
sd8568_exit_code_e sd8568_set_count_down_vaule(unsigned char vaule);
sd8568_exit_code_e sd8568_get_count_down_vaule(unsigned char *vaule);
sd8568_exit_code_e sd8568_set_count_down(count_down_e freq, flag_enable_e enable);




#endif
