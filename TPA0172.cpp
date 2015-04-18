/*
 * See header file for details
 *
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * 
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * 
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 */

/* Dependencies */
#include <Wire.h>
#include "TPA0172.h"

TPA::TPA()
{
}


void TPA::begin(uint8_t address) {

	/* Store the I2C address and init the Wire library */
	_address = address;
	Wire.begin();
}

void TPA::begin(uint8_t address, uint8_t Volume, uint8_t Control) {

  /* Store the I2C address and init the Wire library */
	_address = address;
	Wire.begin();
	
	get_registers();
	if(Volume > 0x3F)
	  Volume = 0x3F;
	  
	_TPA_Regs[RIGHTGAIN_BTL] = Volume;
	_TPA_Regs[LEFTGAIN_BTL] = Volume;
	_TPA_Regs[RIGHTGAIN_SE] = Volume;
	_TPA_Regs[LEFTGAIN_SE] = Volume;
	_TPA_Regs[MASK_REGISTER] = 0xFF;
	_TPA_Regs[CONTROL_REGISTER] = Control;
	
	write_registers();
	
}

void TPA::get_registers(void) {
  Wire.beginTransmission(_address);  // Start transmission
  
  Wire.requestFrom((uint8_t)_address, (uint8_t)6);
  for(uint8_t ndx=0; ndx < 6; ndx++) {
    _TPA_Regs[ndx] = (uint8_t)I2CREAD();
  }
  Wire.endTransmission();           // Stop transmission
  
  #ifdef DEBUGPRINT
    Serial.println("Get Registers");
	for(uint8_t ndx=0; ndx < 6; ndx++) {
	  Serial.print(_TPA_Regs[ndx], HEX);
	  Serial.print(' ');
	}
	Serial.println();
  #endif
}


void TPA::write_registers(void) {
  Wire.beginTransmission(_address);  // Start transmission
  for(uint8_t ndx=0; ndx < 6; ndx++) {
    I2CWRITE((uint8_t)_TPA_Regs[ndx]);
  }
  Wire.endTransmission();           // Stop transmission
  
  #ifdef DEBUGPRINT
    Serial.println("Write Registers");
	for(uint8_t ndx=0; ndx < 6; ndx++) {
	  Serial.print(_TPA_Regs[ndx], HEX);
	  Serial.print(' ');
	}
	Serial.println();
  #endif
}


void TPA::setgain(uint8_t reg, uint8_t gain) {

  get_registers();
  
  if(gain > 0x3F)
	  gain = 0x3F;

  _TPA_Regs[reg] = gain;
  
  write_registers();
}


uint8_t TPA::getgain(uint8_t reg) {

  get_registers();
  
  return((uint8_t) _TPA_Regs[reg]&0x3F);
}


void TPA::setvolume(uint8_t reg, uint8_t Volume) {

  get_registers();
  
  if(Volume > 0x3F)
	  Volume = 0x3F;
  
  if(reg == BTL_GAIN) {
    _TPA_Regs[RIGHTGAIN_BTL] = Volume;
	_TPA_Regs[LEFTGAIN_BTL] = Volume;
  }
  
  if(reg == SE_GAIN) {
    _TPA_Regs[RIGHTGAIN_SE] = Volume;
	_TPA_Regs[LEFTGAIN_SE] = Volume;
  }
  
  write_registers();
}


void TPA::Decrement_Volume(uint8_t reg, uint8_t step) {

  get_registers();
  
  if(reg == BTL_GAIN) {
    if((_TPA_Regs[RIGHTGAIN_BTL] + step) > 0x3F) {
	  _TPA_Regs[RIGHTGAIN_BTL] = 0x3F;
	} else {
      _TPA_Regs[RIGHTGAIN_BTL] += step;
	}
	
	if((_TPA_Regs[LEFTGAIN_BTL] + step) > 0x3F) {
	  _TPA_Regs[LEFTGAIN_BTL] = 0x3F;
	} else {
      _TPA_Regs[LEFTGAIN_BTL] += step;
	}
  }
  
  if(reg == SE_GAIN) {
    if((_TPA_Regs[RIGHTGAIN_SE] + step) > 0x3F) {
	  _TPA_Regs[RIGHTGAIN_SE] = 0x3F;
	} else {
      _TPA_Regs[RIGHTGAIN_SE] += step;
	}
	
	if((_TPA_Regs[LEFTGAIN_SE] + step) > 0x3F) {
	  _TPA_Regs[LEFTGAIN_SE] = 0x3F;
	} else {
      _TPA_Regs[LEFTGAIN_SE] += step;
	}  
  }
  
  write_registers();
}


void TPA::Increment_Volume(uint8_t reg, uint8_t step) {

  get_registers();
  
  if(reg == BTL_GAIN) {
    if(_TPA_Regs[RIGHTGAIN_BTL] <= step) {
	  _TPA_Regs[RIGHTGAIN_BTL] = 0x00;
	} else {
      _TPA_Regs[RIGHTGAIN_BTL] -= step;
	}
	
	if(_TPA_Regs[LEFTGAIN_BTL] <= step) {
	  _TPA_Regs[LEFTGAIN_BTL] = 0x00;
	} else {
      _TPA_Regs[LEFTGAIN_BTL] -= step;
	}
  }
  
  if(reg == SE_GAIN) {
    if(_TPA_Regs[RIGHTGAIN_SE] <= step) {
	  _TPA_Regs[RIGHTGAIN_SE] = 0x00;
	} else {
      _TPA_Regs[RIGHTGAIN_SE] -= step;
	}
	
	if(_TPA_Regs[LEFTGAIN_SE] <= step) {
	  _TPA_Regs[LEFTGAIN_SE] = 0x00;
	} else {
      _TPA_Regs[LEFTGAIN_SE] -= step;
	}  
  }
  
  write_registers();
}

void TPA::setmask(uint8_t mask) {

  get_registers();

  _TPA_Regs[MASK_REGISTER] = mask;
  
  write_registers();
}


uint8_t TPA::getmask(void) {

  get_registers();
  
  return((uint8_t)_TPA_Regs[MASK_REGISTER]);
}


void TPA::setcontrol(uint8_t control) {

  get_registers();

  _TPA_Regs[CONTROL_REGISTER] = control;
  
  write_registers();
}


uint8_t TPA::getcontrol(void) {

  get_registers();
  
  return((uint8_t)_TPA_Regs[CONTROL_REGISTER]);
}


void TPA::mute(void) {

  get_registers();
  
  _TPA_Regs[CONTROL_REGISTER] |= MUTE;

  write_registers();
}


void TPA::unmute(void) {

  get_registers();
  
  _TPA_Regs[CONTROL_REGISTER] &= UNMUTE;

  write_registers();
}