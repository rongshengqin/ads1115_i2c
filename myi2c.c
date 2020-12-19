#include "myi2c.h"
#define EINVAL -1
#define ERANGE -1
void init_i2c()
{
	gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
	gpio_set_level(sda,1);
	gpio_set_level(scl,1);
}

static inline void gpio_set_level_fast(uint32_t pin, uint8_t val) {

if (GPIO_IS_VALID_GPIO(pin)) {
    if (!RTC_GPIO_IS_VALID_GPIO(pin)) {
        if (val)
            GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1ul << pin);
        else
            GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1ul << pin);
    } else {
        if (val)
            REG_SET_BIT(RTC_GPIO_OUT, 1);
        else
            REG_CLR_BIT(RTC_GPIO_OUT, 1);
    }
}
}


static inline void  i2c_start(){
    //gpio_set_direction(sda,GPIO_MODE_OUTPUT);
    //gpio_set_level_fast(sda,1);
    //gpio_set_level_fast(scl,1);
    //os_delay_us(0.5);
    gpio_set_level_fast(sda,0);
    os_delay_us(0.6);
    gpio_set_level_fast(scl,0);
}
static inline void i2c_stop(){
   // gpio_set_direction(sda,GPIO_MODE_OUTPUT);
	gpio_set_level_fast(sda,0);
    gpio_set_level_fast(scl,1);
    os_delay_us(0.6);
    gpio_set_level_fast(sda,1);
    os_delay_us(1.3);
}


static inline void nop_delay(uint32_t n){
	for (int i=0;i<10;i++){
		__asm__ __volatile__ ("nop"); //one nop duration is abort 0.04~0.05 us
	}
}
int  i2c_send_byte(uint8_t byte){
    //gpio_set_direction(sda,GPIO_MODE_OUTPUT);
	//gpio_set_level(sda,0)
    for (int i=0;i<8;i++){
        // if( byte&0X80)
            // gpio_set_level(sda,1);
        // else
            // gpio_set_level(sda,0);
		gpio_set_level_fast(sda,(byte&0X80)?1:0);
        byte<<=1;
        //os_delay_us(0.5);
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		// __asm__ __volatile__ ("nop");
		// __asm__ __volatile__ ("nop");
		//nop_delay(1);
        gpio_set_level_fast(scl,1);
		//nop_delay(2);
        //os_delay_us(1.0);
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		// __asm__ __volatile__ ("nop");
		 //__asm__ __volatile__ ("nop");
		 //__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
        gpio_set_level_fast(scl,0);
		//nop_delay(1);
        //os_delay_us(0.5);
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
	}
    gpio_set_level_fast(sda,1);
    //gpio_set_direction(sda,GPIO_MODE_INPUT);
    //os_delay_us(0.01);
	__asm__ __volatile__ ("nop");
    gpio_set_level_fast(scl,1);
    //int ack=gpio_get_level(sda);
    //os_delay_us(0.02);
	__asm__ __volatile__ ("nop");
	//__asm__ __volatile__ ("nop");
    gpio_set_level_fast(scl,0);
	//gpio_set_direction(sda,GPIO_MODE_OUTPUT);
    //os_delay_us(0.01);
	//__asm__ __volatile__ ("nop");
    // if (ack)
        // printf("device no response!\n");
    return 0;
}
void i2c_send_cmd(uint8_t reg,uint8_t cmd){
    i2c_start();
    i2c_send_byte(0b01111000);
    i2c_send_byte(reg);
    i2c_send_byte(cmd);
    i2c_stop();
}

void i2c_send_data(uint8_t reg,uint8_t* data, uint8_t len){
    i2c_start();
    i2c_send_byte(0b01111000);
    i2c_send_byte(reg);
	for (int i =0;i<len;i++){
		i2c_send_byte(data[i]);
	}
    i2c_stop();
}

int init_ssd1306(void)
{
	init_i2c();
    //uint8_t pin_cfg;
    //pin_cfg = 0x12;
	// gpio_set_level_fast(scl,0);
	// vTaskDelay(2000 / portTICK_RATE_MS);
	// gpio_set_level_fast(scl,1);
	
	uint8_t reg=0b00000000;
	i2c_send_cmd(reg,0xae);
	i2c_send_cmd(reg,0x20);
	i2c_send_cmd(reg,0x0);
	i2c_send_cmd(reg,0x40);
	i2c_send_cmd(reg,0xa1);
	i2c_send_cmd(reg,0xa8);
	i2c_send_cmd(reg,0x3f);
	i2c_send_cmd(reg,0xc8);
	i2c_send_cmd(reg,0xd3);
	i2c_send_cmd(reg,0x0);
	i2c_send_cmd(reg,0xda);
	i2c_send_cmd(reg,0x12);
	i2c_send_cmd(reg,0xd5);
	i2c_send_cmd(reg,0x80);
	i2c_send_cmd(reg,0xd9);
	i2c_send_cmd(reg,0xf1);
	i2c_send_cmd(reg,0xdb);
	i2c_send_cmd(reg,0x30);
	i2c_send_cmd(reg,0x81);
	i2c_send_cmd(reg,0xff);
	i2c_send_cmd(reg,0xa4);
	i2c_send_cmd(reg,0xa6);
	i2c_send_cmd(reg,0x8d);
	i2c_send_cmd(reg,0x14);
	i2c_send_cmd(reg,0xaf);
	return 0;
}

