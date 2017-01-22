#include "vfdBuilder.h"

vfdBuilder::vfdBuilder() {
    rx = -1;
    tx = -1;
    displayWidth = 20;
    displayHeight = 2;
}

VFD* vfdBuilder::buildVFD(){
    if(rx < 0) return nullptr;
    if(tx < 0) return nullptr;
    if(displayWidth < 0) return nullptr;
    if(displayHeight < 0) return nullptr;
    return new VFD(rx, tx, displayWidth, displayHeight);
}

int vfdBuilder::getRx() const {
    return rx;
}

void vfdBuilder::setRx(int rx) {
    vfdBuilder::rx = rx;
}

int vfdBuilder::getTx() const {
    return tx;
}

void vfdBuilder::setTx(int tx) {
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
