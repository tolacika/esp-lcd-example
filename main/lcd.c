#include "lcd.h"

static const char *TAG = "I2C_LCD";
static i2c_master_dev_handle_t i2c_device_handle = NULL;
static i2c_master_bus_handle_t i2c_bus_handle = NULL;
static uint8_t lcd_backlight_status = LCD_BACKLIGHT;

// Helper function to toggle the enable bit
static esp_err_t i2c_transmit_with_enable_toggle(uint8_t data) {
    uint8_t data_with_enable = data | LCD_ENABLE;
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_device_handle, &data_with_enable, 1, -1));
    vTaskDelay(pdMS_TO_TICKS(1));

    data_with_enable &= ~LCD_ENABLE;
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_device_handle, &data_with_enable, 1, -1));
    vTaskDelay(pdMS_TO_TICKS(1));

    return ESP_OK;
}

// Send a byte of data to the LCD in 4-bit mode
static esp_err_t i2c_send_byte_on_4bits(uint8_t data, uint8_t rs) {
    uint8_t nibbles[2] = {
        (data & 0xF0) | rs | lcd_backlight_status | LCD_RW_WRITE,
        ((data << 4) & 0xF0) | rs | lcd_backlight_status | LCD_RW_WRITE
    };
    ESP_ERROR_CHECK(i2c_transmit_with_enable_toggle(nibbles[0]));
    ESP_ERROR_CHECK(i2c_transmit_with_enable_toggle(nibbles[1]));
    vTaskDelay(pdMS_TO_TICKS(1));
    ESP_LOGI(TAG, "I2C device data sent: 0x%02X", data);

    return ESP_OK;
}

// Initialize the I2C master
void i2c_master_init(void) {
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle));
    ESP_LOGI(TAG, "I2C bus initialized");

    i2c_device_config_t i2c_device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = LCD_I2C_ADDRESS,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus_handle, &i2c_device_config, &i2c_device_handle));
    ESP_LOGI(TAG, "I2C device added");
    vTaskDelay(pdMS_TO_TICKS(50));  // Wait for LCD to power up
    ESP_LOGI(TAG, "I2C device initialized");
}

// Initialize the LCD
void lcd_init(void) {
    uint8_t init_8bit_commands[] = {
        lcd_backlight_status | LCD_ENABLE_OFF | LCD_RW_WRITE | LCD_RS_CMD,
        0b00110000 | lcd_backlight_status | LCD_RW_WRITE | LCD_RS_CMD,
        0b00110000 | lcd_backlight_status | LCD_RW_WRITE | LCD_RS_CMD,
        0b00110000 | lcd_backlight_status | LCD_RW_WRITE | LCD_RS_CMD,
        0b00100000 | lcd_backlight_status | LCD_RW_WRITE | LCD_RS_CMD,
    };

    uint8_t init_commands[] = {
        //0b00110000, 0b00110000, 0b00110000, 0b00100000, // 8-bit mode to 4-bit mode
        0b00101000, // Function set: 4-bit mode, 2 lines, 5x8 dots
        0b00001100, // Display control: display on, cursor off, blink off
        0b00000001, // Clear display
        0b00000110, // Entry mode set: increment cursor, no shift
        0b00000010, // Set cursor to home position
        0b10000000  // Set cursor to first line
    };

    for (uint8_t i = 0; i < sizeof(init_8bit_commands); i++) {
        ESP_ERROR_CHECK(i2c_transmit_with_enable_toggle(init_8bit_commands[i]));
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    for (uint8_t i = 0; i < sizeof(init_commands); i++) {
        ESP_ERROR_CHECK(i2c_send_byte_on_4bits(init_commands[i], LCD_RS_CMD));
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

// Set cursor position on the LCD
void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;  // Prevent out-of-bounds access
    uint8_t data = 0x80 | (col + row_offsets[row]);
    ESP_ERROR_CHECK(i2c_send_byte_on_4bits(data, LCD_RS_CMD));
}

// Write a string to the LCD
void lcd_write_string(const char *str) {
    while (*str) {
        ESP_ERROR_CHECK(i2c_send_byte_on_4bits((uint8_t)(*str), LCD_RS_DATA));
        str++;
    }
    ESP_LOGI(TAG, "I2C device string sent");
}

// Clear the LCD display
void lcd_clear(void) {
    uint8_t data = 0b00000001;
    ESP_ERROR_CHECK(i2c_send_byte_on_4bits(data, LCD_RS_CMD));
    vTaskDelay(pdMS_TO_TICKS(2));
}

// Control the LCD backlight
void lcd_backlight(bool state) {
    if (state) {
        lcd_backlight_status |= LCD_BACKLIGHT;
    } else {
        lcd_backlight_status &= ~LCD_BACKLIGHT;
    }
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_device_handle, &lcd_backlight_status, 1, -1));
    ESP_LOGI(TAG, "LCD backlight %s", state ? "on" : "off");
}