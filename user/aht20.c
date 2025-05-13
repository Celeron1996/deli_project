
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
	printf("aht20 init!\r\n");
	
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

	if ((system_get_tick_cnt_ms() - aht20_tick) > 1000)
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

			lcd_temper_humid_display((uint8_t)tempa, (uint8_t)RH);
		}

		aht20_tick = system_get_tick_cnt_ms();
	}
}




