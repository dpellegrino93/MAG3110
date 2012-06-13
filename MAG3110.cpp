#include <Wire.h>
#include "MAG3110.h"

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


MAG3110::MAG3110()
{
	mx = my = mz = 0;
	temp = 0;
}

void MAG3110::writeTo(byte address, byte val)
{
	Wire.beginTransmission(MAG3110_ADDR);
	Wire.write(address);             
	Wire.write(val);                 
	Wire.endTransmission();
}

void MAG3110::readFrom(byte address, int num, byte buff[])
{
	Wire.beginTransmission(MAG3110_ADDR);
	Wire.write(address);
	Wire.endTransmission();
	
	Wire.beginTransmission(MAG3110_ADDR);
	Wire.requestFrom(MAG3110_ADDR, num);
	
	int i = 0;
	while(Wire.available())
	{ 
		buff[i] = Wire.read();
		i++;
	}
	Wire.endTransmission();
}


void MAG3110::setRegisterBit(byte regAdress, int bitPos, bool state) 
{
	byte _b;
	readFrom(regAdress, 1, &_b);
	if (state) {
		_b |= (1 << bitPos);
	} 
	else {
		_b &= ~(1 << bitPos);
	}
	writeTo(regAdress, _b);  
}

bool MAG3110::getRegisterBit(byte regAdress, int bitPos) 
{
	byte _b;
	readFrom(regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}


void MAG3110::init(byte sysmod)
{
	Wire.begin();
	setRegisterBit(MAG3110_REG_CTRL_REG2,
				   MAG3110_BIT_AUTO_RESET,
				   1);
	switch(sysmod)
	{
		case SYSMOD_STANDBY:
			setRegisterBit(MAG3110_REG_CTRL_REG1,
						   MAG3110_BIT_ACTIVE,
						   0);
			break;
		case SYSMOD_ACTIVE_RAW:
			setRegisterBit(MAG3110_REG_CTRL_REG1,
						   MAG3110_BIT_ACTIVE,
						   1);
			setRegisterBit(MAG3110_REG_CTRL_REG2,
						   MAG3110_BIT_RAW,
						   1);
			break;
		case SYSMOD_ACTIVE:
			setRegisterBit(MAG3110_REG_CTRL_REG1,
						   MAG3110_BIT_ACTIVE,
						   1);
			setRegisterBit(MAG3110_REG_CTRL_REG2,
						   MAG3110_BIT_RAW,
						   0);
			break;
	}
}

void MAG3110::read()
{
	byte buf[6];
	readFrom(MAG3110_REG_OUT_X_MSB,6,buf);
	mx = buf[0]<<8 | buf[1];
	my = buf[2]<<8 | buf[3];
	mz = buf[4]<<8 | buf[5];
	readFrom(MAG3110_REG_DIE_TEMP,6,&temp);
}

void MAG3110::reset()
{
	setRegisterBit(MAG3110_REG_CTRL_REG2,
				   MAG3110_BIT_RESET,
				   true);
}

void MAG3110::setDataRate(byte dataRate, byte osRatio)
{
	byte dr;
	readFrom(MAG3110_REG_CTRL_REG1, 1, &dr);
	
	dr &= ~(MAG3110_MASK_DR | MAG3110_MASK_OSR);
	dr |= dataRate;
	dr |= osRatio;

	writeTo(MAG3110_REG_CTRL_REG1, dr);

}


void MAG3110::setOperatingMode(bool isActive)
{
	setRegisterBit(MAG3110_REG_CTRL_REG1,
				   MAG3110_BIT_ACTIVE,
				   isActive);
}

bool MAG3110::getOperatingMode()
{
	return getRegisterBit(MAG3110_REG_CTRL_REG1,
				   MAG3110_BIT_ACTIVE);
}

byte MAG3110::getSysMod()
{
	byte out;
	readFrom(MAG3110_REG_SYSMOD,1,&out);
	return out;
}

void MAG3110::setRawMode(bool isRaw)
{
	setRegisterBit(MAG3110_REG_CTRL_REG2,
				   MAG3110_BIT_RAW,
				   isRaw?1:0);
}

bool MAG3110::getRawMode()
{
	return getRegisterBit(MAG3110_REG_CTRL_REG2,
				   MAG3110_BIT_RAW);

}


//Impostazione offset non funziona
void  MAG3110::setOffsetX(int offset)
{
	writeTo(MAG3110_REG_OFF_X_MSB, offset<<9);
	writeTo(MAG3110_REG_OFF_X_LSB, offset<<1);
}

void  MAG3110::setOffsetY(int offset)
{
	writeTo(MAG3110_REG_OFF_Y_MSB, offset<<9);
	writeTo(MAG3110_REG_OFF_Y_LSB, offset<<1);
}

void  MAG3110::setOffsetZ(int offset)
{
	writeTo(MAG3110_REG_OFF_Z_MSB, offset<<9);
	writeTo(MAG3110_REG_OFF_Z_LSB, offset<<1);
}