from machine import Pin
import time
sda=Pin(14,mode=Pin.OUT)
scl=Pin(2,mode=Pin.OUT)
# interrupt 
irq_pin = Pin(5, mode=Pin.IN)

def start():
    sda=Pin(14,mode=Pin.OUT)
    scl=Pin(2,mode=Pin.OUT)
    sda.on()
    scl.on()
    time.sleep_us(2)
    sda.off()
    time.sleep_us(2)
    scl.off()
    time.sleep_us(2)
def stop():
    sda=Pin(14,mode=Pin.OUT)
    scl=Pin(2,mode=Pin.OUT)
    sda.off()
    time.sleep_us(1)
    scl.on()
    time.sleep_us(2)
    sda.on()
    time.sleep_us(2)
    
def send_byte(byte):
    sda=Pin(14,mode=Pin.OUT)
    scl=Pin(2,mode=Pin.OUT)
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
    sda=Pin(14,mode=Pin.IN)
    time.sleep_us(1)
    scl.on()
    ack=sda.value()
    time.sleep_us(2)
    scl.off()
    time.sleep_us(1)
    if ack:
        print('device no response!')
    #print ('ack=',ack)
    return ack
    
def read_byte(n):
    sda=Pin(14,mode=Pin.IN)
    scl=Pin(2,mode=Pin.OUT)
    buff=bytearray(n)
    scl.off()
    for j in range(n):
        for i in range(8):
            buff[j]<<=1
            time.sleep_us(1)
            scl.on()
            time.sleep_us(2)
            if(sda.value()):
                buff[j]|=0x01
            scl.off()
            time.sleep_us(1)
        sda=Pin(14,mode=Pin.OUT)
        sda.off()
        scl.on()
        time.sleep_us(2)
        scl.off()
        sda=Pin(14,mode=Pin.IN)
    #print(buff[0])
    return buff

def sample():
    # read date
    start()
    send_byte(0b10010000)
    send_byte(0b00000000)
    stop()
    
    start()
    send_byte(0b10010001)
    d=read_byte(2)
    stop()
    print(d[0],d[1],(d[0]*256+d[1])/32768.0*2.048000)

index_put=0    
def interrupt_handler(x):
    global index_put
    #sample()
    index_put+=1
    print('interrupt_handler',index_put)
#set interrupt
start()
send_byte(0b10010000)
send_byte(0b00000010)
send_byte(0b11111111)
send_byte(0b11111111)
stop()
start()
send_byte(0b10010000)
send_byte(0b00000011)
send_byte(0b00000000)
send_byte(0b11111111)
stop()
irq_pin.irq(trigger=Pin.IRQ_FALLING, handler=interrupt_handler)

# set register
start()
send_byte(0b10010000)
send_byte(0b00000001)
send_byte(0b11000100)
send_byte(0b00000011)
stop()


# read register
start()
send_byte(0b10010000)
send_byte(0b00000001)
stop()
start()
send_byte(0b10010001)
read_byte(2)
stop()

for i in range(0):
    time.sleep_ms(500)
    ## read date
    sample()
#t0=time.time()
sample()
while index_put<1000:
    #print(index_put,'###')
    index_put+=1
    time.sleep_ms(1000)
    sample()
#dt=time.time()-t0
#print('dt=',dt,'fps=',100/dt)
