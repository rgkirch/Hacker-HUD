#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t

#include "WString.h"
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8

class Print
{
private:
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
protected:
    void setWriteError(int err = 1) {};
public:
    Print() {};
    virtual size_t write(uint8_t) = 0;
    virtual int availableForWrite() { return 0; };
    virtual int getWriteError() { return 0; };
    virtual size_t print(char) { return 0; };
    virtual size_t print(const Printable&) { return 0; };
    virtual size_t print(const String &) { return 0; };
    virtual size_t print(const char[]) { return 0; };
    virtual size_t print(double, int = 2) { return 0; };
    virtual size_t print(int, int = DEC) { return 0; };
    virtual size_t print(long, int = DEC) { return 0; };
    virtual size_t print(unsigned char, int = DEC) { return 0; };
    virtual size_t print(unsigned int, int = DEC) { return 0; };
    virtual size_t print(unsigned long, int = DEC) { return 0; };
    virtual size_t println(char) { return 0; };
    virtual size_t println(const Printable&) { return 0; };
    virtual size_t println(const String &s) { return 0; };
    virtual size_t println(const __FlashStringHelper *) { return 0; };
    virtual size_t println(const char[]) { return 0; };
    virtual size_t println(double, int = 2) { return 0; };
    virtual size_t println(int, int = DEC) { return 0; };
    virtual size_t println(long, int = DEC) { return 0; };
    virtual size_t println(unsigned char, int = DEC) { return 0; };
    virtual size_t println(unsigned int, int = DEC) { return 0; };
    virtual size_t println(unsigned long, int = DEC) { return 0; };
    virtual size_t println(void) { return 0; };
    virtual size_t write(const char *buffer, size_t size) { return 0; };
    virtual size_t write(const char *str) { return 0; };
    virtual size_t write(const uint8_t *buffer, size_t size) { return 0; };
    virtual void clearWriteError() {};
    virtual void flush() {};
};

#endif
