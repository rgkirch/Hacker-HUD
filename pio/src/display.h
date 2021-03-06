#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include "mySerial.hpp"

using std::string;

class CharacterDisplay {
public:
    CharacterDisplay(int width, int height) : width(width), height(height) {};
    virtual void setUpperLine(string str) =0;
    virtual void setLowerLine(string str) =0;
    void setUpperLine(string left, string right) {
        this->setUpperLine(blockFormat(left, right));
    };
    void setLowerLine(string left, string right) {
        this->setLowerLine(blockFormat(left, right));
    };
    const int getWidth() const { return width; }
    const int getHeight() const { return height; }
private:
    const int width;
    const int height;
    string blockFormat(string left, string right) {
        const int leftLength = static_cast<int>(left.length());
        const int rightLength = static_cast<int>(right.length());
        const int padding = width - left.length() - right.length();
        if (padding > 0) {
            left.resize(static_cast<unsigned int>(leftLength + padding), ' ');
        } else if (rightLength + padding < 0) {
            left.resize(static_cast<unsigned int>(leftLength + rightLength + padding));
            right.clear();
        } else {
            right.resize(static_cast<unsigned int>(rightLength + padding));
        }
        return left + right;
    }
};

class HardwareDisplay : public CharacterDisplay {
public:
    HardwareDisplay(int width, int height, MyPrint *s) : CharacterDisplay(width, height), serial(s) {};
    using CharacterDisplay::setUpperLine;
    using CharacterDisplay::setLowerLine;
protected:
    MyPrint *serial;
};

class VFD : public HardwareDisplay
{
public:
    VFD(int width, int height, MyPrint *serial) : HardwareDisplay(width, height, serial) {
        serial->print("\x1B\x40\x0C"); // initialize ( bytes 0,1) and clear (byte 2) display
    };
    size_t print(const char c) { return serial->print(c); };
    size_t print(const char* cs) { return serial->print(cs); };
//    void println(string str) { this->print(str); this->write("\x1B\x6C\x01\x02"); };
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
    void setUpperLine(string str) {
        serial->print("\x1B\x51\x41");
        str.resize(getWidth(), ' ');
        serial->print(str.c_str());
        serial->print("\x0D");
    };
    void setLowerLine(string str) {
        serial->print("\x1B\x51\x42");
        str.resize(getWidth(), ' ');
        serial->print(str.c_str());
        serial->print("\x0D");
    };
    using CharacterDisplay::setUpperLine;
    using CharacterDisplay::setLowerLine;
//    void printJustified(string str) {};
};

#endif //DISPLAY_H
