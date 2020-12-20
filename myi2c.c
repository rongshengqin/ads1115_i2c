#include "myi2c.h"
#define EINVAL -1
#define ERANGE -1
#define abs(x) ((x)<0 ? -(x) : (x))
#define swap(x, y) do { typeof(x) temp##x##y = x; x = y; y = temp##x##y; } while (0)

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
	os_delay_us(1.0);
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
		__asm__ __volatile__ ("nop");
		__asm__ __volatile__ ("nop");
		// __asm__ __volatile__ ("nop");
		//nop_delay(1);
        gpio_set_level_fast(scl,1);
		//nop_delay(2);
        //os_delay_us(1.0);
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		//__asm__ __volatile__ ("nop");
		// __asm__ __volatile__ ("nop");
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
		 __asm__ __volatile__ ("nop");
		 __asm__ __volatile__ ("nop");
        gpio_set_level_fast(scl,0);
		//nop_delay(1);
        //os_delay_us(0.5);
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
	}
    gpio_set_level_fast(sda,1);
    //gpio_set_direction(sda,GPIO_MODE_INPUT);
    //os_delay_us(0.01);
	__asm__ __volatile__ ("nop");
    gpio_set_level_fast(scl,1);
    //int ack=gpio_get_level(sda);
    //os_delay_us(0.02);
	__asm__ __volatile__ ("nop");
	__asm__ __volatile__ ("nop");
    gpio_set_level_fast(scl,0);
	//gpio_set_direction(sda,GPIO_MODE_OUTPUT);
    //os_delay_us(0.01);
	__asm__ __volatile__ ("nop");
    // if (ack)
         //printf("device no response byte= %d\n",byte);
    return 0;
}
int i2c_send_cmd(uint8_t reg,uint8_t cmd)
{
	int ret=0;
    i2c_start();
	ret|=i2c_send_byte(0b01111000);
    ret|=i2c_send_byte(reg);
    ret|=i2c_send_byte(cmd);
    i2c_stop();
	return ret;
}

void i2c_send_data(uint8_t reg,uint8_t* data, uint32_t len){
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
	i2c_start();
	i2c_stop();
    //uint8_t pin_cfg;
    //pin_cfg = 0x12;
	// gpio_set_level_fast(scl,0);
	// vTaskDelay(2000 / portTICK_RATE_MS);
	// gpio_set_level_fast(scl,1);
	uint8_t cmds[]=
	{
	0xae,
	0x20,
	0x0,
	0x40,
	0xa1,
	0xa8,
	0x3f,
	0xc8,
	0xd3,
	0x0,
	0xda,
	0x12,
	0xd5,
	0x80,
	0xd9,
	0xf1,
	0xdb,
	0x30,
	0x81,
	0xff,
	0xa4,
	0xa6,
	0x8d,
	0x14,
	0xaf
	};
	uint8_t reg=0b00000000;
	int ret=0;
	for (int i=0;i<25;i++){
		ret=i2c_send_cmd(reg,cmds[i]);
		if (ret)
			printf("error %d %d\n",i,cmds[i]);
	}
	// i2c_send_cmd(reg,0x20);
	// i2c_send_cmd(reg,0x0);
	// i2c_send_cmd(reg,0x40);
	// i2c_send_cmd(reg,0xa1);
	// i2c_send_cmd(reg,0xa8);
	// i2c_send_cmd(reg,0x3f);
	// i2c_send_cmd(reg,0xc8);
	// i2c_send_cmd(reg,0xd3);
	// i2c_send_cmd(reg,0x0);
	// i2c_send_cmd(reg,0xda);
	// i2c_send_cmd(reg,0x12);
	// i2c_send_cmd(reg,0xd5);
	// i2c_send_cmd(reg,0x80);
	// i2c_send_cmd(reg,0xd9);
	// i2c_send_cmd(reg,0xf1);
	// i2c_send_cmd(reg,0xdb);
	// i2c_send_cmd(reg,0x30);
	// i2c_send_cmd(reg,0x81);
	// i2c_send_cmd(reg,0xff);
	// i2c_send_cmd(reg,0xa4);
	// i2c_send_cmd(reg,0xa6);
	// i2c_send_cmd(reg,0x8d);
	// i2c_send_cmd(reg,0x14);
	// i2c_send_cmd(reg,0xaf);
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

int ssd1306_draw_line(const ssd1306_t *dev, uint8_t *fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, ssd1306_color_t color)
{
    if ((x0 >= dev->width) || (x0 < 0) || (y0 >= dev->height) || (y0 < 0))
        return -EINVAL;
    if ((x1 >= dev->width) || (x1 < 0) || (y1 >= dev->height) || (y1 < 0))
        return -EINVAL;

    int err;
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t errx = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            if ((err = ssd1306_draw_pixel(dev, fb, y0, x0, color)))
                return err;
        }
        else {
            if ((err = ssd1306_draw_pixel(dev, fb, x0, y0, color)))
                return err;
        }
        errx -= dy;
        if (errx < 0) {
            y0 += ystep;
            errx += dx;
        }
    }
    return 0;
}

