#include <SoftwareSerial.h>

class VFD : public SoftwareSerial
{
public:
    VFD(uint8_t receivePin, uint8_t transmitPin) : SoftwareSerial(receivePin, transmitPin), delayTime(0) {}
    size_t write(uint8_t);
    void clear();
    void newline();
    void slowprint(String);
    void enableScroll();
    void disableScroll();
    void setPrintSpeed(int);
    void setCharacterPrintDelay(int);
    // void slowprint(arg) {delay(100); printchar();}
private:
    int delayTime;
    enum e {BACKSPACE = 8, TAB, LINEFEED, FORMFEED = 12, CARRIAGERETURN, CLEAR, DISABLESCROLL = 17, ENABLESCROLL, CURSOROFF = 20, CURSORON, ALTERNATECHARSET = 25, DEFAULTCHARSET};
};


