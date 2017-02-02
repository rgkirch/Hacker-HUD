#ifndef VFD_h
#define VFD_h

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <WString.h>
#include <string>
#include <memory>

class VFD
{
public:
    class Builder;
private:
    SoftwareSerial* softwareSerial;
    const int width;
    const int height;
public:
    VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight);
    ~VFD();
    int print(const char *c, int n); // print at most n characters of c, stop printing at null char, return number of characters printed
    void print(std::string str);
    void println(std::string str);
    void write(char c);
    void clear();
};

class VFD::Builder
{
private:
    int rx = -1;
    int tx = -1;
    int displayWidth = 20;
    int displayHeight = 2;
public:
    Builder() = default;
    std::unique_ptr<VFD> build();
    VFD::Builder& setRx(int rx);
    VFD::Builder& setTx(int tx);
    VFD::Builder& setDisplayWidth(int displayWidth);
    VFD::Builder& setDisplayHeight(int displayHeight);
};

#endif
