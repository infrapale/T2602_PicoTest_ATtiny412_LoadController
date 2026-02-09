#include "main.h"

void reg_i2c_wr_u8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(reg);      // register address
    Wire.write(value);    // data byte
    Wire.endTransmission();
}

//i2cWriteReg(0x10, 0x7F);

uint8_t reg_i2c_rd_u8(uint8_t reg) {
    Wire.beginTransmission(LOAD_CTRL_ADDR);
    Wire.write(reg);              // register address
    Wire.endTransmission(false);  // send repeated start

    Wire.requestFrom(LOAD_CTRL_ADDR, 1);
    return Wire.read();
}

//uint8_t status = i2cReadReg(0x01);


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