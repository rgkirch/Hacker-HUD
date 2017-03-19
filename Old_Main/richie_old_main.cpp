#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <SPI.h>
#include <EEPROM.h>

#include "vfd.hpp"

#include <tuple>
#include "globals.hpp"

//#define WL_MAX_ATTEMPT_CONNECTION 10
//#define WL_DELAY_START_CONNECTION 5000

// global variables first
const int httpPort = 80;
extern String networkSSID; // your network SSID (name)
extern String networkPassword; // your network password
extern const unsigned int udpLocalPort; // local port to listen for UDP packets

// function declarations second
String networkTime();
int connectToWifi();
int connectToWifi(const String& ssid, const String& password);
int getBitcoinPrice();
unsigned long webUnixTime();
void printWifiStatus(Print& stream);
void printEspInfo();
void OTAupdate();

VFD vfd(D0, D1);

void setup()
{
    // Serial
    Serial.begin(115200);
    while(!Serial);
    //vfd = new VFD(D0,D1); //rx,tx;
    vfd.begin(19200);
    while(!vfd);

    vfd.clear();
    connectToWifi();

    if(WiFi.status() == WL_CONNECTED)
    {
        vfd.clear();
        vfd.print("WiFi IP");
        vfd.newline();
        vfd.print(WiFi.localIP());
    }

    delay(1000);

    //printWifiStatus(Serial);
    //vfd.enableScroll();
    //vfd.setPrintSpeed(10);
    //printWifiStatus(vfd);
    //vfd.setCharacterPrintDelay(0);

    vfd.setPrintSpeed(10);
    //Memory.setSsid(networkSSID);
    //Memory.setNetworkPassword(networkPassword);
}

void loop()
{
    //Serial.println("max attempt connection " + WL_MAX_ATTEMPT_CONNECTION);
    // bitcoin price
    if(1)
    {
        int price = getBitcoinPrice();
        Serial.print("Bitcoin price: ");
        Serial.println(price);
        vfd.clear();
        vfd.print("Bitcoin $: ");
        vfd.print(price);
        delay(4000);

        // time
        String time = networkTime();
        Serial.println(time);
        vfd.clear();
        vfd.print("Time: ");
        vfd.print(time);
        delay(4000);
    }
    //OTAupdate();
}


int connectToWifi()
{
    return connectToWifi(Memory.getSsid(), Memory.getNetworkPassword());
}

int connectToWifi(const String& ssid, const String& password)
{
    if(WiFi.status() == WL_CONNECTED)
    {
        return 1;
    }
    if(!ssid)
    {
        vfd.print("no ssid given");
        return -1;
    }
    int time = millis();
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        vfd.print(".");
        if(millis() - time > 20000)
        {
            Serial.print("\nrestarting device");
            ESP.restart();
            return -1;
        }
    }
    return 1;
}

int getBitcoinPrice()
{
    const String coindesk("api.coindesk.com");
    // Connect to API
    //Serial.println("connecting to " + coindesk);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    int status = 0;
    status = client.connect(coindesk.c_str(), httpPort);
    if(!status)
    {
        Serial.println("Connection to " + coindesk + " failed. Trying again...");
        delay(500);
    } else {
        client.println("GET /v1/bpi/currentprice/USD.json HTTP/1.1\r\nhost: api.coindesk.com\n\nConnection: close\r\n\r\n");
        client.println();

        delay(500);

        // Read all the lines of the reply from server and print them to Serial
        String response;
        while(client.available())
        {
            response += client.readStringUntil('\r');
        }
        //Serial.println(response);
        client.stop();
        // Convert to JSON
        int index = response.indexOf("rate_float");
        String priceString = response.substring(index + 12, index + 18);
        priceString.trim();
        float price = priceString.toFloat();
        return price;
    }
    return -1;
}

/*
 * © Francesco Potortì 2013 - GPLv3
 *
 * Send an HTTP packet and wait for the response, return the Unix time
 */

