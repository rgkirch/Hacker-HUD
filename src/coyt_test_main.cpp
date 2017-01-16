/*
General Display Code template

Coyt Barringer

10/22/16
*/

//included libraries -----
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <Adafruit_MCP9808.h>
#include <Wire.h>

#include <VFD.h>

#include <NtpClientLib.h> //dependent on Time library

//constants -----
const char* ssid     = "HellSpot Slow"; //HellSpot Slow
const char* password = "ILikeWiFi"; //ILikeWiFi

const char* host = "api.coindesk.com";
const char* hostEth = "api.nanopool.org";
const char* hostTime = "script.google.com";

//structs -----
struct jsonThing {
  const String keyword;
  int begin;
  int end;
};

struct Site {
  const char* url;
  const char* host;
  bool secure;
};

//websites and data to parse -----
Site coinDesk = (Site) {.url = "/v1/bpi/currentprice.json", .host = "api.coindesk.com", .secure = false};
jsonThing coinDeskJson {"rate_float", 12, 18};

Site etheriumHashes = (Site) {.url = "/v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1", .host = "api.nanopool.org", .secure = true};
jsonThing ethereumHashesJson = {"data",6,11};

Site etheriumPrice = (Site) {.url = "/v1/eth/prices", .host = "api.nanopool.org", .secure = true};
jsonThing etheriumJson {"price_usd", 11, 16};

//Initialize Objects -----
SoftwareSerial mySerial(D5,D6);//rx,tx
VFD myVFD(mySerial);

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); //for MCP9808












//Function to connect and retrieve web data -----
String scrapeWeb(Site site) {

  String priceString;
  // Connect to API
  Serial.print("connecting to ");
  Serial.println(site.host);

  // Use WiFiClient class to create TCP connections
  WiFiClient* client;
  int httpPort;
  if(site.secure)
  {
    client = new WiFiClientSecure;
    httpPort = 443;
  } else {
    client = new WiFiClient;
    httpPort = 80;
  }

  if(!client->connect(site.host, httpPort)) {
    Serial.println("connection failed");
  }

  // We now create a URI for the request "/v1/bpi/currentprice.json"
  Serial.print("Requesting URL: ");
  Serial.println(site.url);
  Serial.print("host: ");
  Serial.println(site.host);

  // This will send the request to the server
  client->print(String("GET ") + String(site.url) + " HTTP/1.1\r\n" +
               "Host: " + String(site.host) + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (!client->available()) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client->stop();
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String answer;
  while(client->available())
  {
    String line = client->readStringUntil('\r');
    answer += line;
  }

  client->stop();
  Serial.println();
  Serial.println("closing connection");

  // Process answer
  Serial.println();
  Serial.println("Answer: ");
  Serial.println(answer);

  delete client;
  return answer;
}

//fnc to parse data from web page (json) -----
String parseData (String answer, jsonThing whatToParse){

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
  String priceString = jsonAnswer.substring(jsonAnswer.indexOf(whatToParse.keyword) + whatToParse.begin);
  int indexOutput = 0;

  if (priceString.indexOf(",") == -1){
    indexOutput = priceString.indexOf("}");
  } else if (priceString.indexOf("}") == -1){
    indexOutput = priceString.indexOf(",");
  } else {
    int less;
    if (priceString.indexOf("}") < priceString.indexOf(",")) {
      less = priceString.indexOf("}");
    } else {
      less = priceString.indexOf(",");
    }
    indexOutput = less;
  }
  priceString = priceString.substring(0,indexOutput);
  int decimal = priceString.indexOf(".");
  if (decimal > -1) priceString = priceString.substring(0,decimal + 3);
  priceString.trim();
  float price = priceString.toFloat();

  Serial.println("end function");
  return priceString;
}

//Initialize WiFi Connection -----
void InitializeWiFi(){

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    myVFD.print("WiFi Connected: " + (String)ssid); //display on VFD
    delay(4000);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //setup NTP time stuff here
    NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
        if (error) {
            Serial.print("Time Sync error: ");
            if (error == noResponse)
                Serial.println("NTP server not reachable");
            else if (error == invalidAddress)
                Serial.println("Invalid NTP server address");
        }
        else {
            Serial.print("Got NTP time: ");
            Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
        }
    });

    NTP.begin("pool.ntp.org", -5, true);
    NTP.setInterval(1800);
}

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

//fnc for reading time from json response -----
String readTime(String realTime){

  // Convert to JSON
  String timeEst;
  int index;

  for (int i = 0; i < realTime.length(); i++) {
    if (realTime[i] == '{') {
      index = i;
      break;
    }
  }

  /*
  // Get JSON data
  timeEst = realTime.substring(index);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(timeEst);
  jsonAnswer.trim();
  */

  // Get rate as float
  String year = realTime.substring(index+18,index+22);
  String month = realTime.substring(index+10,index+13);
  String day = realTime.substring(index+14,index+16);
  String hour = realTime.substring(index+24,index+25);
  String minute = realTime.substring(index+27,index+28);

  //priceString.trim();
  String jsonTime = (month + " " + day + ", " + year + " " + hour + ":" + minute);

  return jsonTime;
}

//Setup -----
void setup() {
  Serial.begin(115200);
  delay(10); //max delay for wifi to work!

  InitializeWiFi(); //call to initialize wifi fnc. - DO NOT PUT A LARGE DELAY BEFORE THIS!!!!!

  myVFD.print("VFD Display code V1");
  myVFD.brightness(1);

  InitializeTemp(); //Initialize temp sensor
}

//Re-format getTimeDateString fnc for standard display -----
String formatTime(String old){
  mySerial.println(old);
  int fullHour = old.substring(0,2).toInt();
  fullHour = fullHour > 12 ? fullHour - 12 : fullHour;
  String fh = fullHour < 10 ? "0" + String(fullHour) : String(fullHour);
  String fixedTime =  fh + ":" + old.substring(3,5) + ":" + old.substring(6,8) + "  " + old.substring(12,14) + "/" + old.substring(9,11) + "/" + old.substring(15,19);
  return fixedTime;
}

//Loop -----
void loop() {

  String coinDeskWebData = scrapeWeb(coinDesk);
  String btcPrice = parseData(coinDeskWebData, coinDeskJson);

  myVFD.print("  Hacker HUD V1.0  ");
  myVFD.nextLine();
  myVFD.simplePrint(formatTime(NTP.getTimeDateString()));

  delay(8000);
  myVFD.print("Current Temp: " + (String)readTemp()); //print current temp reading to VFD
  delay(2000);

  myVFD.print("$" + btcPrice + " /BTC" + '\x0A' + '\x0D' + "$" + parseData(scrapeWeb(etheriumPrice),etheriumJson) + "   /ETH"); //display BTC price
  //myVFD.nextLine();

  //myVFD.simplePrint("$" + parseData(scrapeWeb(etheriumPrice),etheriumJson) + "  /ETH" ); //display BTC price
  delay(2000);
  

  myVFD.print("Miner: " + parseData(scrapeWeb(etheriumHashes),ethereumHashesJson) + " MH/s"); //display BTC price
  delay(2000);

}
