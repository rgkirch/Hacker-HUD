#ifndef VFDBUILDER_H
#define VFDBUILDER_H

#include "vfd.hpp"

class vfdBuilder
{
public:
    vfdBuilder();
    VFD* buildVFD();
    int getRx() const;
    void setRx(int rx);
    int getTx() const;
    void setTx(int tx);
    int getDisplayWidth() const;
    void setDisplayWidth(int displayWidth);
    int getDisplayHeight() const;
    void setDisplayHeight(int displayHeight);
private:
    int rx;
    int tx;
    int displayWidth;
    int displayHeight;
};

#endif
