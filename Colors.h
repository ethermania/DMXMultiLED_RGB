/*

485BlockMultiLED - An Atmel328 based multi channel 2.5A 12V strip LED driver
www.EtherMania.com - Marco Signorini - December 2013

Based on Elco Jacobs ShiftPWM library. Please go to www.elcojacobs.com/shiftpwm for documentation.
Based on DMX Library (c) 2011 by Matthias Hertel, http://www.mathertel.de

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/

#ifndef _COLORS_H_
#define _COLORS_H_ 

#include "RGB.h"

  class Colors {
    
    public:
    
      typedef enum _mode {
        MODE_DIRECTCOLOR_RGB,// Colors are taken from the latest RGB set value
        MODE_BLACKOUT,       // Blackout
        MODE_ALARM,          // Colors are set to default values read from EEPROM, at max intensity
        MODE_IDENTIFY,       // Identify this channel (usefull when you have a lot of nodes in DMX)
        MODE_SWEEPHUE,       // Sweep hue with timebase set by par1; par2: saturation; par3: value;
        MODE_SWEEPSAT,       // Sweep saturation with timebase set by par1: par2: hue; par3: value;
        MODE_WRITEDEFAULT,   // Current colors are stored to the EEPROM        
      } mode;
      
      typedef struct _parameters {
        unsigned char par1;
        unsigned char par2; 
        unsigned char par3;
      } parameters;
    
    private:  
      rgb inColors;
      rgb outColors;
      unsigned char brightness;
      unsigned char strobo;
      mode workingMode;
      parameters pars;
      unsigned char prescaler;
      unsigned int timer;
      unsigned char tmrstrobo;
      unsigned char id;
      
    public:
      Colors();
      ~Colors();
      
      void setId(unsigned char _id);
      
      void setBrightness(unsigned char value);
      void setStrobo(unsigned char value);
      
      void setRedColor(unsigned char value);
      void setGreenColor(unsigned char value);
      void setBlueColor(unsigned char value);

      bool setWorkingMode(mode wMode);
      bool setWorkingMode(mode wMode, unsigned char par1, unsigned char par2, unsigned char par3);
      void workingTic();
      
      rgb* getCurrentColors();
      
    private:
      mode preStateChange(mode nextWorkingMode);
      void postStateChange(mode oldWorkingMode); 
      
      boolean applyBrightness(rgb *colors);
      boolean applyStrobo(rgb *colors);
      void blackout(rgb *colors);
      void full(rgb *colors);
      
      void getDefaultColor(rgb *colors);
      void storeDefaultColor(rgb *colors);
      
      rgb hsvToRGB(unsigned int hue, unsigned int sat, unsigned int val);
  };

#endif /*  _COLORS_H_ */


