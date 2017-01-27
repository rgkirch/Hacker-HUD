#ifndef VFD_h
#define VFD_h

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <WString.h>
#include <string>

class VFD
{
public:
    VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight);
    ~VFD();
    int print(const char *c, int n); // print at most n characters of c, stop printing at null char, return number of characters printed
    void print(std::string str);
private:
    SoftwareSerial* softwareSerial;
    const int width;
    const int height;
};

#endif
