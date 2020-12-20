#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_system.h>
//#include <ssd1306/ssd1306.h>
#include <driver/i2c.h>
#include <esp_err.h>
#include <bitbang_i2c/myi2c.h>

#define SCL_PIN 5
#define SDA_PIN 4
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
extern uint32_t esp_get_time(void);
static uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

void app_main()
{
	esp_set_cpu_freq(80);
    // init i2s
    // int i2c_master_port = I2C_NUM_0;
    // i2c_config_t conf;
    // conf.mode = I2C_MODE_MASTER;
    // conf.sda_io_num = SDA_PIN;
    // conf.sda_pullup_en = 1;
    // conf.scl_io_num = SCL_PIN;
    // conf.scl_pullup_en = 1;
    // conf.clk_stretch_tick = 300;
    // ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
    // ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));

    // init ssd1306
	
    ssd1306_t dev = {
        .width = DISPLAY_WIDTH,
        .height = DISPLAY_HEIGHT};
    
    int ret=init_ssd1306();
	// printf("main1 ret=%d\n",ret);
	// ret=ssd1306_display_on(&dev, 1);
	// printf("main2 ret=%d\n",ret);
	uint32_t t0=esp_get_time();
	ssd1306_clear_frame_buffer(&dev, buffer);
	ssd1306_draw_string(&dev, buffer, font_builtin_fonts[FONT_FACE_GLCD5x7], 0, 20, "I MISS YOU SO MUCH", OLED_COLOR_WHITE,OLED_COLOR_BLACK);
	// ssd1306_draw_string(&dev, buffer, font_builtin_fonts[FONT_FACE_GLCD5x7], 0, 40, "vot=12242.43", OLED_COLOR_WHITE,OLED_COLOR_BLACK);
	ssd1306_draw_circle(&dev,buffer, 60, 32, 7, OLED_COLOR_WHITE);
	// printf("main3 ret=%d\n",ret);
	char* fps[8];
	if(ssd1306_load_frame_buffer(&dev, buffer))
		printf("ssd1306_load_frame_buffer\n");
	uint32_t t1=esp_get_time();
	printf("fps=%d\n",t1-t0);
	sprintf(fps,"fps %d hz",(uint32_t)(1000000/(t1-t0)));
	ssd1306_draw_string(&dev, buffer, font_builtin_fonts[FONT_FACE_GLCD5x7], 0, 40, fps, OLED_COLOR_WHITE,OLED_COLOR_BLACK);
	if(ssd1306_load_frame_buffer(&dev, buffer))
		printf("ssd1306_load_frame_buffer\n");
	vTaskDelay(10000 / portTICK_RATE_MS);
	//ssd1306_display_on(&dev, 0);
}