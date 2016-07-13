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

#include <SPI.h>

// global variables first
const int httpPort = 80;

//NTP BS -------------------------------------
char networkSSID[] = "HellSpot Slow"; //    your network SSID (name)
char networkPassword[] = "ILikeWiFi"; // your network password
//WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
const unsigned int udpLocalPort = 2390; // local port to listen for UDP packets
//int keyIndex = 0; // your network key Index number (needed only for WEP)
//const String timeServer("time.nist.gov"); // time.nist.gov NTP server
/*
const int NTP_PACKET_SIZE = 48;
byte sendBuffer[] = {
  0b11100011,          // LI, Version, Mode.
  0x0,                 // Stratum unspecified.
  0x6,                 // Polling interval
  0xEC,                // Clock precision.
  0x0, 0x0, 0x0, 0x0}; // Reference ...
*/


// WiFi settings -----------------------------
//const String ssid("HellSpot Slow");
//const String password("ILikeWiFi");


// function declarations second
unsigned long networkTime();
void connectToWifi(const String& ssid, const String& password);
int getBitcoinPrice();

/*
class VFD
{
public:
    VFD(VFD&);
    SoftwareSerial mySerial;
private:
    VFD();
};
*/

    SoftwareSerial mySerial(D0,D1); //rx,tx

void setup()
{
    // Serial
    Serial.begin(115200);
    while(!Serial); // /shrug
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
    connectToWifi(networkSSID, networkPassword);
}

void loop()
{
    // bitcoin price
    int price = getBitcoinPrice();
    Serial.print("Bitcoin price: " + getBitcoinPrice());
    mySerial.write('\x0E');
    mySerial.write('\x0C');
    mySerial.print("Bitcoin $: ");
    mySerial.print(price);
    mySerial.print("    ");
    // time
    float time = networkTime();
    Serial.println(time);
    mySerial.print(time);

    // Wait 5 seconds
    delay(8000);
}

void connectToWifi(const String& ssid, const String& password)
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

int getBitcoinPrice()
{
    const String coindesk("api.coindesk.com");
    // Connect to API
    Serial.println("connecting to " + coindesk);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    int status = 0;
    for(int i = 0; i < 4; ++i)
    {
        status = client.connect(coindesk.c_str(), httpPort);
        if(!status)
        {
            Serial.println(String("Connection to " + coindesk + " failed. Trying again..."));
            delay(500);
        } else {
            client.println("GET /v1/bpi/currentprice/USD.json HTTP/1.1\r\nhost: api.coindesk.com\n\nConnection: close\r\n\r\n");
            client.println();
            
            delay(500);

            // Read all the lines of the reply from server and print them to Serial
            String answer;
            while(client.available())
            {
                String line = client.readStringUntil('\r');
                answer += line;
            }
            client.stop();
            // Convert to JSON
            String jsonAnswer;
            int jsonIndex;
            for (int i = 0; i < answer.length(); i++)
            {
                if (answer[i] == '{')
                {
                    jsonIndex = i;
                    break;
                }
            }
            // Get JSON data
            jsonAnswer = answer.substring(jsonIndex);
            jsonAnswer.trim();
            // Get rate as float
            int rateIndex = jsonAnswer.indexOf("rate_float");
            String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
            priceString.trim();
            float price = priceString.toFloat();
            return price;
        }
    }
    return -1;
}

/*
 * © Francesco Potortì 2013 - GPLv3
 *
 * Send an HTTP packet and wait for the response, return the Unix time
 */

unsigned long webUnixTime ()
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
            if (client.find((char *)"\r\nDate: ") // look for Date: header
	    && client.readBytes(buf, 5) == 5) // discard
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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// send an NTP request to the time server at the given address
unsigned long networkTime()
{
    WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
    IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
    const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

    memset(packetBuffer, 0, NTP_PACKET_SIZE); // set all bytes in the buffer to 0
    // Initialize values needed to form NTP request
    packetBuffer[0] = 0b11100011;     // LI, Version, Mode
    packetBuffer[1] = 0;         // Stratum, or type of clock
    packetBuffer[2] = 6;         // Polling Interval
    packetBuffer[3] = 0xEC;    // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values
    // now you can send a packet requesting a timestamp:
    udp.begin(udpLocalPort);
    while (udp.parsePacket() > 0); // discard any previously received packets
    udp.beginPacket(timeServer, 123); //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();

    delay(1000);
    if (udp.parsePacket())
    {
        Serial.println("packet received");
        // We've received a packet, read the data from it
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, extract the two words:

        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        Serial.print("Seconds since Jan 1 1900 = ");
        Serial.println(secsSince1900);

        // now convert NTP time into everyday time:
        Serial.print("Unix time = ");
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
        // print Unix time:
        Serial.println(epoch);


        // print the hour, minute and second:
        Serial.print("The UTC time is "); // UTC is the time at Greenwich Meridian (GMT)
        Serial.print((epoch % 86400L) / 3600); // print the hour (86400 equals secs per day)
        Serial.print(':');
        if (((epoch % 3600) / 60) < 10)
        {
            // In the first 10 minutes of each hour, we'll want a leading '0'
            Serial.print('0');
        }
        Serial.print((epoch % 3600) / 60); // print the minute (3600 equals secs per minute)
        Serial.print(':');
        if ((epoch % 60) < 10)
        {
            // In the first 10 seconds of each minute, we'll want a leading '0'
            Serial.print('0');
        }
        Serial.println(epoch % 60); // print the second
    }
}

/*
VFD::VFD()
{
    mySerial = SoftwareSerial(D0,D1); //rx,tx
}
*/
