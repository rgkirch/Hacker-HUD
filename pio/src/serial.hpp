#ifndef GTESTPROJ_SERIAL_H
#define GTESTPROJ_SERIAL_H

#include <SoftwareSerial.h>
#include "display.h"

class Serial : public AbstractSerial {
public:
    Serial(int rx, int tx) { serial = new SoftwareSerial(rx, tx); }
    ~Serial() { delete serial; }
    size_t print(const char c) override { serial->print(c); };
    size_t print(const char* cs) override { serial->print(cs); };
protected:
    SoftwareSerial *serial;
};

#endif //GTESTPROJ_SERIAL_H
