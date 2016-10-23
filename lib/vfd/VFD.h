/*
Library for a 2x20 VFD Display using the CD5220 command set
Written by Coyt Barringer on 10/22/16 while he was drunk
it is dependent on SoftwareSerial
*/

#ifndef VFD_h
#define VFD_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class VFD
{
  public:
    VFD(SoftwareSerial&); //int pinRx, int pinTx
    void clearHome();
    void brightness(int);
    void print(String text);
    void print(float text);

  private:
    SoftwareSerial *mySerial;
};

#endif
