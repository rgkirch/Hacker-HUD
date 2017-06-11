//#ifndef SCRAPEWEB_HPP
//#define SCRAPEWEB_HPP
//
//#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
//#include <ESP8266WiFi.h>
//#include <string>
//#include <ArduinoJson.h>
//
//#include "option.hpp"
//#include "site.hpp"
//#include "createClient.hpp"
//#include "myClient.hpp"
//
//#define DEBUGPRINT
//#ifdef DEBUGPRINT
//#define LOG(x) do{Serial.println(x);}while(0)
//#else
//#define LOG(x)
//#endif
//
//Option<std::string> emptyOption;
//
////std::string readData(MyClient *client) {
//////    delay(1000); // actually helps
////    std::string data;
////    for(int read = 0; (read = client->read()) > -1; data.push_back(static_cast<char>(read)));
//////    for(char c; client->readBytes(&c, 1) > -1; data.push_back(c));
//////    for(uint8_t c; client->read(&c, 1) > -1; data.push_back(c));
////    auto i = data.find('{');
////    if (i == data.npos) {
////        i = 0;
////    }
////    client->stop();
////    delete client;
////    return data.substr(i);
////}
//
////Option<std::string> downloadData(int port, const char *host, const char *path) {
////    MyClient *client = createClient(port, host, path);
////    if(client == nullptr) {
////        return emptyOption;
////    }
////    return readData(client);
////}
//
//#endif
