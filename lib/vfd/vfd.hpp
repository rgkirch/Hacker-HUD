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
    template <typename T> void write(T p) { softwareSerial->write(p); }
    template <typename T> void print(T p) { softwareSerial->print(p); }
    template <typename T> void println(T p) { softwareSerial->println(p); }
    void print(std::string str);
    void println(std::string str);
    void overwriteMode()        { this->print("\x1B\x11");};
    void virticalScrollMode()   { this->print("\x1B\x12");};
    void horizontalScrollMode() { this->print("\x1B\x13");};
    void clear()                { this->print("\x0C");};
    void cancel()               { this->print("\x18");};
    void cursorLeft()           { this->print("\x1B\x5B\x44");};
    void backspace()            { this->print("\x08");};
    void cursorRight()          { this->print("\x1B\x5B\x43");};
    void tab()                  { this->print("\x09");};
    void cursorUp()             { this->print("\x1B\x5B\x41");};
    void newLine()              { this->print("\x0A");};
    void home()                 { this->print("\x0B");};
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
