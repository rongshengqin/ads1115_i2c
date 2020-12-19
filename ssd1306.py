from machine import I2C
i2c=machine.I2C(-1, sda=machine.Pin(4), scl=machine.Pin(5), freq=400000)  

from ssd1306 import SSD1306_I2C
oled = SSD1306_I2C(128, 64, i2c)
oled.contrast(100)
oled.text("Hello World !", 0, 30)
oled.show()