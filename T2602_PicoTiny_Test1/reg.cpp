#include <Wire.h>
#include "main.h"
#include "io.h"
#include "reg.h"

void reg_i2c_wr_u8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(reg);      // register address
    Wire.write(value);    // data byte
    Wire.endTransmission();
}

uint8_t reg_bulk[REG_LEN];

void reg_initialize(void)
{

}

uint8_t reg_get_item_len(uint8_t reg_addr)
{   uint8_t len = 0;
    switch(reg_addr)
    {
        case REG_WD_INTERVAL:   len = 4; break;
        case REG_SLEEP_TIME:    len = 4; break;
        case REG_WD_OFF_TIME:   len = 4; break;
        case REG_LOAD_SW:       len = 1; break;
        case REG_EEPROM_STATE:  len = 1; break;
        case REG_EEPROM_ADDR:   len = 1; break;
        case REG_SLEEP_STATE:   len = 1; break;
        case REG_CLR_WD:        len = 1; break;
        case REG_RESERVED_1:    len = 0; break;
        case REG_EEPROM_BUFF:   len = 16; break;
    }
    return len;
}

uint8_t reg_i2c_rd_u8(uint8_t reg) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(reg);              // register address
    Wire.endTransmission(false);  // send repeated start

    Wire.requestFrom(LOAD_CTRL_ADDR, 1);
    return Wire.read();
}

void reg_i2c_wr_data(uint8_t startReg, const uint8_t *data, size_t len) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(startReg);
    for (size_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

void reg_i2c_rd_data(uint8_t startReg, uint8_t *buffer, size_t len) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(startReg);
    Wire.endTransmission(false);  // repeated start

    Wire.requestFrom(LOAD_CTRL_ADDR, len);
    for (size_t i = 0; i < len; i++) {
        buffer[i] = Wire.read();
    }
}

void reg_read_i2c(uint8_t reg_addr )
{
    uint8_t len = reg_get_item_len(reg_addr);
    reg_i2c_rd_data(reg_addr, &reg_bulk[reg_addr],len);
}

void reg_write_i2c(uint8_t reg_addr)
{
    uint8_t len = reg_get_item_len(reg_addr);
    reg_i2c_wr_data(reg_addr, &reg_bulk[reg_addr],len);
}

uint32_t reg_read_i2c_u32(uint8_t reg_addr)
{
    reg_read_i2c(reg_addr);
    return reg_read_u32(reg_addr);
}
void reg_write_i2c_u32(uint8_t reg_addr, uint32_t u32 )
{
    reg_write_u32(reg_addr, u32);
    reg_write_i2c(reg_addr);
}

uint8_t reg_read_i2c_u8(uint8_t reg_addr)
{
    reg_read_i2c(reg_addr);
    return reg_read_u8(reg_addr);
}
void reg_write_i2c_u8(uint8_t reg_addr, uint8_t u8 )
{
    reg_write_u8(reg_addr, u8);
    reg_write_i2c(reg_addr);
}


