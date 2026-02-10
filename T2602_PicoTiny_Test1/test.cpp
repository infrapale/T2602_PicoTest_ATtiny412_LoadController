#include "main.h"
#include "reg.h"

void test_load_sw_on_off(void)
{
    uint8_t bm = 0;
    for(uint8_t i=0; i<10; i++)
    {
        reg_write_i2c_u8(REG_LOAD_SW, bm++);
        if(bm > 3) bm = 0;
        delay(1000);
    }
}
