#include <vector>
#include <functional>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grid.hpp"
#include "main.hpp"

using std::string;
using std::vector;
using std::function;
using ::testing::A;
using ::testing::StrictMock;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;



TEST(frameSelection, positive) {
    auto whichFrame = [](long currentTime) { return getWhichFrame(4, 4000, currentTime); };
    ASSERT_EQ(0, whichFrame(0));
    ASSERT_EQ(0, whichFrame(3999));
    ASSERT_EQ(1, whichFrame(4000));
    ASSERT_EQ(1, whichFrame(7999));
    ASSERT_EQ(2, whichFrame(8000));
    ASSERT_EQ(2, whichFrame(11999));
    ASSERT_EQ(3, whichFrame(12000));
    ASSERT_EQ(3, whichFrame(15999));
    ASSERT_EQ(0, whichFrame(16000));
}

TEST(frameSelection, negative) {
    auto whichFrame = [](long currentTime) { return getWhichFrame(4, 4000, currentTime); };
    ASSERT_EQ(3, whichFrame(-1));
    ASSERT_EQ(3, whichFrame(-3999));
    ASSERT_EQ(3, whichFrame(-4000));
    ASSERT_EQ(2, whichFrame(-7999));
    ASSERT_EQ(2, whichFrame(-8000));
    ASSERT_EQ(1, whichFrame(-11999));
    ASSERT_EQ(1, whichFrame(-12000));
    ASSERT_EQ(0, whichFrame(-15999));
    ASSERT_EQ(0, whichFrame(-16000));
}
