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

  class Colors {
    
    public:
    
      // Defines a simple color container
      typedef struct _rgb {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
      } rgb;
      
      typedef enum _mode {
        MODE_BLACKOUT,       // Blackout
        MODE_ALARM,          // Colors are set to default values read from EEPROM
        MODE_DIRECTCOLOR,    // Colors are taken from the latest set value
        MODE_WRITEDEFAULT,   // Current colors are stored to the EEPROM
        MODE_STROBE,         // Strobe to latest set colors with par1: tOn and par2: tOff
        MODE_FLASH,          // Single flash to latest set colors with par1: tO
        MODE_SWEEPHUE,       // Sweep hue with timebase set by par1
      } mode;
      
      typedef struct _parameters {
        unsigned char par1;
        unsigned char par2; 
        unsigned char par3;
        unsigned char par4;
        unsigned int hue;
      } parameters;
    
    private:  
      rgb inColors;
      rgb outColors;
      mode workingMode;
      parameters pars;
      unsigned char id;
      
    public:
      Colors();
      ~Colors();
      
      void setId(unsigned char _id);
      
      void setRedColor(unsigned char value);
      void setGreenColor(unsigned char value);
      void setBlueColor(unsigned char value);

      bool setWorkingMode(mode wMode);
      bool setWorkingMode(mode wMode, unsigned char par1, unsigned char par2, unsigned char par3, unsigned char par4);
      void workingTic();
      
      rgb* getCurrentColors();
      
    private:
      void preStateChange(mode nextWorkingMode);
      void postStateChange(mode oldWorkingMode); 
      
      void blackout();
      
      void getDefaultColor();
      void storeDefaultColor();
      
      rgb hsvToRGB(unsigned int hue, unsigned int sat, unsigned int val);
  };

#endif /*  _COLORS_H_ */


