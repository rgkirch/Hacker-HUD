#ifndef PROJECT_WIFICLIENTADAPTER_HPP
#define PROJECT_WIFICLIENTADAPTER_HPP

#include <cstddef>
#include <cstdint>

class MyClient {
public:
    virtual int connect(uint16_t port, const char *host) =0;
    virtual size_t print(const char[]) =0;
    virtual uint8_t connected() =0;
    virtual int read() =0;
    virtual void stop() =0;
};

#endif //PROJECT_WIFICLIENTADAPTER_HPP
