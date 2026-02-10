#ifndef __REG_H__
#define __REG_H__
#include "main.h"


#define  REG_LEN                64
#define  REG_EEPROM_BUFF_LEN    16

extern uint8_t reg_bulk[];

typedef enum
{
    REG_WD_INTERVAL     = 0,
    REG_SLEEP_TIME      = REG_WD_INTERVAL + 4,
    REG_WD_OFF_TIME     = REG_SLEEP_TIME + 4,
    REG_LOAD_SW         = REG_WD_OFF_TIME + 4,
    REG_EEPROM_STATE    = REG_LOAD_SW + 1,
    REG_EEPROM_ADDR     = REG_EEPROM_STATE + 1,
    REG_SLEEP_STATE     = REG_EEPROM_ADDR + 1,
    REG_CLR_WD          = REG_SLEEP_STATE + 1,
    REG_RESERVED_1      = REG_CLR_WD + 1,
    REG_EEPROM_BUFF     = REG_RESERVED_1 + 15
} reg_data_et;

void reg_initialize(void);


// ---------------------- inline -----------------------------
static inline void reg_write_u8(uint8_t offset, uint8_t value)
{
    reg_bulk[offset] = value;
}
static inline uint8_t reg_read_u8(uint8_t offset)
{
    return reg_bulk[offset];
}
static inline void reg_write_u16(uint8_t offset, uint16_t value)
{
    reg_bulk[offset + 0] = (value >> 0) & 0xFF;
    reg_bulk[offset + 1] = (value >> 8) & 0xFF;
}
static inline uint16_t reg_read_u16(uint8_t offset)
{
    return ((uint16_t)reg_bulk[offset + 0] << 0) |
           ((uint16_t)reg_bulk[offset + 1] << 8);
}

static inline void reg_write_u32(uint8_t offset, uint32_t value)
{
    reg_bulk[offset + 0] = (value >> 0) & 0xFF;
    reg_bulk[offset + 1] = (value >> 8) & 0xFF;
    reg_bulk[offset + 2] = (value >> 16) & 0xFF;
    reg_bulk[offset + 3] = (value >> 24) & 0xFF;
}

static inline uint32_t reg_read_u32(uint8_t offset)
{
    return ((uint32_t)reg_bulk[offset + 0] << 0)  |
           ((uint32_t)reg_bulk[offset + 1] << 8)  |
           ((uint32_t)reg_bulk[offset + 2] << 16) |
           ((uint32_t)reg_bulk[offset + 3] << 24);
}

//----------------------------------------------------------

uint8_t reg_get_item_len(uint8_t reg_addr); 

uint8_t reg_i2c_rd_u8(uint8_t reg);

void reg_i2c_wr_data(uint8_t startReg, const uint8_t *data, size_t len);

void reg_i2c_rd_data(uint8_t startReg, uint8_t *buffer, size_t len);

void reg_read_i2c(uint8_t reg_addr);

void reg_write_i2c(uint8_t reg_addr);

uint32_t reg_read_i2c_u32(uint8_t reg_addr);

void reg_write_i2c_u32(uint8_t reg_addr, uint32_t u32);

uint8_t reg_read_i2c_u8(uint8_t reg_addr);

void reg_write_i2c_u8(uint8_t reg_addr, uint8_t u8);



#endif