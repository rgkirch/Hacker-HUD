#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/*
//NTP BS ---------------------------------------
unsigned int localPort = 4097;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
byte sendBuffer[] = {
  0b11100011,          // LI, Version, Mode.
  0x0,                 // Stratum unspecified.
  0x6,                 // Polling interval
  0xEC,                // Clock precision.
  0x0, 0x0, 0x0, 0x0}; // Reference ...
  */

// WiFi information ---------------------------
const char WIFI_SSID[] = "AvalonHeights";
const char WIFI_PSK[] = "";

// Remote site information
const char http_site[] = "www.example.com";
const int http_port = 80;

SoftwareSerial mySerial(D5,D6);//rx,tx

//WiFiClient client;


//time_t newTime = getNtpTime();

/*

void float getBitcoinPrice(){
  https://www.bitstamp.net/api/ticker_hour/



}
*/

/*

// Attempt to connect to WiFi
void connectWiFi() {

  byte led_status = 0;

  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);

  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);

  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(D4, led_status);
    led_status ^= 0x01;
    delay(100);
  }

  // Turn LED on when we are connected
  digitalWrite(D4, HIGH);
}
*/

/*

// Perform an HTTP GET request to a remote page
bool getPage() {

  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }

  // Make an HTTP GET request
  client.println("GET /index.html HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();

  return true;
}
*/

void setup() {

  delay(4000);
  Serial.begin(115200);
  mySerial.begin(9600);

  pinMode(D4,OUTPUT);

  //connectWiFi();

  delay(1000);

  mySerial.write('\x1B');
  mySerial.write('\x40');

  delay(1000);

  mySerial.write('\x0C');
  mySerial.print("VFD Display code V1");


  //commands to change brightness based on CD5220 Command Set
  mySerial.write('\x1B');
  mySerial.write('\x2A');
  mySerial.write('\x02'); //brightness select 1 <= n <= 4



  delay(1000);

  //mySerial.write('\x0E');
  //mySerial.write('\x0C');

}

void loop() {
  delay(3000);
  //mySerial.write('\x0E');
  mySerial.write('\x0C');
  mySerial.write('\x0B');
  mySerial.println("Hacker HUD V1.0");
  //mySerial.write('\x0A');
  mySerial.print("test code by Coyt");
  delay(3000);
  mySerial.write('\x0C');
  mySerial.write('\x0B');
  mySerial.print("HACK THE PLANET YAY");

  //Serial.write("penis");


/*  // If there are incoming bytes, print them
  if ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
  */
}