/*
 * ssd1306_lib.h
 *
 *  Created on: 27.12.2020
 *      Author: thorsten kattanek
 */

#ifndef SRC_SSD1306_LIB_H_
#define SRC_SSD1306_LIB_H_

#include <stm32f4xx_hal.h>

// LCD Config
#define LCD_WIDTH 128
#define LCD_HEIGTH 64

// LCD SSD1306 Commands
#define SSD1306_SETCONTRAST   		0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_DISPLAYOFF 			0xAE
#define SSD1306_DISPLAYON 			0xAF
#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETCOMPINS 			0xDA
#define SSD1306_SETVCOMDETECT 		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETPRECHARGE 		0xD9
#define SSD1306_SETMULTIPLEX 		0xA8
#define SSD1306_SETLOWCOLUMN 		0x00
#define SSD1306_SETHIGHCOLUMN 		0x10
#define SSD1306_SETSTARTLINE 		0x40
#define SSD1306_MEMORYMODE 			0x20
#define SSD1306_COLUMNADDR 			0x21
#define SSD1306_PAGEADDR   			0x22
#define SSD1306_COMSCANINC 			0xC0
#define SSD1306_COMSCANDEC 			0xC8
#define SSD1306_SEGREMAP 			0xA0
#define SSD1306_CHARGEPUMP 			0x8D
#define SSD1306_EXTERNALVCC 		0x1
#define SSD1306_SWITCHCAPVCC 		0x2

void ssd1306_send_command(uint8_t c);
void ssd1306_send_data(uint8_t c);
void ssd1306_set_colAddress(void);
void ssd1306_set_pageAddress(void);
void ssd1306_init_display(I2C_HandleTypeDef *i2c_handle, uint8_t i2c_address);
void ssd1306_transfer_buffer(void);

#endif /* SRC_SSD1306_LIB_H_ */