int ssd1306_draw_pixel(const ssd1306_t *dev, uint8_t *fb, int8_t x, int8_t y, ssd1306_color_t color)
{
    uint16_t index;

    if ((x >= dev->width) || (x < 0) || (y >= dev->height) || (y < 0))
        return -EINVAL;
    index = x + (y / 8) * dev->width;
    switch (color) {
        case OLED_COLOR_WHITE:
            fb[index] |= (1 << (y & 7));
            break;
        case OLED_COLOR_BLACK:
            fb[index] &= ~(1 << (y & 7));
            break;
        case OLED_COLOR_INVERT:
            fb[index] ^= (1 << (y & 7));
            break;
        default:
            break;
    }
    return 0;
}

int ssd1306_draw_char(const ssd1306_t *dev, uint8_t *fb, const font_info_t *font, uint8_t x, uint8_t y, char c, ssd1306_color_t foreground,
        ssd1306_color_t background)
{
    uint8_t i, j;
    const uint8_t *bitmap;
    uint8_t line = 0;
    int err = NULL;

    if (font == NULL)
        return 0;

    const font_char_desc_t *d = font_get_char_desc(font, c);
    if (d == NULL)
        return 0;

    bitmap = font->bitmap + d->offset;
    for (j = 0; j < font->height; ++j) {
        for (i = 0; i < d->width; ++i) {
            if (i % 8 == 0) {
                line = bitmap[(d->width + 7) / 8 * j + i / 8]; // line data
            }
            if (line & 0x80) {
                err = ssd1306_draw_pixel(dev, fb, x + i, y + j, foreground);
            }
            else {
                switch (background) {
                    case OLED_COLOR_TRANSPARENT:
                        // Not drawing for transparent background
                        break;
                    case OLED_COLOR_WHITE:
                    case OLED_COLOR_BLACK:
                        err = ssd1306_draw_pixel(dev, fb, x + i, y + j, background);
                        break;
                    case OLED_COLOR_INVERT:
                        // I don't know why I need invert background
                        break;
                }
            }
            if (err)
                return -ERANGE;
            line = line << 1;
        }
    }
    return d->width;
}

int ssd1306_draw_string(const ssd1306_t *dev, uint8_t *fb, const font_info_t *font, uint8_t x, uint8_t y, const char *str,
        ssd1306_color_t foreground, ssd1306_color_t background)
{
    uint8_t t = x;
    int err;

    if (font == NULL || str == NULL)
        return 0;

    while (*str) {
        if ((err = ssd1306_draw_char(dev, fb, font, x, y, *str, foreground, background)) < 0)
            return err;
        x += err;
        ++str;
        if (*str)
            x += font->c;
    }
    return x - t;
}

#define SSD1306_SET_COL_ADDR         (0x21)
#define SSD1306_SET_PAGE_ADDR        (0x22)

int ssd1306_set_column_addr(const ssd1306_t *dev, uint8_t start, uint8_t stop)
{
    int err = 0;
    // if ((err = ssd1306_command(dev, SSD1306_SET_COL_ADDR)))
        // return err;
    // if ((err = ssd1306_command(dev, start)))
        // return err;

    // return ssd1306_command(dev, stop);
	i2c_send_cmd(0x00,SSD1306_SET_COL_ADDR);
	i2c_send_cmd(0x00,start);
	i2c_send_cmd(0x00,stop);
	return err;
}


int ssd1306_set_page_addr(const ssd1306_t *dev, uint8_t start, uint8_t stop)
{
    int err = 0;
    // if ((err = ssd1306_command(dev, SSD1306_SET_PAGE_ADDR)))
        // return err;

    // if ((err = ssd1306_command(dev, start)))
        // return err;

    // return ssd1306_command(dev, stop);
	i2c_send_cmd(0x00,SSD1306_SET_PAGE_ADDR);
	i2c_send_cmd(0x00,start);
	i2c_send_cmd(0x00,stop);
	return err;
}

int ssd1306_load_frame_buffer(const ssd1306_t *dev, uint8_t buf[])
{
    uint16_t i;
    uint8_t tab[16] = { 0 };
    size_t len = dev->width * dev->height / 8;
    ssd1306_set_column_addr(dev, 0, dev->width - 1);
    ssd1306_set_page_addr(dev, 0, dev->height / 8 - 1);
  
	printf("len=%d\n",len);
    for (i = 0; i < len; i++) {
        i2c_send_data(0x40, buf ? &buf[i] : tab, 16);
        i += 15;
    }
	//i2c_send_data(0x40, buf , 1024);
    return 0;
}