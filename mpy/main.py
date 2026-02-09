'''
Test Code for the ATTiny412 Watchdog "edog"
https://github.com/infrapale/T2408_ATtiny412_edog

'''
from machine import I2C
import sys
import time
import reg

#  https://www.fredscave.com/44-micropython-data-types-bytes.html

wd_clr_pin = machine.Pin(14, machine.Pin.OUT);

pin_pwr1_off    = machine.Pin(18, machine.Pin.IN);
pin_pwr2_off    = machine.Pin(21, machine.Pin.IN);
pin_pa3         = machine.Pin(20, machine.Pin.IN);
pin_pa6         = machine.Pin(16, machine.Pin.IN);
pin_pa7         = machine.Pin(17, machine.Pin.IN);




EDOG_ADDR = 0x24
sdaPIN=machine.Pin(4)
sclPIN=machine.Pin(5)
i2c=I2C(0,sda=sdaPIN, scl=sclPIN, freq=400000)
       # create I2C peripheral at frequency of 400kHz
                                # depending on the port, extra parameters may be required                                # to select the peripheral and/or pins to use

#  i2c.scan()                      # scan for peripherals, returning a list of 7-bit addresses
# bytes rxdata

def get_hex_arr_str(arr):
    return (''.join("0x%02x " % i for i in arr))

def i2c_wr_u8(addr, u8):
    barr = u8.to_bytes(1,"big")
    i2c.writeto_mem(EDOG_ADDR, addr, barr)
    #print("i2c_wr_u8: ", get_hex_arr_str(barr))

def i2c_rd_u8(addr):
    u8 = i2c.readfrom_mem(EDOG_ADDR, addr, 1)[0]
    #print("i2c_rd_u8: ", addr, u8 )
    return u8


def i2c_wr_u32(addr, u32):
    # barr = addr.to_bytes(1,"big") + u32.to_bytes(4,"big")
    barr = u32.to_bytes(4,"big")
    i2c.writeto_mem(EDOG_ADDR, addr, barr)
    #print("i2c_wr_u32: ", get_hex_arr_str(barr))

def i2c_rd_u32(addr):
    u32 = 0
    rx_4 = i2c.readfrom_mem(EDOG_ADDR, addr, 4)
    u32 = (rx_4[0] << 24) | (rx_4[1] << 16) |(rx_4[2] << 8) |rx_4[3] 
    #print("i2c_rd_u32: ", addr, get_hex_arr_str(rx_4), u32 )
    # barr = addr.to_bytes(1,"big") + u32.to_bytes(4,"big")
    return u32
    

def i2c_wr_16_bytes(addr, arr16):
    i2c.writeto_mem(EDOG_ADDR, addr, arr16)
    #print("i2c_wr_16_bytes: ", get_hex_arr_str(arr16))

def i2c_rd_16_bytes(addr):
    rx_16 = i2c.readfrom_mem(EDOG_ADDR, addr, 16)  
    #print("i2c_rd_16_bytes: ", get_hex_arr_str(rx_16))
    return rx_16

def i2c_wr_eeprom_16_bytes(addr, arr16):
    i2c_wr_u8(reg.REG_EEPROM_ADDR, addr)
    i2c_wr_16_bytes(reg.REG_EEPROM_BUFF, arr16)
    i2c_wr_u8(reg.REG_EEPROM_STATE, reg.EEPROM_WR_BUFF)
    while (i2c_rd_u8(reg.REG_EEPROM_STATE)) != reg.EEPROM_READY:
        time.sleep(0.1)
    i2c_wr_u8(reg.REG_EEPROM_STATE, reg.EEPROM_IDLE)

def i2c_rd_eeprom_16_bytes(addr):
    i2c_wr_u8(reg.REG_EEPROM_ADDR, addr)
    i2c_wr_u8(reg.REG_EEPROM_STATE, reg.EEPROM_RD_BUFF)
    do_wait = True
    while (i2c_rd_u8(reg.REG_EEPROM_STATE)) != reg.EEPROM_READY:
        time.sleep(0.1)
    i2c_wr_u8(reg.REG_EEPROM_STATE, reg.EEPROM_IDLE)
    time.sleep(0.2)
    return i2c_rd_16_bytes(reg.REG_EEPROM_BUFF)

def load_sw_power_on(pwr1, pwr2):
    bm = 0
    if (not pwr1):
        bm = bm | 0x01  
    if (not pwr2): 
        bm = bm | 0x02   
    i2c_wr_u8(reg.REG_LOAD_SW, bm)
    print ('Load SW Power: ', bm)   
    

TEST_PWR_ON_OFF = 0 
TEST_SLEEP = 1

