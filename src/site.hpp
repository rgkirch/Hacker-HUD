#ifndef SITE_HPP
#define SITE_HPP
#include <ArduinoJson.h>

const int httpPort = 80;
const int httpsPort = 443;

typedef struct {
    const char* host;
    const char* path;
    int port;
    std::function<std::string(JsonObject)> f;
//    typedef <typename T>
//    float(*f)(JsonObject);
} Site;

#endif
