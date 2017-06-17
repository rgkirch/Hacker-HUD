#ifndef MYPRINT_HPP
#define MYPRINT_HPP

class MyPrint {
public:
    virtual size_t print(const char) =0;
    virtual size_t print(const char*) =0;
};


#endif //MYSERIAL_HPP
