#ifndef VFDBUILDER_H
#define VFDBUILDER_H

#include "vfd.hpp"

class vfdBuilder
{
public:
    VFD* buildVFD();
    unsigned char getRx() const;
    void setRx(unsigned char rx);
    unsigned char getTx() const;
    void setTx(unsigned char tx);
    int getDisplayWidth() const;
    void setDisplayWidth(int displayWidth);
    int getDisplayHeight() const;
    void setDisplayHeight(int displayHeight);
private:
    unsigned char rx;
    unsigned char tx;
    int displayWidth;
    int displayHeight;
};

#endif
