#ifndef GTESTPROJ_VFD_H
#define GTESTPROJ_VFD_H

#include <string>
#include <SoftwareSerial.h>

class MySerial : public SoftwareSerial {
public:
    MySerial(int rx, int tx) : SoftwareSerial(rx, tx) {};
    void print(std::string str) {
        print(str.c_str());
    };
    using SoftwareSerial::print;
};

class CharacterDisplay {
public:
    CharacterDisplay(int displayWidth, int displayHeight) : width(displayHeight), height(displayHeight) {};
    virtual void setUpperLine(std::string str) =0;
    virtual void setLowerLine(std::string str) =0;
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
    std::string blockFormat(std::string left, std::string right);
};

class HardwareDisplay : public CharacterDisplay {
public:
    HardwareDisplay(int width, int height, MySerial *s) : CharacterDisplay(width, height), serial(s) {};

    void setUpperLine(std::string str);

    void setLowerLine(std::string str);

    using CharacterDisplay::setUpperLine; // couldn't find setUpperLine(std::string, std::string) without this...
    using CharacterDisplay::setLowerLine;
protected:
    MySerial *serial;
};

class VFD : public HardwareDisplay
{
public:
    VFD(int Width, int Height, MySerial *serial) : HardwareDisplay(width, height, serial) {};
//    void println(std::string str) { this->print(str); this->write("\x1B\x6C\x01\x02"); };
    void overwriteMode()          { serial->print("\x1B\x11"); };
    void virticalScrollMode()     { serial->print("\x1B\x12"); };
    void horizontalScrollMode()   { serial->print("\x1B\x13"); };
    void clear()                  { serial->print("\x0C"); };
    void cancel()                 { serial->print("\x18"); };
    void cursorLeft()             { serial->print("\x1B\x5B\x44"); };
    void backspace()              { serial->print("\x08"); };
    void cursorRight()            { serial->print("\x1B\x5B\x43"); };
    void tab()                    { serial->print("\x09"); };
    void cursorUp()               { serial->print("\x1B\x5B\x41"); };
    void cursorDown()             { serial->print("\x1B\x5B\x42"); };
    void lineFeed()               { serial->print("\x0A"); };
    void carriageReturn()         { serial->print("\x0D"); };
    void home()                   { serial->print("\x0B"); };
    void setUpperLine(std::string str) {
        serial->print("\x1B\x51\x41");
        str.resize(20, ' ');
        serial->print(str);
        serial->print("\x0D");
    };
    void setLowerLine(std::string str) {
        serial->print("\x1B\x51\x42");
        str.resize(20, ' ');
        serial->print(str);
        serial->print("\x0D");
    };
//    void printJustified(std::string str) {};
};

#endif //GTESTPROJ_VFD_H