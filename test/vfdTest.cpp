#include "vfd.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//class Mock

TEST(CharacterDisplay_setUpperLine, basic_test) {
    VFD *myVFD = VFD::Builder().setRx(5).setTx(6).setDisplayWidth(20).setDisplayHeight(2).build();
}


