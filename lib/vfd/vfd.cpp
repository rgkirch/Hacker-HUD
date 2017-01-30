#include "vfd.hpp"

VFD::VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight) : width(displayWidth), height(displayHeight)
{
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
}

VFD::~VFD() {
    delete softwareSerial;
}

int VFD::print(const char *c, int n) {
    if(c == NULL) return 0;
    if(n < 0) n = 0;
    if(n > strlen((const char *) c)) n = strlen((const char *) c);
    int counter = 0;
    for (int i = 0; i < n; ++i) {
        softwareSerial->write(c[i]);
        counter++;
    }
    return counter;
}

void VFD::print(std::string str)
{
    for (char c:str)
    {
        softwareSerial->write(c);
    }
}

void VFD::println(std::string str)
{
    for (char c:str)
    {
        softwareSerial->write(c);
    }
    softwareSerial->write('\n');
}

std::unique_ptr<VFD> VFD::Builder::build(){
    if(rx < 0) return nullptr;
    if(tx < 0) return nullptr;
    if(displayWidth < 0) return nullptr;
    if(displayHeight < 0) return nullptr;
    return std::unique_ptr<VFD>(new VFD(rx, tx, displayWidth, displayHeight));
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
