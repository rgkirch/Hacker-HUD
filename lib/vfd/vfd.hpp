#ifndef VFD_HPP
#define VFD_HPP

#include <SoftwareSerial.h>
//#include <HardwareSerial.h>
//#include <EEPROM.h>
//#include <WString.h>
#include <string>
//#include <memory>

//extern HardwareSerial Serial;

class MySoftwareSerial : public SoftwareSerial {
public:
    MySoftwareSerial(int receivePin, int transmitPin) : SoftwareSerial(receivePin, transmitPin) {};
    size_t print(std::string str)     { return write(str.c_str()); };
    size_t println(std::string str)   { size_t n = write(str.c_str()); n += write("\r\n"); return n; };
    template <typename T> MySoftwareSerial& operator<<(T t) { this->print(t); return *this; }
};

class CharacterDisplay {
public:
//    virtual void clear() { this->home(); for (int i = 0; i < width * height; ++i) this->softwareSerial->print(" "); } // please override this
    virtual void clear() =0;
    virtual void home() =0;
    virtual void setLowerLine(std::string str) =0;
    virtual void setUpperLine(std::string str) =0;
    void setUpperLine(std::string left, std::string right) {
        this->setUpperLine(blockFormat(left, right));
    };
    void setLowerLine(std::string left, std::string right) {
        this->setLowerLine(blockFormat(left, right));
    };
protected:
    const int width;
    const int height;
private:
    std::string blockFormat(std::string left, std::string right) {
        int padding = width - left.length() - right.length();
        if (padding > 0) {
            left.resize(left.length() + padding, ' ');
        } else {
            right.resize(right.length() + padding);
        }
        return left + right;
    }
};

class HardwareCharacterDisplay : public CharacterDisplay
{
public:
    HardwareCharacterDisplay(int receivePin, int transmitPin, int displayWidth, int displayHeight);
    template <typename T> size_t print(T t) { return softwareSerial->print(t); }
    template <typename T> size_t println(T t) { return softwareSerial->println(t); }
protected:
    MySoftwareSerial *softwareSerial;
};

class VFD : public HardwareCharacterDisplay
{
public:
    class Builder;
    VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight);
//    void println(std::string str) { this->print(str); this->write("\x1B\x6C\x01\x02"); };
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
