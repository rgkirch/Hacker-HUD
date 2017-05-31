#ifndef GTESTPROJ_ABSTRACTSERIAL_HPP
#define GTESTPROJ_ABSTRACTSERIAL_HPP

class AbstractSerial {
public:
    virtual size_t print(const char) =0;
    virtual size_t print(const char*) =0;
};


#endif //GTESTPROJ_ABSTRACTSERIAL_HPP
