//#include <Arduino>
#include "../lib/Arduino/variants/nodemcu/pins_arduino.h"
#include "../lib/vfd/vfd.hpp"

typedef unsigned char uint8_t;

VFD* myVFD;

void setup() {
    myVFD = new VFD(D5, D6);
//    myVFD = new VFD(14, 12);
    Serial.begin(115200);
    yield();
}

void loop() {
    unsigned char hello[] = "hello";
    myVFD->print(hello, 5);
}
