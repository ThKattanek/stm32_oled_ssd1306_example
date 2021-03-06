/*
 * ssd1306_lib.c
 *
 *  Created on: 27.12.2020
 *      Author: thorsten kattanek
 */

#include "./ssd1306_lib.h"

static I2C_HandleTypeDef *i2c_handle;	// pointer to i2c struct
static uint8_t i2c_address;			// ic2 display address

static uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_HEIGTH ];

void ssd1306_send_command(uint8_t c)
{
	uint8_t control = 0x00;
	uint8_t buffer[2];

	buffer[0] = control;
	buffer[1] = c;

    HAL_I2C_Master_Transmit(i2c_handle, i2c_address, buffer, 2, 100);
}

void ssd1306_send_data(uint8_t c)
{
	uint8_t data = 0x40;
	uint8_t buffer[2];

	buffer[0] = data;
	buffer[1] = c;

    HAL_I2C_Master_Transmit(i2c_handle, i2c_address, buffer, 2, 100);
}

void ssd1306_set_colAddress(void)
{
  ssd1306_send_command(SSD1306_COLUMNADDR); // 0x21 COMMAND
  ssd1306_send_command(0); // Column start address
  ssd1306_send_command(DISPLAY_WIDTH-1); // Column end address
}

void ssd1306_set_pageAddress(void)
{
  ssd1306_send_command(SSD1306_PAGEADDR); // 0x22 COMMAND
  ssd1306_send_command(0); // Start Page address
  ssd1306_send_command((DISPLAY_HEIGTH/8)-1);// End Page address
}

void ssd1306_init_display(I2C_HandleTypeDef *_i2c_handle, uint8_t _i2c_address)
{
	i2c_handle = _i2c_handle;
	i2c_address = _i2c_address;

	ssd1306_send_command(SSD1306_DISPLAYOFF);           // 0xAE

	ssd1306_send_command(SSD1306_SETDISPLAYCLOCKDIV);   // 0xD5
	ssd1306_send_command(0x80);                 		// the suggested ratio 0x80

	ssd1306_send_command(SSD1306_SETMULTIPLEX);         // 0xA8
	ssd1306_send_command(0x3F);

	ssd1306_send_command(SSD1306_SETDISPLAYOFFSET);     // 0xD3
	ssd1306_send_command(0x0);                          // no offset

	ssd1306_send_command(SSD1306_SETSTARTLINE);			// | 0x0); // line #0

	ssd1306_send_command(SSD1306_CHARGEPUMP);           // 0x8D
	ssd1306_send_command(0x14);  						// using internal VCC

	ssd1306_send_command(SSD1306_MEMORYMODE);           // 0x20
	ssd1306_send_command(0x00);          				// 0x00 horizontal addressing

	ssd1306_send_command(SSD1306_SEGREMAP | 0x01); 		// rotate screen 180

	ssd1306_send_command(SSD1306_COMSCANDEC); 			// rotate screen 180

	ssd1306_send_command(SSD1306_SETCOMPINS);           // 0xDA
	ssd1306_send_command(0x12);

	ssd1306_send_command(SSD1306_SETCONTRAST);          // 0x81
	ssd1306_send_command(0x7f);

	ssd1306_send_command(SSD1306_SETPRECHARGE);         // 0xd9
	ssd1306_send_command(0xF1);

	ssd1306_send_command(SSD1306_SETVCOMDETECT);        // 0xDB
	ssd1306_send_command(0x40);

	ssd1306_send_command(SSD1306_DISPLAYALLON_RESUME);	// 0xA4

	ssd1306_send_command(SSD1306_NORMALDISPLAY);		// 0xA6

	ssd1306_send_command(SSD1306_DISPLAYON);			//switch on OLED

	ssd1306_clear_display();
}

uint8_t* ssd1306_get_display_buffer(void)
{
	return display_buffer + 1;
}

void ssd1306_transfer_buffer(void)
{
    // set the Column and Page addresses to 0,0
	ssd1306_set_colAddress();
	ssd1306_set_pageAddress();

  	display_buffer[0] = 0x40;
  	HAL_I2C_Master_Transmit(i2c_handle, i2c_address, display_buffer, DISPLAY_WIDTH * DISPLAY_HEIGTH / 8 + 1, 1000);
}

void ssd1306_clear_display(void)
{
	memset(display_buffer + 1,0,DISPLAY_WIDTH * DISPLAY_HEIGTH / 8);
	ssd1306_transfer_buffer();
}

void ssd1306_clear_buffer(void)
{
	memset(display_buffer + 1,0,DISPLAY_WIDTH * DISPLAY_HEIGTH / 8);
}

void ssd1306_set_contrast(uint8_t value)
{
	ssd1306_send_command(SSD1306_SETCONTRAST);
	ssd1306_send_command(value);
}

void ssd1306_enable_invertmode(uint8_t value)
{
	if(value == 0)
		ssd1306_send_command(SSD1306_NORMALDISPLAY);
	else
		ssd1306_send_command(SSD1306_INVERTDISPLAY);
}

void ssd1306_set_pixel_to_display(uint8_t x, uint8_t y)
{
	if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGTH)
		return;

	uint8_t col = x;
	uint8_t page = y >> 3;	// /8

	// COL Addressing
	ssd1306_send_command(SSD1306_COLUMNADDR);
	ssd1306_send_command(col); 			// Column start address
	ssd1306_send_command(DISPLAY_WIDTH-1); 	// Column end address

	// Page Addressing
	ssd1306_send_command(SSD1306_PAGEADDR); // 0x22 COMMAND
	ssd1306_send_command(page); // Start Page address
	ssd1306_send_command((DISPLAY_HEIGTH/8)-1);// End Page address

	uint16_t buffer_index = page * DISPLAY_WIDTH + x + 1;
	uint8_t c = display_buffer[buffer_index];

	c |= 1 << (y%8);

	display_buffer[buffer_index] = c;

  	ssd1306_send_data(c);
}

void ssd1306_clear_pixel_to_display(uint8_t x, uint8_t y)
{
	if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGTH)
		return;

	uint8_t col = x;
	uint8_t page = y >> 3;	// /8

	// COL Addressing
	ssd1306_send_command(SSD1306_COLUMNADDR);
	ssd1306_send_command(col); 			// Column start address
	ssd1306_send_command(DISPLAY_WIDTH-1); 	// Column end address

	// Page Addressing
	ssd1306_send_command(SSD1306_PAGEADDR); // 0x22 COMMAND
	ssd1306_send_command(page); // Start Page address
	ssd1306_send_command(page);// End Page address

	uint16_t buffer_index = page * DISPLAY_WIDTH + x + 1;
	uint8_t c = display_buffer[buffer_index];
	c &= ~(1 << (y%8));
	display_buffer[buffer_index] = c;

  	ssd1306_send_data(c);
}

void ssd1306_set_pixel_to_buffer(uint8_t x, uint8_t y)
{
	if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGTH)
		return;

	display_buffer[(y >> 3) * DISPLAY_WIDTH + x + 1] |= 1 << (y%8);
}

void ssd1306_clear_pixel_to_buffer(uint8_t x, uint8_t y)
{
	if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGTH)
		return;

	display_buffer[(y >> 3) * DISPLAY_WIDTH + x + 1] &= ~(1 << (y%8));
}
