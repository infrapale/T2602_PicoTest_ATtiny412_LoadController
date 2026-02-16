#include "main.h"
#include "io.h"
#include "reg.h"
#include "atask.h"


void test_task(void);
//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st tth                  =   {"Test Task      ", 1000,     0,     0,  255,    0,  1,  test_task };
uint8_t     tt_indx;
uint32_t    tt_timeout;
uint8_t     bm = 0;
uint8_t     i = 0;

void test_initialize(void)
{
    tt_indx =  atask_add_new(&tth);
    tth.state = 100;
}

void test_task(void)
{
    switch(tth.state)
    {
        case 0:
            bm = 0;
            io_led_flash(COLOR_RED, BLINK_OFF);
            io_led_flash(COLOR_YELLOW, BLINK_OFF);
            io_led_flash(COLOR_BLUE, BLINK_OFF);
            tth.state = 10;
            break;
        case 10:
            i = 0;
            tt_timeout = millis() + 10000;
            io_led_flash(COLOR_BLUE, BLINK_FAST_FLASH);
            bm=0;
            reg_write_i2c_u8(REG_LOAD_SW, bm);
            tth.state = 12;
            break;
        case 12:
            if(millis() > tt_timeout) tth.state = 20; 
            break;
        case 20:    
            reg_write_i2c_u8(REG_LOAD_SW, bm++);
            if(bm > 3) bm = 0;
            tt_timeout = millis() + 5000;
            tth.state = 30;
            break;
        case 30:
            if(millis() > tt_timeout) tth.state = 20;
            break;
        case 100:
            io_led_flash(COLOR_RED, BLINK_ON);
            io_led_flash(COLOR_YELLOW, BLINK_OFF);
            io_led_flash(COLOR_BLUE, BLINK_OFF); 
            tt_timeout = millis() + 2000;   
            tth.state = 110;
            break;
        case 110:
            if(millis() > tt_timeout){
                tt_timeout = millis() + 1000;   
                tth.state = 120;
            } 
            break;
        case 120:
            io_led_flash(COLOR_YELLOW, BLINK_FAST_BLINK); 
            reg_write_i2c_u32(REG_SLEEP_TIME, 2000); 
            tt_timeout = millis() + 1000;   
            tth.state = 130;   
            break;
        case 130:
            if(millis() > tt_timeout){
                reg_write_i2c_u8(REG_SLEEP_STATE, SLEEP_ACTIVATED); 
                io_led_flash(COLOR_BLUE, BLINK_FAST_BLINK); 
                tth.state = 140;   
            }    
            break;
        case 140:
            io_led_flash(COLOR_RED, BLINK_FAST_BLINK); 
            break;    


    }
}
