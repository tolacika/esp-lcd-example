# ESP-LCD Example

This repository demonstrates how to use ESP-IDF 5.4.1 with a new I2C driver to control an HD44780 2004A LCD via a PCF8574 I2C I/O expander.

## Features

- **ESP-IDF 5.4.1**: Utilizing the latest ESP-IDF framework.
- **I2C Driver**: Includes a new I2C driver for improved communication.
- **LCD Display**: Control an HD44780 2004A LCD display.
- **PCF8574 I/O Expander**: Interface with the LCD using a PCF8574 I2C I/O expander.

## Requirements

- **ESP32 Development Board**
- **HD44780 2004A LCD Display**
- **PCF8574 I2C I/O Expander**
- **ESP-IDF 5.4.1** or later

## Setup

1. **Clone the Repository**
   ```sh
   git clone https://github.com/tolacika/esp-lcd-example.git
   cd esp-lcd-example
   ```

2. **Set Up ESP-IDF**
   Follow the [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) to set up the ESP-IDF environment.

3. **Build and Flash**
   ```sh
   idf.py build
   idf.py flash
   ```

4. **Monitor**
   ```sh
   idf.py monitor
   ```

## Usage

This example initializes the I2C driver and configures the HD44780 2004A LCD display via the PCF8574 I2C I/O expander. The code demonstrates basic LCD operations such as clearing the display, writing text, and setting the cursor position.

## Code Overview

- **main/lcd.c**: Contains the implementation for the LCD functions.
  - Initializes the I2C master and the LCD display.
  - Provides functions to set the cursor, write strings, clear the display, and control the backlight.
- **main/lcd.h**: Header file for the LCD functions.
  - Defines constants and function prototypes for LCD operations

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests for any improvements.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [ESP-IDF](https://github.com/espressif/esp-idf)
- [HD44780 2004A LCD](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)
- [PCF8574 I2C I/O Expander](https://www.nxp.com/docs/en/data-sheet/PCF8574.pdf)
