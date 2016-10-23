/*
General Display Code template

Coyt Barringer

10/22/16
*/

//included libraries -----
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include "VFD.h"

//constants -----
const char WIFI_SSID[] = "AvalonHeights";
const char WIFI_PSK[] = "";

const char http_site[] = "www.example.com";
const int http_port = 80;

//Initialize Objects -----
//SoftwareSerial mySerial(D5,D6);//rx,tx
SoftwareSerial mySerial(D5,D6);//rx,tx
VFD myVFD(mySerial); //D5,D6

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); //for MCP9808


//Initialize Temp Sensor Fnc. -----
void InitializeTemp(){
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
}

//Read Temp Fnc. -----
float readTemp() {
  tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp

  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t");
  Serial.print(f); Serial.println("*F");
  delay(250);

  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
  return f;
}

//Setup -----
void setup() {

  delay(4000);
  Serial.begin(115200);
  //mySerial.begin(9600);

  pinMode(D4,OUTPUT);

  //connectWiFi();

  //delay(1000);

  //together these Initialize the display
  //mySerial.write('\x1B');
  //mySerial.write('\x40');

  delay(1000);

//  mySerial.write('\x0C'); //clear display
  //mySerial.print("");
  myVFD.print("VFD Display code V1");


  //commands to change brightness based on CD5220 Command Set
  //mySerial.write('\x1B');
  //mySerial.write('\x2A');
  //mySerial.write('\x02'); //brightness select 1 <= n <= 4

  myVFD.brightness(2);

  delay(1000);

  InitializeTemp(); //Initialize temp sensor

  //mySerial.write('\x0E');
  //mySerial.write('\x0C');

}



//Loop -----
void loop() {

  delay(3000);

  //mySerial.write('\x0C'); //clear display
  //mySerial.write('\x0B'); //move cursor to home position
  myVFD.clearHome();
  myVFD.print("Hacker HUD V1.0");
  //mySerial.write('\x0A');
  myVFD.print("test code by Coyt");

  delay(3000);

  //mySerial.write('\x0C');
  //mySerial.write('\x0B');
  myVFD.clearHome();
  myVFD.print("HACK THE PLANET YAY");

  delay(3000);
  //mySerial.write('\x0C');
  //mySerial.write('\x0B');
  myVFD.clearHome();
  myVFD.print("Current Temp: "); //print current temp reading to VFD
  myVFD.print(readTemp());

}

//Initialize Display Fnc. -----
