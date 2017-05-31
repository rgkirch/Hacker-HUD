#ifndef GTESTPROJ_SERIAL_H
#define GTESTPROJ_SERIAL_H

#include <SoftwareSerial.h>
#include "abstractSerial.hpp"

class MySerial : public AbstractSerial {
public:
    MySerial(const uint8_t rx, const uint8_t tx) { serial = new SoftwareSerial(rx, tx); }
    ~MySerial() { delete serial; }
    size_t print(const char c) override { serial->print(c); };
    size_t print(const char* cs) override { serial->print(cs); };
protected:
    SoftwareSerial *serial;
};

#endif //GTESTPROJ_SERIAL_H
