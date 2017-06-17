#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "display.h"

using ::testing::A;
using ::testing::StrictMock;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

class MockAbstractSerial : public MyPrint {
public:
    MockAbstractSerial(int rx, int tx) {};
    MOCK_METHOD1(print, size_t(const char));
    MOCK_METHOD1(print, size_t(const char *));
};

#define TESTBOTH(test_case_name, test_case, display_width, expected, ...) \
TESTUPPER(test_case_name, test_case ## Upper, display_width, expected, __VA_ARGS__) \
TESTLOWER(test_case_name, test_case ## Lower, display_width, expected, __VA_ARGS__)

#define TESTUPPER(test_case_name, test_case, display_width, expected, ...) TESTLINE(test_case_name, test_case, display_width, "\x1B\x51\x41", expected, "\x0D", setUpperLine, __VA_ARGS__)

#define TESTLOWER(test_case_name, test_case, display_width, expected, ...) TESTLINE(test_case_name, test_case, display_width, "\x1B\x51\x42", expected, "\x0D", setLowerLine, __VA_ARGS__)

#define TESTLINE(test_case_name, test_case, display_width, begin, expected, end, function, ...) \
TEST(test_case_name, test_case) { \
    StrictMock<MockAbstractSerial> serial(1, 2); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x40\x0C")))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq(begin)))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq(expected)))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq(end)))).Times(1); \
    VFD vfd(display_width, 2, &serial); \
    vfd.function(__VA_ARGS__); \
}

TESTBOTH(vfd, setLineHello, 20, "hello               ", "hello")
TESTBOTH(vfd, setLineCalledWithLongArgument, 18, "one two three four", "one two three four five six")
TESTBOTH(vfd, setLineEmptyString, 7, "       ", "")
TESTBOTH(vfd, setLineTwoStrings, 20, "temp             45C", "temp", "45C")
TESTBOTH(vfd, setLineTwoStringsTooSmallForRightMostAndSomeOfLeft, 8, "temperat", "temperature", "45C")
TESTBOTH(vfd, setLineTwoStringsTooSmallForRightString, 11, "temperature", "temperature", "45C")
TESTBOTH(vfd, setLineTwoStringsRightStringGetsChopped, 12, "temperature4", "temperature", "45C")
TESTBOTH(vfd, setLineTwoStringsLeftChopOneChar, 10, "temperatur", "temperature", "45C")

#undef TESTBOTH
#undef TESTUPPER
#undef TESTLOWER
#undef TESTLINE
