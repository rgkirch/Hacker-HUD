#include "vfd.hpp"

VFD::VFD(int displayWidth, int displayHeight, MySoftwareSerial *serial) : HardwareCharacterDisplay(displayWidth, displayHeight, serial)
{
    this->print("\x1B\x40\x0C"); // initialize and clear display
    this->clear();
    this->home();
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
    MySoftwareSerial *serial = new MySoftwareSerial(rx, tx);
    return new VFD(displayWidth, displayHeight, serial);
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
VFD::Builder& VFD::Builder::setBaudRate(int baudRate) {
    this->baudRate = baudRate;
    return *this;
}
