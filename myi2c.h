#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <fonts/fonts.h>

#define scl    5
#define sda    4
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<scl) | (1ULL<<sda))

/**
 * Device descriptor
 */
typedef struct
{
    uint8_t width;           //!< Screen width, currently supported 128px, 96px
    uint8_t height;          //!< Screen height, currently supported 16px, 32px, 64px
} ssd1306_t;

typedef enum
{
    OLED_COLOR_TRANSPARENT = -1, //!< Transparent (not drawing)
    OLED_COLOR_BLACK = 0,        //!< Black (pixel off)
    OLED_COLOR_WHITE = 1,        //!< White (or blue, yellow, pixel on)
    OLED_COLOR_INVERT = 2,       //!< Invert pixel (XOR)
} ssd1306_color_t;

int  i2c_send_byte(uint8_t byte);
int  i2c_send_cmd(uint8_t reg,uint8_t cmd);
int init_ssd1306(void);

/**
 * Load local framebuffer into the SSD1306 RAM.
 * @param dev Pointer to device descriptor
 * @param buf Pointer to framebuffer or NULL for clear RAM. Framebuffer size = width * height / 8
 * @return Non-zero if error occurred
 */
int ssd1306_load_frame_buffer(const ssd1306_t *dev, uint8_t buf[]);

/**
 * Clear SSD1306 RAM.
 * @param dev Pointer to device descriptor
 * @return Non-zero if error occurred
 */
inline int ssd1306_clear_screen(const ssd1306_t *dev)
{
    return ssd1306_load_frame_buffer(dev, NULL);
}
void ssd1306_clear_frame_buffer(const ssd1306_t *dev, uint8_t buf[]);
int ssd1306_draw_pixel(const ssd1306_t *dev, uint8_t *fb, int8_t x, int8_t y, ssd1306_color_t color);
int ssd1306_draw_hline(const ssd1306_t *dev, uint8_t *fb, int8_t x, int8_t y, uint8_t w, ssd1306_color_t color);
int ssd1306_draw_circle(const ssd1306_t *dev, uint8_t *fb, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color);
int ssd1306_fill_circle(const ssd1306_t *dev, uint8_t *fb, int8_t x0, int8_t y0, uint8_t r, ssd1306_color_t color);
int ssd1306_draw_triangle(const ssd1306_t *dev, uint8_t *fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, ssd1306_color_t color);
int ssd1306_fill_triangle(const ssd1306_t *dev, uint8_t *fb, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, ssd1306_color_t color);
int ssd1306_start_scroll_hori(const ssd1306_t *dev, uint8_t scroll_to_right, uint8_t start_page, uint8_t stop_page, uint8_t frame);
/**
 * Draw one character using currently selected font
 * @param dev Pointer to device descriptor
 * @param fb Pointer to framebuffer. Framebuffer size = width * height / 8
 * @param font Pointer to font info structure
 * @param x X position of character (top-left corner)
 * @param y Y position of character (top-left corner)
 * @param c The character to draw
 * @param foreground Character color
 * @param background Background color
 * @return Width of the character or negative value if error occurred
 */

int ssd1306_draw_char(const ssd1306_t *dev, uint8_t *fb, const font_info_t *font, uint8_t x, uint8_t y, char c, ssd1306_color_t foreground, ssd1306_color_t background);

/**
 * Draw one character using currently selected font
 * @param dev Pointer to device descriptor
 * @param fb Pointer to framebuffer. Framebuffer size = width * height / 8
 * @param font Pointer to font info structure
 * @param x X position of character (top-left corner)
 * @param y Y position of character (top-left corner)
 * @param str The string to draw
 * @param foreground Character color
 * @param background Background color
 * @return Width of the string  or negative value if error occurred
 */
int ssd1306_draw_string(const ssd1306_t *dev, uint8_t *fb, const font_info_t *font, uint8_t x, uint8_t y, const char *str, ssd1306_color_t foreground, ssd1306_color_t background);