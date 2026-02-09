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
