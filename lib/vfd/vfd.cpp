#include "vfd.hpp"

VFD::VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight) : width(displayWidth), height(displayHeight), cursorIndex(0)
{
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
    this->print("\x1B\x40\x0C"); // initialize and clear display
    //together these Initialize the display
//    softwareSerial->write('\x1B');
//    softwareSerial->write('\x40');

//    softwareSerial->write('\x0C'); //clear display
}

VFD::~VFD() {
    delete softwareSerial;
}

void VFD::print(std::string str)
{
    for (char c:str)
    {
        this->write(c);
    }
}

void VFD::println(std::string str)
{
    this->print(str);
    //fnc for moving to next line
    this->write('\x1B');
    this->write('\x6C');
    this->write('\x01');
    this->write('\x02');

//fnc for just clearing the display
}

VFD *VFD::Builder::build(){
    if(rx < 0) return nullptr;
    if(tx < 0) return nullptr;
    if(displayWidth < 0) return nullptr;
    if(displayHeight < 0) return nullptr;
    return new VFD(rx, tx, displayWidth, displayHeight);
}

VFD::Builder& VFD::Builder::setRx(int rx) {
    this->rx = rx;
    return *this;
}

VFD::Builder& VFD::Builder::setTx(int tx) {
    this->tx = tx;
    return *this;
}

VFD::Builder& VFD::Builder::setDisplayWidth(int displayWidth) {
    this->displayWidth = displayWidth;
    return *this;
}

VFD::Builder& VFD::Builder::setDisplayHeight(int displayHeight) {
    this->displayHeight = displayHeight;
    return *this;
}
