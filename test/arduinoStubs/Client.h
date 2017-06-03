#ifndef client_h
#define client_h
#include "Print.h"
#include "Stream.h"
//#include "IPAddress.h"

class Client: public Stream {
    public:
//        virtual int connect(IPAddress ip, uint16_t port) =0;
        virtual int connect(const char *host, uint16_t port) { return 0; };
        virtual size_t write(uint8_t) { return 0; };
        virtual size_t write(const uint8_t *buf, size_t size) { return 0; };
        virtual int available() { return 0; };
        virtual int read() { return 0; };
        virtual int read(uint8_t *buf, size_t size) { return 0; };
        virtual int peek() { return 0; };
        virtual void flush() {};
        virtual void stop() {};
        virtual uint8_t connected() { return 0; };
        virtual operator bool() { return false; };
    protected:
//        uint8_t* rawIPAddress(IPAddress& addr) {
//            return addr.raw_address();
//        };
};

#endif