int ssd1306_draw_hline(const ssd1306_t *dev, uint8_t *fb, int8_t x, int8_t y, uint8_t w, ssd1306_color_t color)
{
    uint16_t index;
    uint8_t mask, t;

    // boundary check
    if ((x >= dev->width) || (x < 0) || (y >= dev->height) || (y < 0))
        return -EINVAL;
    if (w == 0)
        return -EINVAL;
    if (x + w > dev->width)
        w = dev->width - x;

    t = w;
    index = x + (y / 8) * dev->width;
    mask = 1 << (y & 7);
    switch (color) {
        case OLED_COLOR_WHITE:
            while (t--) {
                fb[index] |= mask;
                ++index;
            }
            break;
        case OLED_COLOR_BLACK:
            mask = ~mask;
            while (t--) {
                fb[index] &= mask;
                ++index;
            }
            break;
        case OLED_COLOR_INVERT:
            while (t--) {
                fb[index] ^= mask;
                ++index;
            }
            break;
        default:
            break;
    }
    return 0;
}

int ssd1306_draw_vline(const ssd1306_t *dev, uint8_t *fb, int8_t x, int8_t y, uint8_t h, ssd1306_color_t color)
{
    uint16_t index;
    uint8_t mask, mod, t;

    // boundary check
    if ((x >= dev->width) || (x < 0) || (y >= dev->height) || (y < 0))
        return -EINVAL;
    if (h == 0)
        return -EINVAL;
    if (y + h > dev->height)
        h = dev->height - y;

    t = h;
    index = x + (y / 8) * dev->width;
    mod = y & 7;
    if (mod) // partial line that does not fit into byte at top
    {
        // Magic from Adafruit
        mod = 8 - mod;
        static const uint8_t premask[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
        mask = premask[mod];
        if (t < mod)
            mask &= (0xFF >> (mod - t));
        switch (color) {
            case OLED_COLOR_WHITE:
                fb[index] |= mask;
                break;
            case OLED_COLOR_BLACK:
                fb[index] &= ~mask;
                break;
            case OLED_COLOR_INVERT:
                fb[index] ^= mask;
                break;
            default:
                break;
        }

        if (t < mod)
            return 0;
        t -= mod;
        index += dev->width;
    }
    if (t >= 8) // byte aligned line at middle
            {
        switch (color) {
            case OLED_COLOR_WHITE:
                do {
                    fb[index] = 0xff;
                    index += dev->width;
                    t -= 8;
                } while (t >= 8);
                break;
            case OLED_COLOR_BLACK:
                do {
                    fb[index] = 0x00;
                    index += dev->width;
                    t -= 8;
                } while (t >= 8);
                break;
            case OLED_COLOR_INVERT:
                do {
                    fb[index] = ~fb[index];
                    index += dev->width;
                    t -= 8;
                } while (t >= 8);
                break;
            default:
                break;
        }
    }
    if (t) // partial line at bottom
    {
        mod = t & 7;
        static const uint8_t postmask[8] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
        mask = postmask[mod];
        switch (color) {
            case OLED_COLOR_WHITE:
                fb[index] |= mask;
                break;
            case OLED_COLOR_BLACK:
                fb[index] &= ~mask;
                break;
            case OLED_COLOR_INVERT:
                fb[index] ^= mask;
                break;
            default:
                break;
        }
    }
    return 0;
}


int ssd1306_draw_circle(const ssd1306_t *dev, uint8_t *fb, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color)
{
    // Refer to http://en.wikipedia.org/wiki/Midpoint_circle_algorithm for the algorithm
    int8_t x = r;
    int8_t y = 1;
    int16_t radius_err = 1 - x;
    int err = 0;

    if (r == 0)
        return -EINVAL;

    if ((err = ssd1306_draw_pixel(dev, fb, x0 - r, y0,     color)))
        return err;
    if ((err = ssd1306_draw_pixel(dev, fb, x0 + r, y0,     color)))
        return err;
    if ((err = ssd1306_draw_pixel(dev, fb, x0,     y0 - r, color)))
        return err;
    if ((err = ssd1306_draw_pixel(dev, fb, x0,     y0 + r, color)))
        return err;

    while (x >= y) {
        if ((err = ssd1306_draw_pixel(dev, fb, x0 + x, y0 + y, color)))
            return err;
        if ((err = ssd1306_draw_pixel(dev, fb, x0 - x, y0 + y, color)))
            return err;
        if ((err = ssd1306_draw_pixel(dev, fb, x0 + x, y0 - y, color)))
            return err;
        if ((err = ssd1306_draw_pixel(dev, fb, x0 - x, y0 - y, color)))
            return err;
        if (x != y) {
            /* Otherwise the 4 drawings below are the same as above, causing
             * problem when color is INVERT
             */
            if ((err = ssd1306_draw_pixel(dev, fb, x0 + y, y0 + x, color)))
                return err;
            if ((err = ssd1306_draw_pixel(dev, fb, x0 - y, y0 + x, color)))
                return err;
            if ((err = ssd1306_draw_pixel(dev, fb, x0 + y, y0 - x, color)))
                return err;
            if ((err = ssd1306_draw_pixel(dev, fb, x0 - y, y0 - x, color)))
                return err;
        }
        ++y;
        if (radius_err < 0) {
            radius_err += 2 * y + 1;
        }
        else {
            --x;
            radius_err += 2 * (y - x + 1);
        }

    }
    return 0;
}

int ssd1306_fill_circle(const ssd1306_t *dev, uint8_t *fb, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color)
{
    int8_t x = 1;
    int8_t y = r;
    int16_t radius_err = 1 - y;
    int8_t x1;
    int err = 0;

    if (r == 0)
        return -EINVAL;

    if ((err = ssd1306_draw_vline(dev, fb, x0, y0 - r, 2 * r + 1, color))) // Center vertical line
        return err;
    while (y >= x) {
        if ((err = ssd1306_draw_vline(dev, fb, x0 - x, y0 - y, 2 * y + 1, color)))
            return err;
        if ((err = ssd1306_draw_vline(dev, fb, x0 + x, y0 - y, 2 * y + 1, color)))
            return err;
        if (color != OLED_COLOR_INVERT) {
            if ((err = ssd1306_draw_vline(dev, fb, x0 - y, y0 - x, 2 * x + 1, color)))
                return err;
            if ((err = ssd1306_draw_vline(dev, fb, x0 + y, y0 - x, 2 * x + 1, color)))
                return err;
        }
        ++x;
        if (radius_err < 0) {
            radius_err += 2 * x + 1;
        }
        else {
            --y;
            radius_err += 2 * (x - y + 1);
        }
    }

    if (color == OLED_COLOR_INVERT) {
        x1 = x; // Save where we stopped

        y = 1;
        x = r;
        radius_err = 1 - x;
        if ((err = ssd1306_draw_hline(dev, fb, x0 + x1, y0, r - x1 + 1, color)))
            return err;
        if ((err = ssd1306_draw_hline(dev, fb, x0 - r, y0, r - x1 + 1, color)))
            return err;
        while (x >= y) {
            if ((err = ssd1306_draw_hline(dev, fb, x0 + x1, y0 - y, x - x1 + 1, color)))
                return err;
            if ((err = ssd1306_draw_hline(dev, fb, x0 + x1, y0 + y, x - x1 + 1, color)))
                return err;
            if ((err = ssd1306_draw_hline(dev, fb, x0 - x, y0 - y, x - x1 + 1, color)))
                return err;
            if ((err = ssd1306_draw_hline(dev, fb, x0 - x, y0 + y, x - x1 + 1, color)))
                return err;
            ++y;
            if (radius_err < 0) {
                radius_err += 2 * y + 1;
            }
            else {
                --x;
                radius_err += 2 * (y - x + 1);
            }
        }
    }
    return 0;
}


int ssd1306_draw_triangle(const ssd1306_t *dev, uint8_t *fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
        ssd1306_color_t color)
{
    int err;
    if ((err = ssd1306_draw_line(dev, fb, x0, y0, x1, y1, color)))
        return err;
    if ((err = ssd1306_draw_line(dev, fb, x1, y1, x2, y2, color)))
        return err;
    return ssd1306_draw_line(dev, fb, x2, y2, x0, y0, color);
}

int ssd1306_fill_triangle(const ssd1306_t *dev, uint8_t *fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
        ssd1306_color_t color)
{
    int16_t a, b, y, last;
    int err;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1);swap(x0, x1);
    }
    if (y1 > y2) {
        swap(y2, y1);swap(x2, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);swap(x0, x1);
    }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)      a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a)      a = x2;
        else if (x2 > b) b = x2;
        if ((err = ssd1306_draw_hline(dev, fb, a, y0, b - a + 1, color)))
            return err;
        return 0;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2) last = y1;     // Include y1 scanline
    else          last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) swap(a, b);
        if ((err = ssd1306_draw_hline(dev, fb, a, y, b - a + 1, color)))
            return err;
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) swap(a, b);
        if ((err = ssd1306_draw_hline(dev, fb, a, y, b - a + 1, color)))
            return err;
    }
    return 0;
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
	//ssd1306_set_column_addr(dev, 0, dev->width - 1);
	//ssd1306_set_page_addr(dev, 0, dev->height / 8 - 1);
  
	//printf("len=%d\n",len);
    // for (i = 0; i < len; i++) {
        // i2c_send_data(0x40, buf ? &buf[i] : tab, 16);
        // i += 15;
    // }
	i2c_send_data(0x40, buf , 1024);
    return 0;
}

void ssd1306_clear_frame_buffer(const ssd1306_t *dev, uint8_t buf[])
{
    uint16_t i;
	uint32_t* ptr=(uint32_t*)buf;
    size_t len = dev->width * dev->height / 8/4;
    for (i = 0; i < len; i++) {
       ptr[i]=0;
    }
}

#define SSD1306_SCROLL_ENABLE        (0x2F)
#define SSD1306_SCROLL_DISABLE       (0x2E)
int ssd1306_start_scroll_hori(const ssd1306_t *dev, uint8_t scroll_to_right, uint8_t start_page, uint8_t stop_page, uint8_t frame)
{
    int err;
	uint8_t cmds[7]={0x2E,scroll_to_right?0x26:0x27,0x00,start_page,frame,stop_page,0,0,0x2F};
	i2c_send_data(0x00,cmds,9);
    return 0;
}