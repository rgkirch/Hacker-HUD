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

MemoryClass Memory;
