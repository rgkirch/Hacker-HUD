#ifndef GTESTPROJ_VFD_H
#define GTESTPROJ_VFD_H

#include <string>

class AbstractSerial {
public:
    virtual size_t print(const char) =0;
    virtual size_t print(const char*) =0;
};

class CharacterDisplay {
public:
    CharacterDisplay(int width, int height) : width(width), height(height) {};
    virtual void setUpperLine(std::string str) =0;
    virtual void setLowerLine(std::string str) =0;
    void setUpperLine(std::string left, std::string right) {
        this->setUpperLine(blockFormat(left, right));
    };
    void setLowerLine(std::string left, std::string right) {
        this->setLowerLine(blockFormat(left, right));
    };
    const int getWidth() const { return width; }
    const int getHeight() const { return height; }
private:
    const int width;
    const int height;
    std::string blockFormat(std::string left, std::string right) {
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
    HardwareDisplay(int width, int height, AbstractSerial *s) : CharacterDisplay(width, height), serial(s) {};
    using CharacterDisplay::setUpperLine;
    using CharacterDisplay::setLowerLine;
protected:
    AbstractSerial *serial;
};

class VFD : public HardwareDisplay
{
public:
    VFD(int width, int height, AbstractSerial *serial) : HardwareDisplay(width, height, serial) {};
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
        str.resize(getWidth(), ' ');
        serial->print(str.c_str());
        serial->print("\x0D");
    };
    void setLowerLine(std::string str) {
        serial->print("\x1B\x51\x42");
        str.resize(getWidth(), ' ');
        serial->print(str.c_str());
        serial->print("\x0D");
    };
    using CharacterDisplay::setUpperLine;
    using CharacterDisplay::setLowerLine;
//    void printJustified(std::string str) {};
};

#endif //GTESTPROJ_VFD_H
