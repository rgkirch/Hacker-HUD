/*
HackerHUD Core Code V2
Coyt Barringer & Richie Kirchofer
3/13/17
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

#include <ArduinoJson.h>

String inString = "";    // string to hold input

//new types: -----
typedef struct{
    const char* url;
    const char* host;
    bool secure;
} website;

typedef struct{
    const char* keys[4];
    int numKeys;
    String output;
} keyStruct;



//constants -----
const char* ssid     = "HellSpot Slow";
const char* password = "ILikeWiFi";

//websites to parse -----
website coinDesk = {.url = "/v1/bpi/currentprice.json", .host = "api.coindesk.com", .secure = false};
//keyStruct coinDeskArray[] = {{.keys = {"bpi","USD","rate_float"},.numKeys = 3},{}};
//setup data to parse?


//.keys = {"price","usd"}, .numKeys = 2
website coinMarketCap = {.url = "/api/eth", .host = "coinmarketcap-nexuist.rhcloud.com", .secure = true};
//setup data to parse?

website openWeatherMap = {.url = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=f8ffd4de380fb081bfc12d4ee8c82d29", .host = "api.openweathermap.org", .secure = false};
keyStruct openWeatherMapData[] = {{.keys = {"main","temp"},.numKeys = 2},{.keys = {"main","humidity"},.numKeys = 2}};

//Initialize Objects -----
SoftwareSerial mySerial(D5,D6);//rx,tx
VFD myVFD(mySerial);

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); //for MCP9808


//setup ntp time -----
void ntpSetup() {
    NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
        if (error) {
            Serial.print("Time Sync error: ");
            if (error == noResponse)
                Serial.println("NTP server not reachable");
            else if (error == invalidAddress)
                Serial.println("Invalid NTP server address");
        } else {
            Serial.print("Got NTP time: ");
            Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
        }
    });

    NTP.begin("pool.ntp.org", -5, true);
    NTP.setInterval(1800);
}

//Function to connect and retrieve web data -----
String scrapeWeb(website site) {

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

    String realAnswer = answer.substring(answer.indexOf('{'));

    client->stop();
    Serial.println();
    Serial.println("closing connection");

    // Process answer
    Serial.println();
    Serial.println("Answer: ");
    Serial.println(realAnswer);

    delete client;
    return realAnswer;
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
    ntpSetup();
}

//Initialize Temp Sensor Fnc. -----
void InitializeTemp(){
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
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

//main setup -----
void setup() {
  Serial.begin(115200); //setup computer serial port
  delay(10); //max delay for wifi to work!

  InitializeWiFi(); //call to initialize wifi fnc. - DO NOT PUT A LARGE DELAY BEFORE THIS!!!!!

  myVFD.print("VFD Display code V1");
  myVFD.brightness(1);

  InitializeTemp(); //Initialize temp sensor

    pinMode(A0, INPUT);

}

void parseData(website site, keyStruct* keysToParse, int num){

    const size_t bufferSize = 4000;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject& root = jsonBuffer.parseObject(scrapeWeb(site));

    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
    }

    for (int i = 0; i < num ; i++){

        if(keysToParse[i].numKeys == 1){
            keysToParse[i].output = (const char*)root[keysToParse[i].keys[0]];
        }

        else if(keysToParse[i].numKeys == 2) {
            JsonObject& keyOne = root[keysToParse[i].keys[0]];
            keysToParse[i].output = (const char*)keyOne[keysToParse[i].keys[1]];
        }

        else if(keysToParse[i].numKeys == 3) {
            JsonObject& keyOne = root[keysToParse[i].keys[0]];
            JsonObject& keyTwo = keyOne[keysToParse[i].keys[1]];
            keysToParse[i].output = (const char*)keyTwo[keysToParse[i].keys[2]];
        }

        else if(keysToParse[i].numKeys == 4) {
            JsonObject& keyOne = root[keysToParse[i].keys[0]];
            JsonObject& keyTwo = keyOne[keysToParse[i].keys[1]];
            JsonObject& keyThree = keyTwo[keysToParse[i].keys[2]];
            keysToParse[i].output = (const char*)keyThree[keysToParse[i].keys[3]];
        }

        else
            keysToParse[i].output = "nothing found!";
    }
}

//Loop -----
void loop() {


    //startup frame and time
    myVFD.print("  Hacker HUD V1.0  ");
    myVFD.nextLine();
    myVFD.simplePrint((NTP.getTimeDateString()));
    delay(6000);

    //Temp
    //myVFD.print("Current Temp: " + (String)readTemp()); //print current temp reading to VFD
    //delay(4000);

    //BTC and ETH Price frame
    //String currentBtcPrice = (String)parseData(coinDesk,coinDeskArray,(sizeof(coinDeskArray)/sizeof(keyStruct))).toFloat();
    //String currentEthPrice = (String)parseData(coinMarketCap).toFloat();
    //myVFD.print("$" + currentBtcPrice + " /BTC");
    //myVFD.nextLine();
    //myVFD.simplePrint("$" + currentEthPrice + " /ETH");
    //delay(6000);


    //display weather
    float temp = analogRead(A0);
    temp = (((temp/1023)*3.3*100)*1.8) + 32;

    parseData(openWeatherMap,openWeatherMapData,(sizeof(openWeatherMapData)/sizeof(keyStruct)));
    myVFD.print("Temperature: " + openWeatherMapData[0].output + ((char)223) + "F");
    myVFD.nextLine();
    myVFD.simplePrint("Humidity: " + openWeatherMapData[1].output);
    delay(6000);
    myVFD.print("Inside: " + (String)temp + ((char)223) + "F");
    delay(6000);

    //display trump tweets
}