#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <NtpClientLib.h>
#include "../lib/vfd/vfd.hpp"
#include "../lib/vfd/vfdBuilder.h"
#include "../lib/wifi/wifi.hpp"
#include <NtpClientLib.h>

typedef unsigned char uint8_t;

VFD* myVFD;
int vfdPrint(const char *c, int n)
{
    if(myVFD == nullptr)
    {
        return 0;
    } else {
        return myVFD->print(c, n);
    }
}

void setup() {
    vfdBuilder myVfdBuilder;
    myVfdBuilder.setRx(D5);
    myVfdBuilder.setTx(D6);
    myVfdBuilder.setDisplayWidth(20);
    myVfdBuilder.setDisplayHeight(2);
    myVFD = myVfdBuilder.buildVFD();
    Serial.begin(115200);
    yield();
    InitializeWiFi();
}

void loop() {
    const char* host = "www.example.com";
    WiFiClient client;

    Serial.printf("\n[Connecting to %s ... ", host);
    if (client.connect(host, 80))
    {
        Serial.println("connected]");

        Serial.println("[Sending a request]");
        client.print(String("GET /") + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n" +
                     "\r\n"
        );

        Serial.println("[Response:]");
        while (client.connected())
        {
            if (client.available())
            {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }
        client.stop();
        Serial.println("\n[Disconnected]");
    }
    else
    {
        Serial.println("connection failed!]");
        client.stop();
    }
    delay(5000);
    yield();
}
