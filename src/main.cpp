#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <NtpClientLib.h>
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
    Serial.write("hello");
}
