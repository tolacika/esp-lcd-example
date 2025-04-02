#ifndef LCD_H
#define LCD_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

// I2C configuration
#define I2C_MASTER_NUM        I2C_NUM_0
#define I2C_MASTER_SDA_IO     14
#define I2C_MASTER_SCL_IO     15
#define I2C_MASTER_FREQ_HZ    100000
#define LCD_I2C_ADDRESS       0x27    // Adjust to your PCF8574 address

// LCD commands
#define WRITE_BIT           I2C_MASTER_WRITE

#define LCD_BACKLIGHT (1 << 3) // Backlight bit
#define LCD_ENABLE (1 << 2)   // Enable bit
#define LCD_ENABLE_OFF (0 << 2) // Enable off
#define LCD_RW (1 << 1)      // Read/Write bit
#define LCD_RW_WRITE (0 << 1) // Write mode
#define LCD_RW_READ (1 << 1)  // Read mode
#define LCD_RS (1 << 0)      // Register Select bit
#define LCD_RS_CMD (0 << 0)    // Command mode
#define LCD_RS_DATA (1 << 0)   // Data mode
#define LCD_DB7 (1 << 7) // Data bit 7
#define LCD_DB6 (1 << 6) // Data bit 6
#define LCD_DB5 (1 << 5) // Data bit 5
#define LCD_DB4 (1 << 4) // Data bit 4

// Function prototypes
void i2c_master_init(void);
void lcd_init(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_write_string(const char *str);
void lcd_clear(void);
void lcd_backlight(bool state);

#endif // LCD_H