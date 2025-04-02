#include "lcd.h"

void app_main() {
    i2c_master_init();
    lcd_init();
    lcd_backlight(true);

    lcd_set_cursor(0, 0);
    lcd_write_string("Hello, ESP32!");
    lcd_set_cursor(0, 1);
    lcd_write_string("20x4 LCD Display");
}