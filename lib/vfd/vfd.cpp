#include "vfd.hpp"

VFD::VFD(const unsigned char receivePin, const unsigned char transmitPin, int displayWidth, int displayHeight) : width(displayWidth), height(displayHeight)
{
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
}

VFD::~VFD() {
    delete softwareSerial;
}

int VFD::print(const unsigned char *c, int n) {
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
