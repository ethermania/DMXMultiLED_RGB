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
#include "RGB.h"

rgb::rgb() {
}

rgb::rgb(rgb& other) {
  red = other.red;
  green = other.green;
  blue = other.blue;
  changed = other.changed;
}

void rgb::setRed(unsigned char _red) {
  if (_red != red) {
    changed = true;
  }
  red = _red;
}

void rgb::setBlue(unsigned char _blue) {
  if (_blue != blue) {
    changed = true;
  }
  blue = _blue;
}

void rgb::setGreen(unsigned char _green) {
  if (_green != green) {
    changed = true;
  }
  green = _green;
}

unsigned char& rgb::getRed() {
  return red;
}

unsigned char& rgb::getBlue() {
  return blue;
}

unsigned char& rgb::getGreen() {
  return green;
}

boolean& rgb::hasChanged() {
  return changed;
}


