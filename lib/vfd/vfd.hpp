#ifndef VFD_h
#define VFD_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class VFD
{
public:
    VFD(const unsigned char receivePin, const unsigned char transmitPin);
    ~VFD();
    int print(unsigned char *c, int n); // print at most n characters of c, stop printing at null char, return number of characters printed
private:
    SoftwareSerial* softwareSerial;
};

#endif
