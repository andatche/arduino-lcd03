/*
  LCD03.cpp - Arduino library for I2C LCD03 display from Robot Electronics
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
#include "LCD03.h"

#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

char _i2c_address;

#ifdef ARDUINO_ARCH_esp826
  uint8_t _sda_pin;
  uint8_t _scl_pin;

  LCD03::LCD03(char i2c_address, uint8_t sda_pin, uint8_t scl_pin) {
    _sda_pin = sda_pin;
    _scl_pin = scl_pin;
#else
  LCD03::LCD03(char i2c_address) {
#endif
  // Convert 8-bit address from LCD to 7-bit address for Wire
  _i2c_address = i2c_address>>1;
}

void LCD03::begin() {
#ifdef ARDUINO_ARCH_esp826
  Wire.begin(_sda_pin, _scl_pin);
#else
  Wire.begin();
#endif
  noCursor();
  clear();
}

uint8_t LCD03::setType(uint8_t cols, uint8_t rows, String color) {
  uint8_t type = 0;
  if(cols == 20 && rows == 4) {
	if(color == "green" || color == "Green")
      type = 3;
	else if(color == "blue" || color == "Blue")
	  type = 4;
  } else if (cols == 16 && rows == 2) {
	if(color == "green" || color == "Green")
      type = 5;
	else if(color == "blue" || color == "Blue")
	  type = 6;
  }
  
  if(type != 0) {
    Wire.beginTransmission(_i2c_address);
    Wire.write(REG_COMMAND);
    Wire.write(LCD_DISPLAYTYPE);
    Wire.write(type);
    Wire.endTransmission();     
  }
  
  return type;
}

void LCD03::setCursor(uint8_t pos) {
  pos++;
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_CURSORPOS);
  Wire.write(pos);
  Wire.endTransmission();
}

void LCD03::setCursor(uint8_t col, uint8_t row) {
  // convert LiquidCrystal col & rows to LCD03
  col++;
  row++;
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_CURSORPOSXY);
  Wire.write(row);
  Wire.write(col);
  Wire.endTransmission();
}

void LCD03::tabSet(uint8_t ts) {
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_TABSET);
  Wire.write(ts);
  Wire.endTransmission();
}

void LCD03::createChar(uint8_t location, uint8_t charmap[]) {
  // remap custom chars from 0-7 to match LiquidCrystal
  location += LCD_CUSTOMCHAR_BASE;
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_CUSTOMCHAR);
  Wire.write(location);
  for(int i = 0; i < 8; i++) {
    Wire.write(charmap[i] |= LCD_CUSTOMCHAR_MASK);
  }
  Wire.endTransmission();
}

// get the free buffer capacity in bytes
uint8_t LCD03::bufferFreeBytes() {
  Wire.requestFrom(_i2c_address, 1);
  if(Wire.available()) {
    return Wire.read();
  }
}

uint16_t LCD03::readKeypad () {  
  Wire.requestFrom(_i2c_address, 3);
  uint8_t lowByte;
  uint8_t highByte;
  uint16_t bothBytes;
 
  // Don't care about the first byte
  Wire.read();
  // Read the low and high bytes
  lowByte = Wire.read();
  highByte = Wire.read();

  bothBytes = (lowByte | highByte << 8);

  return bothBytes;
}

// write a single byte in one transmission
size_t LCD03::write(uint8_t value) {
  // remap custom chars to 0-7 to match LiquidCrystal
  if(value < 8) {
    value += LCD_CUSTOMCHAR_BASE;
  }
  send(value);
  // assume write was a success
  return 1;
}

// write multiple bytes in as many transmissions as required
size_t LCD03::write(const uint8_t *buffer, size_t size) {
  Wire.beginTransmission(_i2c_address);
  //wait until the buffer is empty enough
  while(bufferFreeBytes() < BUFFER_LENGTH);
  Wire.write(REG_COMMAND);
  for(int i = 0; i < size; i++) {    
    // we've filled the I2C buffer, flush it and start a new transmission
    if(i != 0 && i % (BUFFER_LENGTH-1) == 0) {
      Wire.endTransmission();
      //wait until the buffer is empty enough
      while(bufferFreeBytes() < BUFFER_LENGTH);
      Wire.beginTransmission(_i2c_address);
      Wire.write(REG_COMMAND);
    }
    Wire.write(buffer[i]);
  }
  Wire.endTransmission();
  // assume write was a success
  return size;
}

void LCD03::changeAddress(uint8_t address) {
  if(address >= 0xC6 && address <= 0xCE) {
	if(address % 2 == 0) {
	  Wire.beginTransmission(_i2c_address);
	  Wire.write(REG_COMMAND);
	  Wire.write(LCD_CHANGEADDRESS);
	  Wire.write(0xA0);
	  Wire.write(0xAA);
	  Wire.write(0xA5);
	  Wire.write(address);
	  Wire.endTransmission();

	  _i2c_address = address >> 1;
	}
  }
}

void changeContrast(uint8_t contrast) {
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_CONTRASTSET);
  Wire.write(contrast);
  Wire.endTransmission();
}

void changeBrightness(uint8_t brightness) {
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(LCD_BRIGHTNESSSET);
  Wire.write(brightness);
  Wire.endTransmission();
}

// send a single byte command in a single transmission
void LCD03::send(uint8_t command) {
  Wire.beginTransmission(_i2c_address);
  Wire.write(REG_COMMAND);
  Wire.write(command);
  Wire.endTransmission();
}
