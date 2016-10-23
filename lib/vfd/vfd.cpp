/*
Library for a 2x20 VFD Display using the CD5220 command set
Written by Coyt Barringer on 10/22/16 while he was drunk
it is dependent on SoftwareSerial
*/

#include "Arduino.h"
#include "VFD.h"
#include <SoftwareSerial.h>

//constructor
VFD::VFD(SoftwareSerial& my) //int pinRx,int pinTx
{
  mySerial = &my;
  mySerial->begin(9600);

  //together these Initialize the display
  mySerial->write('\x1B');
  mySerial->write('\x40');

  mySerial->write('\x0C'); //clear display
}

//fnc for clearing and resetting cursor to home position
void VFD::clearHome(){
  mySerial->write('\x0C'); //clear display
  mySerial->write('\x0B'); //move cursor to home position
}

//fnc for setting brightness - based on CD5220 POS command set
void VFD::brightness(int value){
  mySerial->write('\x1B');
  mySerial->write('\x2A');
  mySerial->write('\x0' + value); //brightness select 1 <= n <= 4 //mySerial.write('\x02');
}

//fnc for printing a String to display
void VFD::print(String text){
  clearHome();
  mySerial->print(text);
}

//fnc for printing a float to the display
void VFD::print(float text){
  clearHome();
  mySerial->print(text);
}
