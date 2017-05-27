#include <Arduino.h>
#include <display.h>
//#include <SoftwareSerial.h>

VFD *vfd;
void setup() {
    vfd = new VFD(20, 2, new MySerial(5, 6));
}

void loop() {
    vfd->home();
    vfd->setUpperLine("hello");
}
