#include "Arduino.h"

/*
  ##########################
  # Arduino MAG3110 driver #
  ##########################
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
*/
#ifndef mag3110_h
#define mag3110_h

#define BITMASK(a) 1<<a

#define MAG3110_ADDR 0x0E

//Registri
#define MAG3110_REG_WHO_AM_I	0x07

#define MAG3110_REG_DR_STATUS	0x00
#define MAG3110_REG_OUT_X_MSB	0x01
#define MAG3110_REG_OUT_X_LSB	0x02
#define MAG3110_REG_OUT_Y_MSB	0x03
#define MAG3110_REG_OUT_Y_LSB	0x04
#define MAG3110_REG_OUT_Z_MSB	0x05
#define MAG3110_REG_OUT_Z_LSB	0x06

#define MAG3110_REG_OFF_X_MSB	0x09
#define MAG3110_REG_OFF_X_LSB	0x0A
#define MAG3110_REG_OFF_Y_MSB	0x0B
#define MAG3110_REG_OFF_Y_LSB	0x0C
#define MAG3110_REG_OFF_Z_MSB	0x0D
#define MAG3110_REG_OFF_Z_LSB	0x0E

#define MAG3110_REG_DIE_TEMP	0x0F
#define MAG3110_REG_CTRL_REG1	0X10
#define MAG3110_REG_CTRL_REG2	0X11
#define MAG3110_REG_SYSMOD		0x08

//maschere
#define MAG3110_MASK_DR			0xE0
#define MAG3110_MASK_OSR		0x18
//posizioni bit
#define MAG3110_BIT_ACTIVE		0x0
#define MAG3110_BIT_AUTO_RESET	0x7
#define MAG3110_BIT_RAW			0x5
#define MAG3110_BIT_RESET		0x4

//Valori
#define SYSMOD_STANDBY		0x0
#define SYSMOD_ACTIVE_RAW	0x1
#define SYSMOD_ACTIVE		0x2
#define DR_0				B00000000
#define DR_1				B00100000
#define DR_2				B01000000
#define DR_3				B01100000
#define DR_4				B10000000
#define DR_5				B10100000
#define DR_6				B11000000
#define DR_7				B11100000
#define OSR_16				B00000000
#define OSR_32				B00001000
#define OSR_64				B00010000
#define OSR_128				B00011000

class MAG3110
{
public:
	int mx;
	int my;
	int mz;
	byte temp;

	MAG3110();
	void init(byte sysmod);
	void read();
	void reset();

	void setDataRate(byte dataRate, byte osRatio);

	void setOffsetX(int offset);
	void setOffsetY(int offset);
	void setOffsetZ(int offset);

	void setOperatingMode(bool isActive);
	bool getOperatingMode();

	byte getSysMod();

	void setRawMode(bool isRaw);
	bool getRawMode();

	void writeTo(byte address, byte val);
	void readFrom(byte address, int num, byte buff[]);
	void setRegisterBit(byte regAdress, int bitPos, bool state);
	bool getRegisterBit(byte regAdress, int bitPos);  

};
#endif