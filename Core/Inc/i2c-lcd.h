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

#include "stm32f1xx_hal.h"

void lcd_init (char *addr);   // initialize lcd with given i2c address

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_test(void);	// pixel test of the whole display

void lcd_clear (void);	// clears the display with the specified command of the HD44780, not by a for-loop

void lcd_backlight (char state);	// set backlight 0 or 1
