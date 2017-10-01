#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <string.h>
#include "display.h"
#include "transform.hpp"
#include "myAlgorithm.hpp"

using std::string;
using std::vector;
using std::function;
using std::begin;
using std::end;

vector<string> parseThatJson(string json) {
    vector<string> v{};
    auto i = json.find(':') + 1;
    while(true) {
        auto begin = json.find(':', i+1) + 1;
        auto end = json.find(',', begin);
        if(json[end-1] == '}') {
            v.push_back(json.substr(begin, end-1 - begin));
            break;
        }
        v.push_back(json.substr(begin, end - begin));
        i = end + 1;
    }
    return v;
}

function<vector<double>(string)> parseThatJsonToDoubles = [](string s)->vector<double>{
    return transform(parseThatJson(s), [](string s)->double{ return atof(s.c_str()); });
};


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
    operator string() {
        return string(x);
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
    string toString() {
        string str;
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

class BarGraph {
public:
    BarGraph(int width, int height) : width(width), height(height) {}
    // takes a bunch of ints all in the range of [0,7) and builds the character set
    void print(function<void(char)> f) {
        for (char c = '\x21'; c < '\x21' + 40; c++) {
            f(c);
        }
    }

    vector<int> normalize(vector<double> data) {
//        double max = maximum(data);
        double max = *std::max_element(begin(data), end(data));
        return transform(data, [=](double x)->int { // is this capturing height or this?
            return (int)(x / max * height);
        });
    }

    string set(vector<int> cols, function<void(char)> f) {
        if(cols.size() != width) return "length of cols is wrong";
        if(cols.size() % 5 != 0) return "length of cols is wrong"; // linter should highlight this
        if(cols.size() - 1 + '\x21' > '\xFF') return "length of cols is wrong";
        vector<char> cs {'\x1B','\x25','\x01','\x1B','\x26','\x01'};
        for ( auto c : cs ) f(c);
        f('\x21');
        f('\x21' + (40 - 1)); // todo - don't hard code 40, take number of rows into account
        vector<char> first {};
        vector<char> later {};
        for (auto x = begin(cols); x < end(cols); x += 5) {
            first.push_back('\x05');
            later.push_back('\x05');
            for (auto y = x; y < x + 5; y++) {
                int n = *y / 7;
                switch(n) {
                    case 2:
                        first.push_back(static_cast<char>(pow(2, 7) - 1));
                        later.push_back(static_cast<char>(pow(2, 7) - 1));
                        break;
                    case 1:
                        first.push_back(static_cast<char>(pow(2, *y % 7) - 1));
                        later.push_back(static_cast<char>(pow(2, 7) - 1));
                        break;
                    case 0:
                        first.push_back(static_cast<char>(0));
                        later.push_back(static_cast<char>(pow(2, *y) - 1));
                        break;
                    default:
                        break;
                }
            }
        }
        for(auto x : first) {
            f(x);
        }
        for(auto x : later) {
            f(x);
        }
        return "success";
    }

    vector<char> toChars(const vector<int> vec) {
        vector<char> bitPatterns;
        for (int col : vec) {
            bitPatterns.emplace_back(col);
        }
        return bitPatterns;
    };
private:
    int width;
    int height;

};

char first = '\x21';
char last = first + 39;

void deleteAll(VFD myVFD) {
    for (char a = '\x21'; a <= '\x21' + 39; a++) {
        myVFD.print("\x1B\x3F");
        myVFD.print(a);
    }
//    delay(1000);
}

//void grid(VFD myVFD) {
////    myVFD.print("\x1B\x40");
////    myVFD.print("\x1B\x52\x00");
//    deleteAll(myVFD);
//}

void messItUp(function<void(char)> f) {
    char counter = 0;
    vector<char> cs {'\x1B','\x25','\x01','\x1B','\x26','\x01'};
    for ( auto c : cs ) f(c);
    f(first);
    f(last);
    for (char a = first; a <= last; a++) {
        f('\x05');
        for (int i = 0; i < 5; i++) {
            f(counter++);
        }
    }
}

//void loop()
//{
////    if(WiFi.status() != WL_CONNECTED) connectToWifi(std::function<void(string)> {[](string str)->void { myVFD.print(str); }});
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
