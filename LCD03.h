/*
  LCD03.h - Arduino library for I2C LCD03 display from Robot Electronics
  see http://www.robot-electronics.co.uk/htm/Lcd03tech.htm
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
*/
#ifndef LCD03_h
#define LCD03_h

#include <inttypes.h>
#include <Wire.h>
#include "Print.h"

// default i2c address
#define I2C_ADDR 0xC6

// registers
#define REG_COMMAND 0x00
#define REG_KEYPADLOW 0x01
#define REG_KEYPADHIGH 0x02
#define REG_VERSION 0x03

// commands
#define LCD_NOOP 0x00
#define LCD_CURSORHOME 0x01
#define LCD_CURSORPOS 0x02
#define LCD_CURSORPOSXY 0x03
#define LCD_CURSOROFF 0x04
#define LCD_CURSORON 0x05
#define LCD_CURSORBLINK 0x06
#define LCD_BACKSPACE 0x08
#define LCD_TAB 0x09
#define LCD_CURSORDOWN 0x0A
#define LCD_CURSORUP 0x0B
#define LCD_CLEARDISPLAY 0x0C
#define LCD_LINEFEED 0x0D
#define LCD_CLEARCOLUMN 0x11
#define LCD_TABSET 0x12
#define LCD_BACKLIGHTON 0x13
#define LCD_BACKLIGHTOFF 0x14
#define LCD_DISABLEMESSAGE 0x15
#define LCD_ENABLEMESSAGE 0x16
#define LCD_SAVEMESSAGE 0x17
#define LCD_DISPLAYTYPE 0x18
#define LCD_CHANGEADDRESS 0x19
#define LCD_CUSTOMCHAR 0x1B
#define LCD_DOUBLERATESCAN 0x1C
#define LCD_NORMALRATESCAN 0x1D
#define LCD_CONTRASTSET 0x1E
#define LCD_BRIGHTNESSSET 0x1E

// custom chars
#define LCD_CUSTOMCHAR_BASE 0x80
#define LCD_CUSTOMCHAR_MASK 0b11100000

// keypad values
#define KEYPAD_1 0x01
#define KEYPAD_2 0x02
#define KEYPAD_3 0x04
#define KEYPAD_4 0x08
#define KEYPAD_5 0x10
#define KEYPAD_6 0x20
#define KEYPAD_7 0x40
#define KEYPAD_8 0x80
#define KEYPAD_9 0x100
#define KEYPAD_STAR 0x200
#define KEYPAD_0 0x400
#define KEYPAD_HASH 0x800

class LCD03 : public Print {
public:
  // Constructors
#ifdef ARDUINO_ARCH_esp8266
  LCD03(char i2c_address=I2C_ADDR, uint8_t sda_pin=SDA, uint8_t scl_pin=SCL);
#else
  LCD03(char i2c_address=I2C_ADDR);
#endif
  // LiquidCrystal compatible functions
  void begin();
  void clear();
  void home();
  uint8_t setType(uint8_t cols, uint8_t rows, String color);
  void setCursor(uint8_t);
  void setCursor(uint8_t, uint8_t);
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  void cursor();
  void noCursor(); 
  void blink();
  void noBlink();
  void display();
  void noDisplay();
  void createChar(uint8_t, uint8_t[]);

  // Unimplemented LiquidCrystal functions
  //void scrollDisplayLeft();
  //void scrollDisplayRight();
  //void autoscroll();
  //void noAutoscroll();
  //void leftToRight();
  //void rightToLeft();

  // LCD03 extras
  void newLine();
  void cursorDown();
  void cursorUp();
  void clearColumn();
  void backspace();
  void tab();
  void tabSet(uint8_t);
  void backlight();
  void noBacklight();
  void disableMessage();
  void enableMessage();
  void saveMessage();
  void doubleRateScan();
  void normalRateScan();
  void changeContrast(uint8_t);
  void changeBrightness(uint8_t);
  void changeAddress(uint8_t);
  uint8_t bufferFreeBytes();
  uint16_t readKeypad();

private:
  char _i2c_address;

  void send(uint8_t);
};

inline void LCD03::clear() {
  send(LCD_CLEARDISPLAY);
}

inline void LCD03::backlight() {
  send(LCD_BACKLIGHTON);
}

inline void LCD03::noBacklight() {
  send(LCD_BACKLIGHTOFF);
}

// provided for compatibility with LiquidCrystal - only backlight is switched
inline void LCD03::display() {
  send(LCD_BACKLIGHTON);
}

// provided for compatibility with LiquidCrystal - only backlight is switched
inline void LCD03::noDisplay() {
  send(LCD_BACKLIGHTOFF);
}

inline void LCD03::home() {
  send(LCD_CURSORHOME);
}

inline void LCD03::noBlink() {
  send(LCD_CURSOROFF);
}

inline void LCD03::blink() {
  send(LCD_CURSORBLINK);
}

inline void LCD03::noCursor() {
  send(LCD_CURSOROFF);
}

inline void LCD03::cursor() {
  send(LCD_CURSORON);
}

inline void LCD03::newLine() {
  send(LCD_LINEFEED);
}

inline void LCD03::cursorDown() {
  send(LCD_CURSORDOWN);
}

inline void LCD03::cursorUp() {
  send(LCD_CURSORUP);
}

inline void LCD03::clearColumn() {
  send(LCD_CLEARCOLUMN);
}

inline void LCD03::backspace() {
  send(LCD_BACKSPACE);
}

inline void LCD03::tab() {
  send(LCD_TAB);
}

inline void LCD03::disableMessage() {
  send(LCD_DISABLEMESSAGE);
}

inline void LCD03::enableMessage() {
  send(LCD_ENABLEMESSAGE);
}

inline void LCD03::saveMessage() {
  send(LCD_SAVEMESSAGE);
}

inline void LCD03::doubleRateScan() {
  send(LCD_DOUBLERATESCAN);
}

inline void LCD03::normalRateScan() {
  send(LCD_NORMALRATESCAN);
}

#endif
