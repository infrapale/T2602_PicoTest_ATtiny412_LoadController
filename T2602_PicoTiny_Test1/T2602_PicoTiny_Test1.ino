#include "Arduino.h"
#include <Wire.h>
#include "main.h"
#include "atask.h"
#include "io.h"
#include "reg.h"
#include "test.h"

#define EDOG_I2C_ADDR   (0x24)

void debug_print_task(void) {atask_print_status(true);}

//                                  123456789012345      ival  next  state  prev  cntr flag  call backup
atask_st debug_print_handle      = {"Debug Print    ",   1000, 0,    0,     255,  0,   1, debug_print_task};

void setup() {
    Serial.begin(9600);
    Wire.setSCL(PIN_I2C0_SCL);
    Wire.setSDA(PIN_I2C0_SDA);
    Wire.setClock(100000);
    Wire.begin();

    atask_initialize();
    atask_add_new(&debug_print_handle);
    reg_initialize();

    io_initialize();
    io_task_initialize();
    io_led_flash(COLOR_RED, BLINK_SHORT_FLASH);
    io_led_flash(COLOR_YELLOW, BLINK_BLINK);
    io_led_flash(COLOR_BLUE, BLINK_FAST_FLASH);

    test_initialize();

}

void loop() {
    atask_run();
}
