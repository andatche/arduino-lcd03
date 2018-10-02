# LCD03 Arduino library

LCD03 is an [Arduino](http://arduino.cc/) library for [I2C](http://en.wikipedia.org/wiki/I²C) control of the LCD03 and LCD05 20x4 and 16x2 serial LCD modules with green and blue backlight color from [Robot Electronics](http://www.robot-electronics.co.uk), see [LCD03](http://www.robot-electronics.co.uk/htm/Lcd03tech.htm) datasheet and the [LCD05](http://www.robot-electronics.co.uk/htm/Lcd05tech.htm ) datasheet for details. It aims to maintain compatibility with the Arduino [LiquidCrystal](http://arduino.cc/en/Reference/LiquidCrystal) library (version 0017 onwards) , though some features of LiquidCrystal are ommited and additonal features are provided. It supports all features of the LCD03 including custom characters and the ability to read the keypad. Supports Arudino 1.0.0 and newer on avr and ESP8266 architectures.

The library is released under the GNU LGPL 2.1.

For source, issues or to contribute, please see the [project page](https://github.com/andatche/arduino-lcd03) on GitHub.

## Downloading

The latest stable release is version 1.1.2 [(zip)](https://github.com/andatche/arduino-lcd03/archive/v1.1.2.zip) [(tar.gz)](https://github.com/andatche/arduino-lcd03/archive/v1.1.2.tar.gz) [(browse)](https://github.com/andatche/arduino-lcd03/tree/v1.1.2).

## Installing

Simply navigate to your Arduino libraries directory and clone the git repository into a directory named 'LCD03'.

```
$ cd ~/Documents/Arduino/libraries
$ git clone git://github.com/andatche/arduino-lcd03.git LCD03
```

To install without git, simple use the "ZIP" link from the GitHub page to grab a .zip of the repository and unzip it to a directory named 'LCD03' in your Arduino libraries directory.

You'll need to restart the Arduino IDE after installing before the library will be detected.

## Functions

The following public functions are exposed.

### LCD03([i2c_address], [i2c_sda], [i2c_scl])

New LCD03 at `i2c_address`. If `i2c_address` is omitted the LCD03's default I2C address (0xC6) is used. On ESP8266 architectures only, `i2c_sda` and `i2c_scl` can optionally be used to set the data (SDA) and clock (SCL) pins used for I2C.

*i2c_address (char): 8-bit I2C address of the display (optional, as displayed during LCD initilisation)*  
*i2c_sda (uint8_t): the data (SDA) line pin for I2C (ESP8266 only, optional, defaults to setting SDA from Wire.h)*  
*i2c_scl (uint8_t): the clock (SCL) line pin for I2C (ESP8266 only, optional, defaults to setting SCL from Wire.h)*  

### begin()

Initialise the display, clear the display and set the cursor to the top-left.

### clear()

Clear the display and return the cursor to the top-left.

### home()

Move the cursor to the top-left.

### setCursor(pos)

Move the cursor to `pos`.

*pos (uint8_t): cursor position (0-79)*

### setCursor(col, row)

Move the cursor to `col`, `row`.

*col (uint8_t): column, Arduino format (0-19)*  
*row (uint8_t): row, Arduino format (0-3)*

### write(data)

Write a single character to the LCD. Custom characters are mapped to values 0-7.

*col (uint8_t): the character to write to the display*

**Returns** *size (size_t): the number of bytes written*

### print(data, [base])

Print text to the LCD.

*data (char, byte, int, long, or string): the data to print to the display*  
*base (optional): the base in which to print numbers: BIN, DEC, OCT or HEX*

**Returns** *size (size_t): the number of bytes written*

### cursor()

Display the underline LCD cursor at the position to which the next character will be written.

### noCursor()

Hides the LCD cursor.

### blink()

Display the blinking (block) LCD cursor at the position to which the next character will be written.

### noBlink()

Turns off the blinking LCD cursor.

### display()

Provided for LiquidCrystal compatibility, turns on the backlight.

### noDisplay()

Provided for LiquidCrystal compatibility, turns off the backlight.

### createChar(num, data[]);

Create a custom character (gylph) for use on the LCD. Up to eight characters of 5x8 pixels are supported (numbered 0 to 7). The appearance of each custom character is specified by an array of eight bytes, one for each row. The five least significant bits of each byte determine the pixels in that row. To display a custom character on the screen, write() its number.

*num (uint8_t): where to store the character (0-7)*  
*data (uint8_t[]): array containing pixel data*

### newLine()

Move the cursor to the first column of the next row.

### cursorDown()

Move the cursor down one row.

### cursorUp()

Move the cursor up one row.

### clearColumn()

Clear the column of the current cursor position.

### backspace()

Delete the character preceding the cursor position and move the cursor back one.

### tab()

Moves the cursor right by the tab space. Tab space is set using `setTab(width)` and default tab space is 4.

### tabSet(width);

Set the tab space.

*width (uint8_t): width of the tab space*

### backlight()

Turn on the LCD backlight.

### noBacklight()

Turn off the LCD backlight.

### disableMessage()

Disables the display of setup information at power up

### enableMessage()

Enables the display of setup information at power up

### saveMessage() - LCD05 only

Saves current screen and displays it on power up

### setType(cols, rows, color) - LCD05 only

Sets which display is connected to the LCD05 controller

*cols (uint8_t): total columns of the display (16/20)*  
*rows (uint8_t): total rows of the display (2/4)*
*color (String): backlight color of the display ('Green'/'Blue')*

### changeAddress(address)

Sets which display is connected to the LCD05 controller

*address (uint8_t): new i2c address of the display, Arduino format (0xC6-0xCE)*

**Possible values for address**: *0xC6, 0xC8,0xCA,0xCC or 0xCE (only even numbers)*

### doubleRateScan()

Increases the frequency of the keypad scan to 20hz

### normalRateScan()

Returns to the default keypad scan frequency of 10hz

### changeContrast(contrast) - LCD05 only

Set the contrast level of the display

*contrast (uint8_t): contrast of the display, Arduino format (0-255)*

### changeBrightness(brightness) - LCD05 only

Set the brightness level of the display

*brightness (uint8_t): brightness of the display, Arduino format (0-255)*

### bufferFreeBytes()

Return the number of free bytes in the LCD03's buffer.

**Returns** *free (uint8_t): the number of free bytes (0-64)*

### readKeypad()

Read the state of the (optionally) attached keypad. For more information see "Reading the keypad" below.

**Returns** *keys (uint16_t): 16-bit state of the keypad matrix*
  
## Example usage

Make sure you have the LCD03 library installed as above. To use LCD03 simply include Wire.h and "LCD03.h" in your sketch and instantiate a new LCD03 instance.

```c++
#include <Wire.h>
#include "LCD03.h"

// Create new LCD03 instance
LCD03 lcd;

void setup() {
  // Initialise the LCD
  lcd.begin();

  // Turn on the backlight
  lcd.backlight();
  
  // Write to the LCD
  lcd.print("Hello world!");

  // Wait for 5 seconds
  delay(5000);

  // Clear the LCD
  lcd.clear();
}

void loop() {
  // Set the cursor to the top left
  lcd.home();

  // Print the uptime in millis
  lcd.print(millis());
}
```

## Reading the keypad

The `readKeypad()` function returns the 2-byte keypad matrix state (see datasheet) in a uint16_t. The library provides some handy definitions which can be used to check the state of a particular key with some simple comparison and bitwise operations.

### Definitions

* KEYPAD_1
* KEYPAD_2
* KEYPAD_3
* KEYPAD_4
* KEYPAD_5
* KEYPAD_6
* KEYPAD_7
* KEYPAD_8
* KEYPAD_9
* KEYPAD_0
* KEYPAD_STAR
* KEYPAD_HASH

### Example usage

The following examples show some common scenarios where you may want to check for particular inputs.

```c++
// Read the current state of the keypad
uint16_t keystate = lcd.readKeypad();

// Check if key 1 is pressed
if(keystate & KEYPAD_1 == KEYPAD_1) {
  // Do something
}

// Check if key 1 is pressed exclusively
if(keystate == KEYPAD_1) {
 // Do something
}

// Check if keys 1 and * are pressed simultaneously
if(keystate & (KEYPAD_1 | KEYPAD_STAR) == (KEYPAD_1 | KEYPAD_2)) {
  // Do something
}

// Check if keys 1 and * are pressed simultaneously exclusively
if(keystate == (KEYPAD_1 | KEYPAD_STAR)) {
  // Do something
}
```

## Contributing

1. Fork it
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Added some feature')
4. Push to the branch (git push origin my-new-feature)
5. Create new Pull Request

## License

Copyright (c) 2013 Ben Arblaster.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
