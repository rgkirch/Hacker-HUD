#include <WiFiUdp.h>
#include "vfd.hpp"

int MemoryClass::setSsid(const String& ssid)
{
    EEPROM.begin(512);
    if(ssid.length() > 64)
    {
        return -1;
    }
    if(ssid.indexOf('\0') != -1) // string can't have null in it
    {
        return -1;
    }
    for(int i = 0; i < ssid.length(); ++i)
    {
        EEPROM.write(e::SSID + i, ssid[i]);
    }
    EEPROM.write(e::SSID + ssid.length(), '\0');
    EEPROM.commit();
    return 1;
}

String MemoryClass::getSsid()
{
    EEPROM.begin(512);
    String ssid;
    char c;
    for(int i = e::SSID; (c = EEPROM.read(i)) != '\0' && i < 64; ++i)
    {
        ssid += (char)c;
    }
    return ssid;
}

int MemoryClass::setNetworkPassword(const String& networkPassword)
{
    EEPROM.begin(512);
    if(networkPassword.length() > 64)
    {
        return -1;
    }
    if(networkPassword.indexOf('\0') != -1) // string can't have null in it
    {
        return -1;
    }
    for(int i = 0; i < networkPassword.length(); ++i)
    {
        EEPROM.write(e::NETWORKPASSWORD + i, networkPassword[i]);
    }
    EEPROM.write(e::NETWORKPASSWORD + networkPassword.length(), '\0');
    EEPROM.commit();
    return 1;
}

String MemoryClass::getNetworkPassword()
{
    EEPROM.begin(512);
    String networkPassword;
    char c;
    for(int i = 0; (c = EEPROM.read(e::NETWORKPASSWORD + i)) != '\0' && i < 64; ++i)
    {
        networkPassword += (char)c;
    }
    return networkPassword;
}

size_t VFD::write(uint8_t b)
{
    if(delayTime > 0)
    {
        delay(delayTime);
    }
    return SoftwareSerial::write(b);
}

void VFD::clear()
{
    write(e::CLEAR);
    write(e::FORMFEED);
}
void VFD::newline()
{
    write(e::LINEFEED);
    write(e::CARRIAGERETURN);
}

void VFD::enableScroll()
{
    write(e::ENABLESCROLL);
}

void VFD::disableScroll()
{
    write(e::DISABLESCROLL);
}

void VFD::slowprint(String string)
{
    for(int i = 0; i < string.length(); ++i)
    {
        print(string[i]);
        delay(100);
    }
}

void VFD::setPrintSpeed(int charsPerSecond)
{
    if(charsPerSecond > 0)
    {
        delayTime = 1000 / charsPerSecond;
    }
}

void VFD::setCharacterPrintDelay(int time)
{
    if(time >= 0)
    {
        delayTime = time;
    }
}

TimeClass::TimeClass()
{
}
int TimeClass::currentTime()
{}

// send an NTP request to the time server at the given address
TimeClass::networkTime()
{
    WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
    IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
    const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

    memset(packetBuffer, 0, NTP_PACKET_SIZE); // set all bytes in the buffer to 0
    // Initialize values needed to form NTP request
    packetBuffer[0] = 0b11100011;     // LI, Version, Mode
    packetBuffer[1] = 3;         // Stratum, or type of clock
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


        String time;
        // print the hour, minute and second:
        //Serial.print("The UTC time is "); // UTC is the time at Greenwich Meridian (GMT)
        //Serial.print((epoch % 86400L) / 3600); // print the hour (86400 equals secs per day)
        time += ((epoch % 86400L) / 3600); // print the hour (86400 equals secs per day)
        //Serial.print(':');
        time += (':');
        if (((epoch % 3600) / 60) < 10)
        {
            // In the first 10 minutes of each hour, we'll want a leading '0'
            //Serial.print('0');
            time += ('0');
        }
        //Serial.print((epoch % 3600) / 60); // print the minute (3600 equals secs per minute)
        time += ((epoch % 3600) / 60); // print the minute (3600 equals secs per minute)
        //Serial.print(':');
        time += (':');
        if ((epoch % 60) < 10)
        {
            // In the first 10 seconds of each minute, we'll want a leading '0'
            //Serial.print('0');
            time += ('0');
        }
        //Serial.println(epoch % 60); // print the second
        time += (epoch % 60); // print the second
        return time;
    }
    return "for a better clock";
}


int TimeClass::setTimeZone()
{}

MemoryClass Memory;
