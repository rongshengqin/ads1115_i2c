def callback(p):
    print('pin change', p)
from machine import Pin
p0 = Pin(5, Pin.IN)
p0.on()
p2 = Pin(2, Pin.IN)
p0.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler=callback)
p2.irq(trigger=Pin.IRQ_FALLING, handler=callback)