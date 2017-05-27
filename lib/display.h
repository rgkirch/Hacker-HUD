#ifndef GTESTPROJ_VFD_H
#define GTESTPROJ_VFD_H

#include <string>

class Print {
public:
//    virtual ~Print() =0; // google test says that it must have a virtual destructor
    Print(int port) : port(port) {};
    virtual size_t print(std::string) =0;
    virtual int getPort() {
        return port;
    };
protected:
    int port;
};
class Serial : public Print {
public:
    Serial(int p) : Print(p) {};
    size_t print(std::string str) {
        // write data
        return str.size();
    };
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
protected:
    const int width;
    const int height;
private:
    std::string blockFormat(std::string left, std::string right);
};

class HardwareDisplay : public CharacterDisplay {
public:
    HardwareDisplay(int width, int height, Print* s) : CharacterDisplay(width, height), serial(s) {};
    void setUpperLine(std::string str);
    void setLowerLine(std::string str);
    using CharacterDisplay::setUpperLine; // couldn't find setUpperLine(std::string, std::string) without this...
    using CharacterDisplay::setLowerLine;

private:
    Print *serial;
};

#endif //GTESTPROJ_VFD_H
