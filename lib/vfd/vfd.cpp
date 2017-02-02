#include "vfd.hpp"

VFD::VFD(int receivePin, int transmitPin, int displayWidth, int displayHeight) : width(displayWidth), height(displayHeight)
{
    softwareSerial = new SoftwareSerial(receivePin, transmitPin);
    softwareSerial->begin(9600);
    //together these Initialize the display
    softwareSerial->write('\x1B');
    softwareSerial->write('\x40');

    softwareSerial->write('\x0C'); //clear display
}

VFD::~VFD() {
    delete softwareSerial;
}

void VFD::print(int num)
{
    int base = 10;
    if(num == 0) this->print('0');
    int length = 0;
    int numberToPrint = num;
    while(num != 0)
    {
        num /= base;
        length++;
    }
    char* buffer = (char*)malloc(1 + length * sizeof(char));
    buffer[length] = '\0';
    sprintf(buffer, "%d", num);
    this->print(buffer, length);
}

int VFD::print(const char *c, int n) {
    if(c == nullptr) return 0;
    if(n < 0) n = 0;
    if(n > strlen(c)) n = strlen(c);
    int counter = 0;
    for (int i = 0; i < n; ++i) {
        softwareSerial->write(c[i]);
        counter++;
    }
    return counter;
}

void VFD::print(std::string str)
{
    for (char c:str)
    {
        this->write(c);
    }
}

void VFD::println(std::string str)
{
    this->print(str);
    //fnc for moving to next line
    this->write('\x1B');
    this->write('\x6C');
    this->write('\x01');
    this->write('\x02');

//fnc for just clearing the display
}

void VFD::print(char c) {
    this->write(c);
}

void VFD::write(char c) {
    softwareSerial->write(c);
}

void VFD::clear()
{
    this->write('\x0C'); //clear display
}

std::unique_ptr<VFD> VFD::Builder::build(){
    if(rx < 0) return nullptr;
    if(tx < 0) return nullptr;
    if(displayWidth < 0) return nullptr;
    if(displayHeight < 0) return nullptr;
    return std::unique_ptr<VFD>(new VFD(rx, tx, displayWidth, displayHeight));
}

VFD::Builder& VFD::Builder::setRx(int rx) {
    this->rx = rx;
    return *this;
}

VFD::Builder& VFD::Builder::setTx(int tx) {
    this->tx = tx;
    return *this;
}

VFD::Builder& VFD::Builder::setDisplayWidth(int displayWidth) {
    this->displayWidth = displayWidth;
    return *this;
}

VFD::Builder& VFD::Builder::setDisplayHeight(int displayHeight) {
    this->displayHeight = displayHeight;
    return *this;
}
