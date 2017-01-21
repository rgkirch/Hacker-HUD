#include "vfdBuilder.h"

VFD* vfdBuilder::buildVFD(){
    return new VFD(rx, tx, displayWidth, displayHeight);
}

void vfdBuilder::setRx(unsigned char rx) {
    vfdBuilder::rx = rx;
}

unsigned char vfdBuilder::getTx() const {
    return tx;
}

void vfdBuilder::setTx(unsigned char tx) {
    vfdBuilder::tx = tx;
}

int vfdBuilder::getDisplayWidth() const {
    return displayWidth;
}

void vfdBuilder::setDisplayWidth(int displayWidth) {
    vfdBuilder::displayWidth = displayWidth;
}

int vfdBuilder::getDisplayHeight() const {
    return displayHeight;
}

void vfdBuilder::setDisplayHeight(int displayHeight) {
    vfdBuilder::displayHeight = displayHeight;
}
