/* main work by https://controllerstech.com/i2c-lcd-in-stm32/
 *
 * added by maxl95:
 * - variable I2C address
 * - backlight control
 * - lcd-clear function with HD44780 commands, not by a for-loop
 * - lcd-test function to test every single pixel
 *
 *
 * */
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

// #define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup
char LCD_SLAVE_ADDRESS;
char LCD_BACKLIGHT = 0x00;

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0

	if(LCD_BACKLIGHT == 0) {
		for(uint8_t i = 0; i <= 3; i++) {
			data_t[i] &= ~(1UL << 3);
		}
	} else {
		for(uint8_t i = 0; i <= 3; i++) {
			data_t[i] |= 1UL << 3;
		}
	}

	HAL_I2C_Master_Transmit (&hi2c1, LCD_SLAVE_ADDRESS,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0

	if(LCD_BACKLIGHT == 0) {
		for(uint8_t i = 0; i <= 3; i++) {
			data_t[i] &= ~(1UL << 3);
		}
	} else {
		for(uint8_t i = 0; i <= 3; i++) {
			data_t[i] |= 1UL << 3;
		}
	}

	HAL_I2C_Master_Transmit (&hi2c1, LCD_SLAVE_ADDRESS,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
	/*
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}*/
	lcd_send_cmd(0x01);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
        case 2:
            col |= 0x90;
            break;
        case 3:
            col |= 0xD0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (char *addr)
{
	LCD_SLAVE_ADDRESS = addr;
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_test (void)
{
	for(int r = 0; r < 4; r++) {
		lcd_put_cur(r, 0);
		for(int s = 0; s < 16; s++) {
			lcd_send_data (0xFF);
			HAL_Delay(10);
		}
	}
}

void lcd_backlight (char state)
{
	LCD_BACKLIGHT = state;
	lcd_send_cmd(0x00);
}
