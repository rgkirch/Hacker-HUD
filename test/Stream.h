#ifndef Stream_h
#define Stream_h

#include <inttypes.h>
#include "Print.h"
enum LookaheadMode{
    SKIP_ALL,
    SKIP_NONE,
    SKIP_WHITESPACE
};

#define NO_IGNORE_CHAR  '\x01'

class Stream : public Print
{
protected:
    struct MultiTarget {
        const char *str;
        size_t len;
        size_t index;
    };
    virtual float parseFloat(char ignore) { return 0.0; }
    virtual int findMulti(struct MultiTarget *targets, int tCount) { return 0; };
    virtual int peekNextDigit(LookaheadMode lookahead, bool detectDecimal) { return 0; };
    virtual int timedPeek() { return 0; };
    virtual int timedRead() { return 0; };
    virtual long parseInt(char ignore) { return 0L; };
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    Stream() {};
//    virtual String readString() {};
//    virtual String readStringUntil(char terminator);
    virtual bool find(char *target) { return false; };
    virtual bool find(char *target, size_t length) { return false; };
    virtual bool find(char target) { return false; };
    virtual bool find(uint8_t *target) { return false; };
    virtual bool find(uint8_t *target, size_t length) { return false; };
    virtual bool findUntil(char *target, char *terminator) { return false; };
    virtual bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen) { return false; };
    virtual bool findUntil(uint8_t *target, char *terminator) { return false; };
    virtual bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) { return false; };
    virtual float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) { return 0.0; };
    virtual long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) { return 0L; };
    virtual size_t readBytes( char *buffer, size_t length) { return 0; };
    virtual size_t readBytes( uint8_t *buffer, size_t length) { return 0; };
    virtual size_t readBytesUntil( char terminator, char *buffer, size_t length) { return 0; };
    virtual size_t readBytesUntil( char terminator, uint8_t *buffer, size_t length) { return 0; };
    virtual unsigned long getTimeout(void) { return 0; };
    virtual void setTimeout(unsigned long timeout) {};
};

#endif
