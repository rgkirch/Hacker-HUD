#include <stdio.h>
#include <time.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

//constants -----
const char* ssid     = "HellSpot Slow"; //HellSpot Slow
const char* password = "ILikeWiFi"; //ILikeWiFi
// const char* host = "utcnist2.colorado.edu";
const char* host = "128.138.141.172";

SoftwareSerial mySerial(D5,D6);//rx,tx
VFD myVFD(mySerial);
time_t rawtime;
struct tm * timeinfo;
//Setup -----
void setup() {
  Serial.begin(115200);
  delay(10); //max delay for wifi to work!
  Wifi.begin();

  myVFD.brightness(1);

}

int ln = 0;
String TimeDate = "";

String getTime();

void loop() {
  // time ( &rawtime );
  // timeinfo = localtime ( &rawtime );
  // printf("Current local time and date: %s", asctime (timeinfo) );
  // myVFD.print(asctime(timeinfo));
  myVFD.print(getTime());
  delay(3000);
}

String getTime()
{
  WiFiClient client;
  const int httpPort = 13;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return "";
  }
  // This will send the request to the server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");
  delay(100);
  // Read all the lines of the reply from server and print them to Serial
  // expected line is like : Date: Thu, 01 Jan 2015 22:00:14 GMT
  char buffer[12];
  String dateTime = "";
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    if (line.indexOf("Date") != -1)
    {
      Serial.print("=====>");
    } else {
      Serial.print(line);
    //   TimeDate = line.substring(7);
    //   Serial.println(TimeDate);
    //   // time starts at pos 14
    //   TimeDate = line.substring(7, 15);
    //   TimeDate.toCharArray(buffer, 10);
    //   sendStrXY("UTC Date/Time:", 4, 0);
    //   sendStrXY(buffer, 5, 0);
    //   TimeDate = line.substring(16, 24);
    //   TimeDate.toCharArray(buffer, 10);
    //   sendStrXY(buffer, 6, 0);
    }
  }
}
