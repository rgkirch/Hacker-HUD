#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>

#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <string>

#define DEFAULT_TIMESERVER "0.pool.ntp.org"//"time.nist.gov"
#define MINIMUM_INTERVAL 60

// global variables first

//NTP BS -------------------------------------
const std::string timeServer("time.nist.gov"); // time.nist.gov NTP server
unsigned int localPort = 123;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
byte sendBuffer[] = {
  0b11100011,          // LI, Version, Mode.
  0x0,                 // Stratum unspecified.
  0x6,                 // Polling interval
  0xEC,                // Clock precision.
  0x0, 0x0, 0x0, 0x0}; // Reference ...


// WiFi settings -----------------------------
const std::string ssid("HellSpot Slow");
const std::string password("ILikeWiFi");

// API server
const char host[] = "api.coindesk.com";

SoftwareSerial mySerial(D0,D1); //rx,tx

// function declarations second
void connectToWifi(const std::string& ssid, const std::string& password);
void sendNTPpacket(WiFiUDP& u);
time_t getNtpTime();

void setup()
{
  // Serial
  Serial.begin(115200);
  mySerial.begin(19200); // for VFD
  delay(100);

  mySerial.write('\x0E');
  mySerial.write('\x0C');

  mySerial.print("VFD Display code V1");

  mySerial.write('\x0E');
  mySerial.write('\x0C');

  delay(2000);

  // Initialize display

  // We start by connecting to a WiFi network
  connectToWifi(ssid, password);
}

void loop() {

  // Connect to API
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/v1/bpi/currentprice.json";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");

  // Process answer
  Serial.println();
  Serial.println("Answer: ");
  Serial.println(answer);

  // Convert to JSON
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(jsonAnswer);
  jsonAnswer.trim();

  // Get rate as float
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  float price = priceString.toFloat();

  // Print price
  Serial.println();
  Serial.println("Bitcoin price: ");
  Serial.println(price);

  //Display Price
  mySerial.write('\x0E');
  mySerial.write('\x0C');
  mySerial.print("Bitcoin $: ");
  mySerial.print(price);
  mySerial.print("    ");
  float penis = getNtpTime();

  mySerial.print(penis);

  // Wait 5 seconds
  delay(8000);
}

//NTP functions ------------------------------
void sendNTPpacket(WiFiUDP& u)
{
  // Zeroise the buffer.
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  memcpy(packetBuffer, sendBuffer, 16);


  if (u.beginPacket(timeServer.c_str(), 123)) {
    u.write(packetBuffer, NTP_PACKET_SIZE);
    u.endPacket();
  }
}


time_t getNtpTime()
{
  WiFiUDP udp;
  udp.begin(localPort);
  while (udp.parsePacket() > 0); // discard any previously received packets
  for (int i = 0 ; i < 5 ; i++) // 5 retries.
  {
    sendNTPpacket(udp);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
      if (udp.parsePacket()) {
         udp.read(packetBuffer, NTP_PACKET_SIZE);
         // Extract seconds portion.
         unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
         unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
         unsigned long secSince1900 = highWord << 16 | lowWord;
         udp.flush();
         Serial.print(secSince1900);
         return secSince1900 - 2208988800UL + 1 * 60;
      }
      delay(10);
    }
  }
  return 0; // return 0 if unable to get the time
}

void connectToWifi(const std::string& ssid, const std::string& password)
{
    int delayTime = 200;
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
        delay(delayTime);
        Serial.print(".");
        if(delayTime < 5000)
        {
            delayTime *= 2;
        }
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
