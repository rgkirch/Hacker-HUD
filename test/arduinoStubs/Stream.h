#ifndef Stream_h
#define Stream_h

#include <inttypes.h>
#include "Print.h"

class Stream: public Print {
protected:
    virtual int timedRead() {return 0; };
    virtual int timedPeek() {return 0; };
    virtual int peekNextDigit() {return 0; };

public:
    virtual int available() { return 0; };
    virtual int read() { return 0; };
    virtual int peek() { return 0; };
    virtual void flush() {};

//    Stream() {};
//    virtual ~Stream() {};
    void setTimeout(unsigned long timeout) {};
    bool find(const char *target) {};
    bool find(uint8_t *target) {};

    bool find(const char *target, size_t length) { return false; };
    bool find(const uint8_t *target, size_t length) { return false; };

    bool find(char target) { return false; };

    bool findUntil(const char *target, const char *terminator) { return false; };
    bool findUntil(const uint8_t *target, const char *terminator) { return false; };
    bool findUntil(const char *target, size_t targetLen, const char *terminate, size_t termLen) { return false; };
    bool findUntil(const uint8_t *target, size_t targetLen, const char *terminate, size_t termLen) { return false; };
    long parseInt() { return 0; };
    float parseFloat() { return 0.0; };
    virtual size_t readBytes(char *buffer, size_t length) { return 0; };
    virtual size_t readBytes(uint8_t *buffer, size_t length) { return 0; };
    size_t readBytesUntil(char terminator, char *buffer, size_t length) { return 0; };
    size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length) { return 0; };
    String readString() { return String(); };
    String readStringUntil(char terminator) { return String(); };
protected:
    long parseInt(char skipChar) { return 0; };
    float parseFloat(char skipChar) { return 0.0; };
};

#endif
