
#include "aht20.h"
#include "myi2c.h"
#include "lcd.h"
#include "system.h"


void aht20_delay(uint32_t nCount)
{
    uint32_t tcnt;
    while(nCount--)
    {
        tcnt = 64000 / 9;
        while (tcnt--){;}
    }
}



void aht20_init(void)
{
	aht20_delay(200);

	/* 软复位 */
	uint8_t data_buf = 0xBA;	// 软复位命令
	if (i2c_master_send(&data_buf, 1, AHT20_SLAVE_ADDR_7BIT) != 0)
	{
		printf("aht20 soft reset error!\r\n");
		while (1);
	}

	aht20_delay(200);

	/* 初始化，校准 */
	uint8_t init_buffer[3] = {0xBE, 0x08, 0x00};
	if (i2c_master_send(init_buffer, 3, AHT20_SLAVE_ADDR_7BIT) != 0)
	{
		printf("aht20 send init data error!\r\n");
		while (1);
	}

	aht20_delay(200);
	
	printf("aht20 init success!\r\n");
}


 /*******************************************************************************
* Function Name  : CheckCrc
* Description    :  
* Input          :  None
* Output         : None
* Return         :None
*******************************************************************************/
unsigned char  CheckCrc8(unsigned char *pDat,unsigned char Lenth)
{
unsigned char crc = 0xff, i, j;

    for (i = 0; i < Lenth ; i++)
    {
        crc = crc ^ *pDat;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc <<= 1;
        }
				pDat++;
    }
    return crc;
}

void aht20_test(void)
{
	uint8_t send_buffer[3] = {0xAC, 0x33, 0x00};
	uint8_t read_buffer[7];
	float RH;
	float tempa;
	uint32_t s32x;
	
	aht20_init();
	
	while (1)
	{
		i2c_master_send(send_buffer, 3, AHT20_SLAVE_ADDR_7BIT);

		aht20_delay(200);
		
		i2c_master_recv(read_buffer, 7, AHT20_SLAVE_ADDR_7BIT);
		
		if((CheckCrc8(&read_buffer[0],6)==read_buffer[6])&&((read_buffer[0]&0x98) == 0x18))
		{
			s32x=read_buffer[1];s32x=s32x<<8;s32x+=read_buffer[2];s32x=s32x<<8;s32x+=read_buffer[3];s32x=s32x>>4;				
			RH=s32x;
			RH=RH*100/1048576;
			s32x=read_buffer[3]&0x0F;s32x=s32x<<8;s32x+=read_buffer[4];s32x=s32x<<8;s32x+=read_buffer[5];		
			tempa=s32x;
			tempa=tempa*200/1048576-50;
			
		 printf("RH = %0.2f",RH);
		 printf("T = %0.2f\n",tempa);
		}
		else{
			printf("read error!\r\n");
		}
		
		aht20_delay(1000);
	}
}




void aht20_process(void)
{
	static tick_type aht20_tick = 0;
	uint8_t send_buffer[3] = {0xAC, 0x33, 0x00};
	uint8_t read_buffer[7];
	float RH;
	float tempa;
	uint32_t s32x;
	uint16_t u16_RH;
	uint16_t u16_tempa;

	if ((system_get_tick_cnt_ms() - aht20_tick) > 1000)
	{
		i2c_master_send(send_buffer, 3, AHT20_SLAVE_ADDR_7BIT);

		aht20_delay(120);

		i2c_master_recv(read_buffer, 7, AHT20_SLAVE_ADDR_7BIT);

		if((CheckCrc8(&read_buffer[0],6)==read_buffer[6])&&((read_buffer[0]&0x98) == 0x18))
		{
			s32x=read_buffer[1];s32x=s32x<<8;s32x+=read_buffer[2];s32x=s32x<<8;s32x+=read_buffer[3];s32x=s32x>>4;				
			RH=s32x;
			RH=RH*100/1048576;
			s32x=read_buffer[3]&0x0F;s32x=s32x<<8;s32x+=read_buffer[4];s32x=s32x<<8;s32x+=read_buffer[5];		
			tempa=s32x;
			tempa=tempa*200/1048576-50;

			printf("RH = %0.2f",RH);
			printf("T = %0.2f\n",tempa);

			/* 四舍五入 */
			u16_tempa = (uint16_t)(tempa*10);
			u16_RH = (uint16_t)(RH*10);

			if ((u16_tempa % 10) >= 5){
				u16_tempa = (u16_tempa/10) + 1;
			}
			else{
				u16_tempa = (u16_tempa/10);
			}

			if ((u16_RH % 10) >= 5){
				u16_RH = (u16_RH/10) + 1;
			}
			else{
				u16_RH = (u16_RH/10);
			}
		 	
			lcd_temper_humid_display((uint8_t)u16_tempa, (uint8_t)u16_RH);
		}

		aht20_tick = system_get_tick_cnt_ms();
	}
}


