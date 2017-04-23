#ifndef VFD_HPP
#define VFD_HPP

#include <SoftwareSerial.h>
//#include <EEPROM.h>
//#include <WString.h>
#include <string>
//#include <memory>

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
    ~VFD() { delete softwareSerial; }
    template <typename T> VFD& operator<<(T t) { this->print(t); return *this; }
    template <typename T> void write(T p)   { softwareSerial->write(p); }
    template <typename T> void print(T p)   { softwareSerial->print(p); }
    template <typename T> void println(T p) { softwareSerial->println(p); }
    void print(std::string str)   { for (char c:str) this->write(c); };
    void println(std::string str) { this->print(str); this->write("\x1B\x6C\x01\x02"); };
//    void printn(std::string str, int n) { for (int i=0; i < n; i++) { this->print(str[i]); } };
    void overwriteMode()          { this->print("\x1B\x11"); };
    void virticalScrollMode()     { this->print("\x1B\x12"); };
    void horizontalScrollMode()   { this->print("\x1B\x13"); };
    void clear()                  { this->print("\x0C"); };
    void cancel()                 { this->print("\x18"); };
    void cursorLeft()             { this->print("\x1B\x5B\x44"); };
    void backspace()              { this->print("\x08"); };
    void cursorRight()            { this->print("\x1B\x5B\x43"); };
    void tab()                    { this->print("\x09"); };
    void cursorUp()               { this->print("\x1B\x5B\x41"); };
    void cursorDown()             { this->print("\x1B\x5B\x42"); };
    void lineFeed()               { this->print("\x0A"); };
    void carriageReturn()         { this->print("\x0D"); };
    void home()                   { this->print("\x0B"); };
    void setUpperLine(std::string str) {
        this->print("\x1B\x51\x41");
        str.resize(20, ' ');
        this->print(str);
        this->print("\x0D");
    };
    void setLowerLine(std::string str) {
        this->print("\x1B\x51\x42");
        str.resize(20, ' ');
        this->print(str);
        this->print("\x0D");
    };
    void setLowerLine(std::string left, std::string right) {
        this->print("\x1B\x51\x42");
        int padding = width - left.length() - right.length();
        if (padding > 0) {
            left.resize(padding, ' ');
        } else {
            right.resize(right.length() + padding);
        }
        this->print(left + right);
        this->print("\x0D");
    };
//    void printJustified(std::string str) {};
};

class VFD::Builder
{
private:
    int rx = -1;
    int tx = -1;
    int displayWidth = 20;
    int displayHeight = 2;
public:
    Builder() = default; // todo - give it default values?
    VFD *build();
    VFD::Builder& setRx(int rx);
    VFD::Builder& setTx(int tx);
    VFD::Builder& setDisplayWidth(int displayWidth);
    VFD::Builder& setDisplayHeight(int displayHeight);
};

#endif
