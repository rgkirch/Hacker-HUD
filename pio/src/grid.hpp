#include <string>
#include <functional>
#include <string.h>
#include <HardwareSerial.h>
#include <pins_arduino.h>
#include <os_type.h>
#include <Arduino.h>
#include "display.h"

char toByte(int number) {
    return (number / 1000000 % 2 << 6) + (number / 100000 % 2 << 5) + (number / 10000 % 2 << 4) + (number / 1000 % 2 << 3) + (number / 100 % 2 << 2) + (number / 10 % 2 << 1) + (number % 2);
}
class Char {
public:
    Char(const char *c) {
        for (int i = 0; i < 5; ++i) {
            x[i] = c[i];
        }
    };
    Char(Char& c) {
        *this = c;
    };
    Char& operator=(const Char &c) {
        if (this != &c) {
            for (int i = 0; i < 5; ++i) {
                x[i] = c.x[i];
            }
        }
        return *this;
    };
    operator std::string() {
        return std::string(x);
    };
    Char& operator&=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] &= c.x[i];
        }
        return *this;
    };
    const Char& operator&(const Char &c) {
        return Char(*this) &= c;
    };
    Char& operator|=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] |= c.x[i];
        }
        return *this;
    };
    Char& operator-=(const Char &c) {
        return (Char(*this) &= c) ^= *this;
    };
    Char& operator^=(const Char &c) {
        for (int i = 0; i < 5; ++i) {
            x[i] ^= c.x[i];
        }
        return *this;
    };
    const Char& operator^(const Char &c) {
        return Char(*this) ^= c;
    };
    const Char& operator|(const Char &c) {
        return Char(*this) |= c;
    };
    bool operator==(const Char &c) {
        if (this == &c) return true;
        for (int i = 0; i < 5; ++i) {
            if (x[i] != c.x[i]) return false;
        }
        return true;
    };
    bool operator!=(const Char &c) {
        return !(*this == c);
    };
    Char& operator~() {
        for (int i = 0; i < 5; ++i) {
            x[i] = ~x[i];
        }
        return *this;
    };
    char x[5] {0};
};
// grid 15 rows and 19*7+5 or 138
void map(const char *cs, std::function<void(char)> f) {
    for (unsigned int i = 0; i < strlen(cs); i++) {
        f(cs[i]);
    }
}

class Grid {
public:
    const int width = 138;
    const int height = 15;
//    void setOn(int x, int y) {
//        data[(y * width + x) / 8] |= (1 << (8 - ((y * width + x) % 8)));
//    };
//    void setOff(int x, int y) {
//        data[(y * width + x) / 8] &= ~(1 << (8 - ((y * width + x) % 8)));
//    };
    std::string toString() {
        std::string str;
        for (int y = 0; y < 14; y++) {
            for (int x = 0; x < 20 * 5; x++) {
                if ((data[x * 2 + y / 8] & (1 << (7 - (y % 8)))) != 0) {
                    str += '1';
                } else {
                    str += '0';
                }
            }
            str += '\n';
        }
        return str;
    };
    void setOn(int x, int y) {
        data[x / 8 + y / 2] |= (1 << (7 - ((y + 1) % 8)));
    };
    void setOff(int x, int y) {
        data[x / 8 + y / 2] &= ~(1 << (7 - ((y + 1) % 8)));
    };
    void dump(std::function<void(char)> print) {
        map("\x1B\x25\x01", print);
        map("\x1B\x26\x01", print);
        char first = '\x21';
        char last = first + 39;
        print(first);
        print(last);
        for (int x = 0; x < 40; x += 2) {
            print(data[x]);
        }
        for (int x = 1; x < 40; x += 2) {
            print(data[x]);
        }
    };
    char data[2 * 5 * 20] {0};
};

char first = '\x21';
char last = first + 39;
char counter = 0;

void deleteAll(VFD myVFD) {
    for (char a = '\x21'; a <= '\x21' + 39; a++) {
        myVFD.print("\x1B\x3F");
        myVFD.print(a);
    }
    delay(1000);
}

void grid(VFD myVFD) {
//    myVFD.print("\x1B\x40");
//    myVFD.print("\x1B\x52\x00");
    deleteAll(myVFD);
}

void messItUp(VFD myVFD) {
//    memset(grid.data, '\x88', 2 * 5 * 20);
//    grid.dump([](char x){myVFD.print(x);});
//    os_timer_arm(&myTimer, 1000, true);
    myVFD.print("\x1B\x25\x01");
    myVFD.print("\x1B\x26\x01");
    myVFD.print(first);
    myVFD.print(last);
    for (char a = first; a <= last; a++) {
        myVFD.print('\x05');
//        myVFD.print("\x63\x33\x15\x06\x03");
        for (int i = 0; i < 5; i++) {
            myVFD.print(counter++);
        }
    }

    myVFD.home();
    for (char c = '\x21'; c < '\x21' + 40; c++) {
        myVFD.print(c);
    }
//    Serial.println(grid.toString().c_str());
//    for (char a = first; a < last; a++) {
//        myVFD.print(a);
//        delay(200);
//    }
    delay(1000);
}
//void loop()
//{
////    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(std::string)> {[](std::string str)->void { myVFD.print(str); }});
//    if(WiFi.status() != WL_CONNECTED) connectToWifi();
//    unixTime = NTP.getTime();
//    unixTimeUpdated = millis();
//
//    updateSite(coindesk);
//    myVFD.setLowerLine("bitcoin", coindesk.lastResult.orElse("no data"));
//    delay(4000);
//
//    updateSite(coinMarketCap);
//    myVFD.setLowerLine("etherium", coinMarketCap.lastResult.orElse("no data"));
//    delay(4000);
//
//    updateSite(openWeatherMapTemp);
//    myVFD.setLowerLine("tampa temp", openWeatherMapTemp.lastResult.orElse("no data"));
//    delay(4000);
//
//    updateSite(openWeatherMapHumidity);
//    myVFD.setLowerLine("tampa humidity", openWeatherMapHumidity.lastResult.orElse("no data"));
//    delay(4000);
//
////    myVFD.clear();
////    myVFD.home();
////    myVFD.print("sensor temp "); //(char)223)
////    myVFD.println(readTemp(tempsensor));
////    myVFD.print("time "); //(char)223)
//
//    yield();
//}

// cd5220
// JsonObject.success()
