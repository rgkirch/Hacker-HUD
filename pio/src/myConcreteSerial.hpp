#ifndef GTESTPROJ_SERIAL_H
#define GTESTPROJ_SERIAL_H

#include <SoftwareSerial.h>
#include "myAbstractSerial.hpp"

class MyConcretePrint : public MyPrint {
public:
    MyConcretePrint(const uint8_t rx, const uint8_t tx) : serial(SoftwareSerial(rx, tx)) {};
//    ~MyConcretePrint() { delete serial; }
    size_t print(const char c) override { return serial.print(c); };
    size_t print(const char* cs) override { return serial.print(cs); };
protected:
    SoftwareSerial serial;
};

#endif //GTESTPROJ_SERIAL_H