#Test = TEST_PWR_ON_OFF
Test = TEST_SLEEP
try:
    print('Test: ', Test)
except Exception as e:
    print('Test Exception: ', e)        
time.sleep(2)


if Test == TEST_SLEEP:
    while True:
        time.sleep(2)
        try:
            i2c_wr_u32( reg.REG_SLEEP_TIME, 2000)
            time.sleep(1)    
            i2c_wr_u8(reg.REG_SLEEP_STATE, reg.SLEEP_ACTIVATED)
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            print('Test Interrupted')
        except Exception as e:
            pass
            # print('Test Exception: ', e)



interval = 2000
if Test == TEST_PWR_ON_OFF: 
    while True: 
        try:
            load_sw_power_on(True, True)
            time.sleep(0.1) 
            load_sw_power_on(False, True)
            time.sleep(0.1)               
            load_sw_power_on(True, False)
            time.sleep(0.1)       
            load_sw_power_on(False, False)
            time.sleep(0.1)   
        except KeyboardInterrupt:
            print('Test Interrupted')
        except Exception as e:
            print('Test Exception: ', e)


b16_wr = bytearray(0x10 + i*2 for i in range(16))
b16_rd = [[0]]*16
ep_state = 0
do_wait = True

i2c_wr_eeprom_16_bytes(0x20, b16_wr)
b16_rd = i2c_rd_eeprom_16_bytes(0x18)   
print(list(b16_wr),'->',list(b16_rd))
sys.exit()


i2c_wr_u32(0x04, 706969)
# time.sleep(0.01)
i2c_rd_u32(0x04)
sys.exit()

i2c_wr_u32(reg.CMD_SET_WD_INTERVAL, interval)
#interval = i2c_rd_u32(reg.CMD_GET_WD_INTERVAL)
print('interval rd = ',interval)
#interval = interval + 1000

wd_clr_value = 0

for delay_sec in range(0,40):
    wd_clr_pin.value(wd_clr_value)
    if wd_clr_value == 0:
        wd_clr_value = 1
    else:
        wd_clr_value = 0
    time.sleep(delay_sec/10)
    print('delay_sec=',delay_sec/10)

while (True):
    pass
    

rx_data = i2c.readfrom_mem(EDOG_ADDR, reg.CMD_GET_WD_INTERVAL, 4)        # read 4 bytes from peripheral with 7-bit address 42

i2c_wr_u32(reg.CMD_SET_WD_INTERVAL, interval)

# i2c.readfrom_mem(EDOG_ADDR, 8, 3)
print("Initial read CMD_GET_WD_INTERVAL: ", get_hex_arr_str(rx_data))

base_val = 0x00

while(True):
    b8_wr = bytearray(base_val + i for i in range(8))
    i2c_wr_8_bytes(reg.CMD_EEPROM_WRITE, b8_wr)
    # print("bytearray: ", get_hex_arr_str(b8_wr))
    time.sleep(0.1)
    base_val = base_val + 0x10
    if base_val > 0xF0:
        base_val = 0x00
    i2c.writeto_mem(EDOG_ADDR, reg.CMD_EEPROM_LOAD, b'\x00')
    time.sleep(0.1)
    b8_rd = i2c_rd_8_bytes(reg.CMD_EEPROM_READ)


while(True):
    i2c_wr_u32(reg.CMD_SET_WD_INTERVAL, interval)
    # '\x01\x12\x23\x45\x67')
    time.sleep(0.5)
    # rx_data = i2c.readfrom(EDOG_ADDR, 4)             # read 4 bytes from peripheral with 7-bit address 42
    rx_data = i2c.readfrom_mem(EDOG_ADDR, reg.CMD_GET_WD_INTERVAL, 4)        # read 4 bytes from peripheral with 7-bit address 42
    # i2c.readfrom_mem(EDOG_ADDR, 8, 3)
    print("Read CMD_GET_WD_INTERVAL: ", get_hex_arr_str(rx_data))
    time.sleep(0.1)
    rx_data = i2c.readfrom_mem(EDOG_ADDR, reg.CMD_GET_RESTARTS, 1)
    print("Read CMD_GET_RESTARTS: ", get_hex_arr_str(rx_data))
    interval = interval + 16
    time.sleep(10.0)
    
    
    
i2c.readfrom_mem(EDOG_ADDR, 8, 3)      # read 3 bytes from memory of peripheral 42,
                                #   starting at memory-address 8 in the peripheral
i2c.writeto_mem(EDOG_ADDR, 2, b'\x10') # write 1 byte to memory of peripheral 42
                                #   starting at address 2 in the peripheral
                                
