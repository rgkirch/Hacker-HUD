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
    void clear();
    template <typename T> void write(T p) { softwareSerial->write(p); }
    template <typename T> void print(T p) { softwareSerial->print(p); }
    template <typename T> void println(T p) { softwareSerial->println(p); }
    void print(std::string str);
    void println(std::string str);
    void overwriteMode() { this->write('\x1B'); this->write('\x11');};
    void virticalScrollMode() { this->write('\x1B'); this->write('\x12');};
    void horizontalScrollMode() { this->write('\x1B'); this->write('\x13');};
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
