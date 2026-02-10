#include "Arduino.h"
#include <Wire.h>
#include "io.h"
#include "reg.h"
#include "test.h"

#define EDOG_I2C_ADDR   (0x24)

void setup() {
    Serial.begin(9600);
    Wire.setSCL(PIN_I2C0_SCL);
    Wire.setSDA(PIN_I2C0_SDA);
    Wire.setClock(100000);
    Wire.begin();

    reg_initialize();

    test_load_sw_on_off();

}

void loop() {
  // put your main code here, to run repeatedly:



}
