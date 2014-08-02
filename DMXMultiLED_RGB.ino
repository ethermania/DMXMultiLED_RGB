/*

485BlockMultiLED - An Atmel328 based multi channel 2.5A 12V strip LED driver
www.EtherMania.com - Marco Signorini - December 2013

Based on Elco Jacobs ShiftPWM library. Please go to www.elcojacobs.com/shiftpwm for documentation.
Based on DMX Library (c) 2011 by Matthias Hertel, http://www.mathertel.de

This code is provided under the Creative Commons Attribution License. More information can be found here:
http://creativecommons.org/licenses/by/3.0/

(Use this code freely! Please just remember to give us credit where it's due. Thanks!)

*/

#define RS485_TXE_PIN  2
#define INPUT_1    3
#define INPUT_2    4
#define INPUT_3    5
#define BUTTON_1  A0  

#define SHIFTPWM_USE_TIMER2

const int ShiftPWM_latchPin=8;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h> 
#include <EEPROM.h>
#include <DMXSerial.h>
#include <TimerOne.h>
#include "Colors.h"

#define MAXBRIGHTNESS 255
#define PWMFREQUENCY 75
#define NUMREGISTERS 2
#define NUMRGBLEDS (NUMREGISTERS*8/3)

#define DEFAULTDMXTIMEOUT 5000
#define MAXCHANNELS 4

Colors colorEngine[MAXCHANNELS];

unsigned short dmxChannelBase = 1;

void setup() {
  
  // Initialize the DMX library as controller
  DMXSerial.init(DMXReceiver);
  
  // Turn off RS485 transceiver
  pinMode(RS485_TXE_PIN, OUTPUT);
  digitalWrite(RS485_TXE_PIN, LOW);
  
  // Initialize input lines
  pinMode(INPUT_1, INPUT);
  pinMode(INPUT_2, INPUT);
  pinMode(INPUT_3, INPUT);
  
  // Initialize the onboard pushbutton
  pinMode(BUTTON_1, INPUT);
  
  // Initialize the color engine pool
  colorEngine[1].setId(1);
  colorEngine[2].setId(2);
  colorEngine[3].setId(3);
  
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(NUMREGISTERS);
 
  // SetPinGrouping allows flexibility in LED setup. 
  // We use RGBRGBRGBRGB
  ShiftPWM.SetPinGrouping(1); 
  
  // Start the ShiftPWM library
  ShiftPWM.Start(PWMFREQUENCY,MAXBRIGHTNESS);  
  
  // We use the Timer1 for color engine state machine handling
  Timer1.initialize(10000);  // each 10ms
  Timer1.attachInterrupt(timer1Callback);
}

void loop()
{
  // Check if DMX cable is connected
  unsigned long lastPacket = DMXSerial.noDataSince();
  
    if (lastPacket < DEFAULTDMXTIMEOUT) {
      
      // Read data from DMX bus and update the color engines
      unsigned int dmxCh = dmxChannelBase;
      for (unsigned char channel = 0; channel < MAXCHANNELS; channel++) {
        colorEngine[channel].setBrightness(DMXSerial.read(dmxCh++));
        colorEngine[channel].setRedColor(DMXSerial.read(dmxCh++));
        colorEngine[channel].setGreenColor(DMXSerial.read(dmxCh++));
        colorEngine[channel].setBlueColor(DMXSerial.read(dmxCh++));
        colorEngine[channel].setStrobo(DMXSerial.read(dmxCh++));
        
        Colors::mode wMode = (Colors::mode)(DMXSerial.read(dmxCh++)/10);
        unsigned char par1 = DMXSerial.read(dmxCh++);
        unsigned char par2 = DMXSerial.read(dmxCh++);
        unsigned char par3 = DMXSerial.read(dmxCh++);
        colorEngine[channel].setWorkingMode(wMode, par1, par2, par3);
      }
      
    } else {

      // Set color engines in alarm mode
      for (unsigned char channel = 0; channel < MAXCHANNELS; channel++) {
        colorEngine[channel].setWorkingMode(Colors::MODE_ALARM);
      }

  }
  

  // Update the outputs if needed
  for (unsigned char channel = 0; channel < MAXCHANNELS; channel++) {
    rgb *color;
    color = colorEngine[channel].getCurrentColors();
    
    if(color->hasChanged()) {
      ShiftPWM.SetRGB(channel, color->getRed(), color->getGreen(), color->getBlue());
    }
  }

}

// We use the timer 1 as a clock reference for the color engines
void timer1Callback() {
  for (unsigned char channel = 0; channel < MAXCHANNELS; channel++) {
    colorEngine[channel].workingTic();
  }
}


