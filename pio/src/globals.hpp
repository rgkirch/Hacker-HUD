#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#define LOGN(x) do{Serial.print(x);}while(0)

#define DEBUGPRINT
#ifdef DEBUGPRINT
#define LOG(x) do{Serial.println(x);}while(0)
#else
#define LOG(x)
#endif

const char ssid[] = "HellSpot Slow";
const char password[] = "ILikeWiFi";
const int httpPort = 80;
const int httpsPort = 443;
//const unsigned int udpLocalPort = 2390;

#endif
