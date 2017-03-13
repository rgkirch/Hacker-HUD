#ifndef SITE_HPP
#define SITE_HPP

const int httpPort = 80;
const int httpsPort = 443;

typedef struct {
    const char* host;
    const char* path;
    int port;
} Site;

#endif
