#include "Arduino.h"
#include <Wire.h>

#define EDOG_I2C_ADDR   (0x24)

void setup() {
    Serial.begin(9600);
    Wire.setSCL(PIN_I2C_SCL);
    Wire.setSDA(PIN_I2C_SDA);
    Wire.setClock(100000);
    Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}
