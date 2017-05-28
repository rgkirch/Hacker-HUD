#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <Stream.h>

class SoftwareSerial : public Stream
{
public:
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false) {};
    ~SoftwareSerial() {};
    virtual void begin(long speed) {};
    virtual bool listen() {return false; };
    virtual void end() {};
    virtual bool isListening() {return false; };
    virtual bool stopListening() {return false; };
    virtual bool overflow() {return false; };
    virtual int peek() {return 0; };

    virtual size_t write(uint8_t byte) { return 0; };
    virtual int read() { return 0; };
    virtual int available() { return 0; };
    virtual void flush() {};
    virtual operator bool() { return false; }

    using Print::write;
};

#endif
