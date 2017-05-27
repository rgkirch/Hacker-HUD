#include "display.h"

std::string CharacterDisplay::blockFormat(std::string left, std::string right) {
    int padding = width - left.length() - right.length();
    if (padding > 0) {
        left.resize(left.length() + padding, ' ');
    } else {
        right.resize(right.length() + padding);
    }
    return left + right;
}

void HardwareDisplay::setUpperLine(std::string str) {
    serial->print("\x0b\x01");
    serial->print(str);
};
void HardwareDisplay::setLowerLine(std::string str) {
    serial->print("\x0b\x02");
    serial->print(str);
};
