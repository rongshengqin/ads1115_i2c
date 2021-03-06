from machine import Pin
import time
sda=Pin(4,mode=Pin.OUT)
scl=Pin(5,mode=Pin.OUT)

def start():
    sda=Pin(4,mode=Pin.OUT)
    scl=Pin(5,mode=Pin.OUT)
    sda.on()
    scl.on()
    time.sleep_us(2)
    sda.off()
    time.sleep_us(2)
    scl.off()
    time.sleep_us(2)
def stop():
    sda=Pin(4,mode=Pin.OUT)
    scl=Pin(5,mode=Pin.OUT)
    sda.off()
    time.sleep_us(1)
    scl.on()
    time.sleep_us(2)
    sda.on()
    time.sleep_us(2)
    
def send_byte(byte):
    sda=Pin(4,mode=Pin.OUT)
    scl=Pin(5,mode=Pin.OUT)
    scl.off()
    for i in range(8):
        if byte&0X80:
            sda.on()
            #print(1)
        else:
            sda.off()
            #print(0)
        byte<<=1
        time.sleep_us(1)
        scl.on()
        time.sleep_us(2)
        scl.off()
        time.sleep_us(1)
    sda.on()
    sda=Pin(4,mode=Pin.IN)
    time.sleep_us(1)
    scl.on()
    ack=sda.value()
    time.sleep_us(2)
    scl.off()
    time.sleep_us(1)
    if ack:
        print('device no response!')
    print ('ack=',ack)
    return ack
def send_cmd(byte):
    start()
    send_byte(0b01111000)
    send_byte(0b00000000)
    send_byte(byte)
    stop()
def send_data(byte):
    start()
    send_byte(0b01111000)
    send_byte(0x40)
    send_byte(byte)
    stop()
SET_CONTRAST = const(0x81)
SET_ENTIRE_ON = const(0xA4)
SET_NORM_INV = const(0xA6)
SET_DISP = const(0xAE)
SET_MEM_ADDR = const(0x20)
SET_COL_ADDR = const(0x21)
SET_PAGE_ADDR = const(0x22)
SET_DISP_START_LINE = const(0x40)
SET_SEG_REMAP = const(0xA0)
SET_MUX_RATIO = const(0xA8)
SET_COM_OUT_DIR = const(0xC0)
SET_DISP_OFFSET = const(0xD3)
SET_COM_PIN_CFG = const(0xDA)
SET_DISP_CLK_DIV = const(0xD5)
SET_PRECHARGE = const(0xD9)
SET_VCOM_DESEL = const(0xDB)
SET_CHARGE_PUMP = const(0x8D)
height=64
cmds=(SET_DISP | 0x00,  # off
            # address setting
            SET_MEM_ADDR,
            0x00,  # horizontal
            # resolution and layout
            SET_DISP_START_LINE | 0x00,
            SET_SEG_REMAP | 0x01,  # column addr 127 mapped to SEG0
            SET_MUX_RATIO,
            height - 1,
            SET_COM_OUT_DIR | 0x08,  # scan from COM[N] to COM0
            SET_DISP_OFFSET,
            0x00,
            SET_COM_PIN_CFG,
            0x12,
            # timing and driving scheme
            SET_DISP_CLK_DIV,
            0x80,
            SET_PRECHARGE,
            0xF1,
            SET_VCOM_DESEL,
            0x30,  # 0.83*Vcc
            # display
            SET_CONTRAST,
            0xFF,  # maximum
            SET_ENTIRE_ON,  # output follows RAM contents
            SET_NORM_INV,  # not inverted
            # charge pump
            SET_CHARGE_PUMP,
            0x14,
            SET_DISP | 0x01,
        )
for c in cmds:
    #send_cmd(c)
    print("ssd1306_command(dev,%s);"%hex(c))
#send_data(23)
#send_data(42)