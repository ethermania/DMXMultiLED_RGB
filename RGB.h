/*

485BlockMultiLED - An Atmel328 based multi channel 2.5A 12V strip LED driver
www.EtherMania.com - Marco Signorini - December 2013

Based on Elco Jacobs ShiftPWM library. Please go to www.elcojacobs.com/shiftpwm for documentation.
Based on DMX Library (c) 2011 by Matthias Hertel, http://www.mathertel.de

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/

#ifndef _RGB_H_
#define _RGB_H_ 

  class rgb {
    
    private:
      unsigned char red;
      unsigned char blue;
      unsigned char green;
      boolean changed;
      
    public:
      rgb();
      rgb(rgb& other);
      
      void setRed(unsigned char _red);
      void setBlue(unsigned char _blue);
      void setGreen(unsigned char _green);
      
      unsigned char& getRed();
      unsigned char& getBlue();
      unsigned char& getGreen();
      
      boolean& hasChanged();
  };    

#endif /*  _RGB_H_ */


