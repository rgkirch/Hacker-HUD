#include "vfd.hpp"

HardwareCharacterDisplay::HardwareCharacterDisplay(int receivePin, int transmitPin, int displayWidth, int displayHeight) : width(displayWidth), height(displayHeight) {
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
}

VFD::VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight) : HardwareCharacterDisplay(receivePin, transmitPin, displayWidth, displayHeight)
{
    this->print("\x1B\x40\x0C"); // initialize and clear display
    //together these Initialize the display
//    softwareSerial->write('\x1B');
//    softwareSerial->write('\x40');

//    softwareSerial->write('\x0C'); //clear display
}

VFD *VFD::Builder::build(){
    if(rx < 0) return nullptr;
    if(tx < 0) return nullptr;
    if(displayWidth < 0) return nullptr;
    if(displayHeight < 0) return nullptr;
    if (rx == tx) return nullptr;
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
