/*

485BlockMultiLED - An Atmel328 based multi channel 2.5A 12V strip LED driver
www.EtherMania.com - Marco Signorini - December 2013

Based on Elco Jacobs ShiftPWM library. Please go to www.elcojacobs.com/shiftpwm for documentation.
Based on DMX Library (c) 2011 by Matthias Hertel, http://www.mathertel.de

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/
#include <Arduino.h>
#include <EEPROM.h>
#include "Colors.h"

Colors::Colors() {
  
  // Initialize to default parameters
  id = 0;  
  workingMode = MODE_BLACKOUT;
  preStateChange(workingMode);
}

Colors::~Colors() {
}

void Colors::setId(unsigned char _id) {
  id = _id;
}

void Colors::setRedColor(unsigned char value) {
  inColors.red = value;
}

void Colors::setGreenColor(unsigned char value) {
  inColors.green = value;
}

void Colors::setBlueColor(unsigned char value) {
  inColors.blue = value;
}

bool Colors::setWorkingMode(mode wMode, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4) {

  // If a new working mode has ben set, reset the internal state machine  
  if (workingMode != wMode) {
    
    // The following operations should be marked as critical section
    noInterrupts();
      pars.par1 = par1;
      pars.par2 = par2;
      pars.par3 = par3;
      pars.par4 = par4;
      mode oldMode = workingMode;      
      wMode = preStateChange(wMode);
      workingMode = wMode;
      postStateChange(oldMode);
    interrupts();
    
    return true;
  }
  
  return false;
}

bool Colors::setWorkingMode(mode wMode) {
  
  return setWorkingMode(wMode, 0, 0, 0, 0);
}

// Interrupt driven
void Colors::workingTic() {
  
  switch(workingMode) {
    
    // Copy in buffer to out buffer
    case MODE_DIRECTCOLOR_RGB:
      outColors = inColors;
    break;
    
    // RGB are read as HSV
    case MODE_DIRECTCOLOR_HSV:
      outColors = hsvToRGB(inColors.red, inColors.green, inColors.blue);
    break;
    
    // Sweep hue based on par1 timing
    case MODE_SWEEPHUE:
      prescaler++;
      if (prescaler > pars.par1) {
        prescaler = 0;
        
        timer++;
        if (timer == 360) {
          timer = 0;
        }
        outColors = hsvToRGB(timer, pars.par2, pars.par3);        
      }
    break;
    
    case MODE_SWEEPSAT:
      prescaler++;
      if (prescaler > pars.par1) {
        prescaler = 0;
        
        timer++;
        timer = timer & 0x00FF;
        
        outColors = hsvToRGB(pars.par2, timer, pars.par3);
      }
    break;
    
    case MODE_STROBE:
      prescaler++;
      if (prescaler > pars.par1) {
        prescaler = 0;
        timer++;
        if (timer == (pars.par2 + pars.par3)) {
          timer = 0;
        }
        
        if (timer < pars.par2) {
          // Ton period
          outColors = inColors;
        } else {
          // Toff period
          blackout();
      }
    }
    break;
    
    case MODE_FLASH:
      prescaler++;
      if (prescaler > pars.par1) {
        prescaler = 0;
        if (timer > 0) {
          timer--;
          // Ton period
          outColors = inColors;
        } else {
          // Toff always
          blackout();
        }
      }
    break;
  }
  
}

Colors::rgb* Colors::getCurrentColors() {
  return &outColors;
}
      
Colors::mode Colors::preStateChange(mode nextWorkingMode) {
  
  switch(nextWorkingMode) {
    
    case MODE_BLACKOUT:
      blackout();
    break;
    
    case MODE_ALARM:
      getDefaultColor();
    break;
    
    case MODE_WRITEDEFAULT:
      storeDefaultColor();
    break;
    
    case MODE_IDENTIFY:
      // Blink each second
      pars.par1 = 0;
      pars.par2 = 2;
      pars.par4 = 8;
      return MODE_STROBE;
    break;
    
    case MODE_FLASH:
      prescaler = 0;
      timer = pars.par2;
    break;
    
    case MODE_STROBE:
    case MODE_SWEEPHUE:
    case MODE_SWEEPSAT:
      prescaler = 0;
      timer = 0;
    break;
  }
  
  return nextWorkingMode;
}

void Colors::postStateChange(mode oldWorkingMode) {
}

void Colors::blackout() {
  outColors.red = 0;
  outColors.green = 0;
  outColors.blue = 0;
}
      
void Colors::getDefaultColor() {

  unsigned char address = id << 2;
  outColors.red = EEPROM.read(address++);
  outColors.green = EEPROM.read(address++);
  outColors.blue = EEPROM.read(address);
}

void Colors::storeDefaultColor() {
  unsigned char address = id << 2;
  EEPROM.write(address++, inColors.red);
  EEPROM.write(address++, inColors.green);
  EEPROM.write(address++, inColors.blue);
}

Colors::rgb Colors::hsvToRGB(unsigned int hue, unsigned int sat, unsigned int val) {
  unsigned char r = 0,g = 0,b = 0;
  unsigned int H_accent = hue/60;
  unsigned int bottom = ((255 - sat) * val)>>8;
  unsigned int top = val;
  unsigned char rising  = ((top-bottom)  *(hue%60   )  )  /  60  +  bottom;
  unsigned char falling = ((top-bottom)  *(60-hue%60)  )  /  60  +  bottom;
  
  switch(H_accent) {
    case 0:
      r = top; g = rising; b = bottom;
    break;
  
    case 1:
      r = falling; g = top; b = bottom;
    break;
  
    case 2:
      r = bottom; g = top; b = rising;
    break;
  
    case 3:
      r = bottom; g = falling; b = top;
    break;
  
    case 4:
      r = rising; g = bottom; b = top;
    break;
  
    case 5:
      r = top; g = bottom; b = falling;
    break;
  }  
  
  rgb result;
  result.red = r;
  result.green = g;
  result.blue = b;
  
  return result;
}

