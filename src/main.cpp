#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <NtpClientLib.h>
#include "../lib/vfd/vfd.hpp"
#include "../lib/vfd/vfdBuilder.h"

typedef unsigned char uint8_t;

VFD* myVFD;

void setup() {
    vfdBuilder myVfdBuilder;
    myVfdBuilder.setRx(D5);
    myVfdBuilder.setTx(D6);
    myVfdBuilder.setDisplayWidth(20);
    myVfdBuilder.setDisplayHeight(2);
    myVFD = myVfdBuilder.buildVFD();
//    myVFD = new VFD(14, 12);
    Serial.begin(115200);
    yield();
}

void loop() {
    unsigned char hello[] = "hellt";
    myVFD->print(hello, 5);
    Serial.write("hello");
}
