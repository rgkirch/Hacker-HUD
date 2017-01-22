#ifndef SCRAPEWEB_HPP
#define SCRAPEWEB_HPP
const char* host = "api.coindesk.com";
const char* hostEth = "api.nanopool.org";
const char* hostTime = "script.google.com";

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

#endif
