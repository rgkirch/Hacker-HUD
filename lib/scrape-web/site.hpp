#ifndef SITE_HPP
#define SITE_HPP

const int httpPort = 80;
const int httpsPort = 443;

struct Site {
    const char* host;
    const char* path;
    int port;
};

#endif
