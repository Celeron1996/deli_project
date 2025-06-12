#ifndef MYI2C_H
#define MYI2C_H


#include <stdio.h>
#include <stdint.h>


typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

typedef enum
{
    NONE = 0,
    TRANSMIT,
    RECEIVE
} TR_STA;

typedef enum
{
    C_READY = 0,
    C_START_BIT,
    C_STOP_BIT
}CommCtrl_t;

typedef enum
{
    MASTER_OK = 0,
    MASTER_BUSY,
    MASTER_MODE,
    MASTER_TXMODE,
    MASTER_RXMODE,
    MASTER_SENDING,
    MASTER_SENDED,
    MASTER_RECVD,
    MASTER_BYTEF,
    MASTER_BUSERR,
    MASTER_UNKNOW,
    SLAVE_OK = 20,
    SLAVE_BUSY,
    SLAVE_MODE,
    SLAVE_BUSERR,
    SLAVE_UNKNOW,

}ErrCode_t;

#define MODULE_SELF_RESET       1
#define MODULE_RCC_RESET        2
#define SYSTEM_NVIC_RESET       3
#define COMM_RECOVER_MODE       0



int i2c_master_init(void);
int i2c_master_send(uint8_t* data, int len, uint8_t slave_addr);
int i2c_master_recv(uint8_t* data, int len, uint8_t slave_addr);
int i2c_master_read_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_len);
int i2c_master_write_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_len);



#endif
