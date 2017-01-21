#include "vfd.hpp"

VFD::VFD(const unsigned char receivePin, const unsigned char transmitPin)
{
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
}

VFD::~VFD() {
    delete softwareSerial;
}

int VFD::print(unsigned char *c, int n) {
    int counter = 0;
    for (int i = 0; i < n; ++i) {
        softwareSerial->write(c[i]);
        counter++;
    }
    return counter;
}
