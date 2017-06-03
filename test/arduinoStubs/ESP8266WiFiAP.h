#ifndef ESP8266WIFIAP_H_
#define ESP8266WIFIAP_H_


#include "ESP8266WiFiType.h"
#include "ESP8266WiFiGeneric.h"


class ESP8266WiFiAPClass {
    public:
        bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
        bool softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
        bool softAPdisconnect(bool wifioff = false);
        uint8_t softAPgetStationNum();
        IPAddress softAPIP();
        uint8_t* softAPmacAddress(uint8_t* mac);
        String softAPmacAddress(void);
    protected:
};

#endif /* ESP8266WIFIAP_H_*/
