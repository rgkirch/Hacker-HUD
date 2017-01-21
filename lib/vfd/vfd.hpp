#ifndef VFD_h
#define VFD_h

#include "../Arduino/libraries/SoftwareSerial/src/SoftwareSerial.h"

class VFD : public SoftwareSerial
{
public:
    VFD(uint8_t receivePin, uint8_t transmitPin);
    ~VFD();
    int print(char* c, int n); // print at most n characters of c, stop printing at null char, return number of characters printed
private:
    SoftwareSerial* softwareSerial;
};

#endif