unsigned long webUnixTime()
{
    WiFiClient client;
    unsigned long time = 0;

    // Just choose any reasonably busy web server, the load is really low
    if (client.connect("g.cn", 80))
    {
        // Make an HTTP 1.1 request which is missing a Host: header
        // compliant servers are required to answer with an error that includes
        // a Date: header.
        client.print(F("GET / HTTP/1.1 \r\n\r\n"));

        char buf[5]; // temporary buffer for characters
        client.setTimeout(5000);
        if(client.find((char *)"\r\nDate: ") && client.readBytes(buf, 5) == 5) // look for Date: header; discard
        {
            unsigned day = client.parseInt(); // day
            client.readBytes(buf, 1); // discard
            client.readBytes(buf, 3); // month
            int year = client.parseInt(); // year
            byte hour = client.parseInt(); // hour
            byte minute = client.parseInt(); // minute
            byte second = client.parseInt(); // second

            int daysInPrevMonths;
            switch (buf[0])
            {
                case 'F': daysInPrevMonths = 31; break; // Feb
                case 'S': daysInPrevMonths = 243; break; // Sep
                case 'O': daysInPrevMonths = 273; break; // Oct
                case 'N': daysInPrevMonths = 304; break; // Nov
                case 'D': daysInPrevMonths = 334; break; // Dec
                default:
                if (buf[0] == 'J' && buf[1] == 'a')
        daysInPrevMonths = 0;		// Jan
                else if (buf[0] == 'A' && buf[1] == 'p')
        daysInPrevMonths = 90;		// Apr
                else switch (buf[2])
                {
                case 'r': daysInPrevMonths =    59; break; // Mar
                case 'y': daysInPrevMonths = 120; break; // May
                case 'n': daysInPrevMonths = 151; break; // Jun
                case 'l': daysInPrevMonths = 181; break; // Jul
                default: // add a default label here to avoid compiler warning
                case 'g': daysInPrevMonths = 212; break; // Aug
                }
            }

            // This code will not work after February 2100
            // because it does not account for 2100 not being a leap year and because
            // we use the day variable as accumulator, which would overflow in 2149
            day += (year - 1970) * 365;	// days from 1970 to the whole past year
            day += (year - 1969) >> 2;	// plus one day per leap year
            day += daysInPrevMonths;	// plus days for previous months this year
            if (daysInPrevMonths >= 59	// if we are past February
                    && ((year & 3) == 0))	// and this is a leap year
                day += 1;			// add one day
            // Remove today, add hours, minutes and seconds this month
            time = (((day-1ul) * 24 + hour) * 60 + minute) * 60 + second;
        }
    }
    delay(10);
    client.flush();
    client.stop();

    return time;
}

void printWifiStatus(Print& stream) {
  // print the SSID of the network you're attached to:
  stream.println();
  stream.print("SSID: ");
  stream.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  stream.print("IP: ");
  stream.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  stream.print("RSSI:");
  stream.print(rssi);
  stream.println(" dBm");
}

void OTAupdate()
{
    //Serial.print("\nmode set to station");
    //WiFi.mode(WIFI_STA);
    Serial.print("\nconnect to wifi... again");
    connectToWifi();
    Serial.print("\nafter wifi func");
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    Serial.print("\nafter while loop");

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        Serial.println("Start");
        vfd.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
        vfd.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        vfd.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        vfd.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            Serial.println("Auth Failed");
            vfd.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println("Begin Failed");
            vfd.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println("Connect Failed");
            vfd.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println("Receive Failed");
            vfd.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println("End Failed");
            vfd.println("End Failed");
        }
    });

    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    vfd.newline();
    vfd.println("hostname");
    vfd.print(ArduinoOTA.getHostname());
    delay(1000);
    vfd.newline();
    vfd.print("updating");

    while(1)
    {
        ArduinoOTA.handle();
        delay(500);
    }
    Serial.print("\nafter while 1.. should never get here");
}

void printEspInfo()
{
    Serial.println(ESP.getVcc());
    Serial.println(ESP.getFreeHeap());
    Serial.println(ESP.getChipId());
    Serial.println(ESP.getSdkVersion());
    Serial.println(ESP.getCoreVersion());
    Serial.println(ESP.getBootVersion());
    Serial.println(ESP.getBootMode());
    Serial.println(ESP.getCpuFreqMHz());
    Serial.println(ESP.getFlashChipId());
    Serial.println(ESP.getFlashChipRealSize()); //gets the actual chip size based on the flash id
    Serial.println(ESP.getFlashChipSize()); //gets the size of the flash as set by the compiler
    Serial.println(ESP.getFlashChipSpeed());
    Serial.println(ESP.getFlashChipMode());
    Serial.println(ESP.getFlashChipSizeByChipId());
}

//<p class="TweetTextSize TweetTextSize--26px js-tweet-text tweet-text" lang="en" data-aria-label-part="0">
//http://www.esp8266.com/wiki/doku.php?id=ota-over-the-air-esp8266

//8 Move Cursor Left (backspace)
//9 	Move Cursor Right (tab)
//10 	Move Cursor Down (linefeed)
//12 	Move Cursor To Top Left (formfeed)
//13 	Move Cursor To Line Start (carriage return)
//14 	Clear Display
//17 	Disable Scroll (default)
//18 	Enable Scroll
//20 	Cursor Off (default)
//21 	Cursor On
//22 	Cursor Off
//23 	Cursor Off
//25 	Enable Character Set 2
//26 	Enable Character Set 1 (default)
//27 - 255 	Display Visible ASCII Characters