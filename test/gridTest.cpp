#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "grid.hpp"
#include <string>
#include <vector>

using ::testing::A;
using ::testing::StrictMock;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;


// long version
const string sampleJson {R"cocksucker!({"bpi":{"2017-03-17":1070.128,"2017-03-18":970.598,"2017-03-19":1017.8,"2017-03-20":1041.343,"2017-03-21":1115.039,"2017-03-22":1037.44,"2017-03-23":1029.95,"2017-03-24":935.946,"2017-03-25":964.692,"2017-03-26":965.229,"2017-03-27":1040.491,"2017-03-28":1044.251,"2017-03-29":1040.392,"2017-03-30":1037.527,"2017-03-31":1079.748,"2017-04-01":1089.511,"2017-04-02":1098.776,"2017-04-03":1147.631,"2017-04-04":1143.746,"2017-04-05":1134.998,"2017-04-06":1190.597,"2017-04-07":1193.019,"2017-04-08":1184.824,"2017-04-09":1210.055,"2017-04-10":1213.339,"2017-04-11":1224.767,"2017-04-12":1216.505,"2017-04-13":1178.533,"2017-04-14":1183.44,"2017-04-15":1180.699,"2017-04-16":1184.787,"2017-04-17":1203.731,"2017-04-18":1217.596,"2017-04-19":1226.939,"2017-04-20":1255.403,"2017-04-21":1257.135,"2017-04-22":1244.375,"2017-04-23":1248.2175,"2017-04-24":1248.325,"2017-04-25":1263.545,"2017-04-26":1284.845,"2017-04-27":1329.19,"2017-04-28":1320.0513,"2017-04-29":1327.0388,"2017-04-30":1347.9588,"2017-05-01":1402.0838,"2017-05-02":1443.6825,"2017-05-03":1491.9988,"2017-05-04":1515.6288,"2017-05-05":1512.2088,"2017-05-06":1548.2863,"2017-05-07":1555.4713,"2017-05-08":1639.3225,"2017-05-09":1706.9313,"2017-05-10":1756.8025,"2017-05-11":1807.3725,"2017-05-12":1676.9938,"2017-05-13":1759.9613,"2017-05-14":1772.4163,"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138,"2017-06-24":2617.9375},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};

using std::string;
using std::vector;

TEST(grid, set) {
    vector<char> expected {'\x1b','\x25','\x01','\x1b','\x26','\x01', // send command to start programming
                  '\x21','\x48', // start at 21 and stop at 21+40
                  '\x05','\x7f','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x00','\x05','\x00','\x00','\x00','\x00','\x01','\x05','\x01','\x00','\x01','\x01','\x00','\x05','\x00','\x01','\x01','\x03','\x03','\x05','\x03','\x03','\x07','\x0f','\x07','\x05','\x07','\x03','\x07','\x07','\x07','\x05','\x07','\x0f','\x0f','\x0f','\x0f','\x05','\x1f','\x3f','\x1f','\x3f','\x3f','\x05','\x3f','\x7f','\x1f','\x1f','\x0f','\x05','\x0f','\x0f','\x1f','\x0f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x7f','\x0f','\x0f','\x0f','\x1f','\x05','\x0f','\x0f','\x0f','\x0f','\x0f','\x05','\x0f','\x0f','\x0f','\x0f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x1f','\x1f','\x1f','\x1f','\x1f','\x05','\x1f','\x3f','\x3f','\x3f','\x3f','\x05','\x3f','\x3f','\x3f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f','\x05','\x7f','\x7f','\x7f','\x7f','\x7f'};
    BarGraph grid(100, 14);
    vector<char> saved{};
    function<void(char)> g = [&](char c)->void { saved.push_back(c); };

    auto normalized = grid.normalize(parseThatJsonToDoubles(sampleJson));
    auto msg = grid.set(normalized, g);
    ASSERT_EQ(expected, saved);
}

//template<class InputIt, class OutputIt, class UnaryOperation>
//OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
//    while (first1 != last1) {
//        *d_first++ = unary_op(*first1++);
//    }
//    return d_first;
//}

//TEST(grid, one) {
//    BarGraph grid(20, 2);
//    vector<int> vec {1,3,7,15,31,1,3,7,15,31, 1,3,7,15,31,1,3,7,15,31, 1,3,7,15,31,1,3,7,15,31, 1,3,7,15,31,1,3,7,15,31};
//    auto chars = grid.toChars(vec);
//    auto msg = grid.set(chars, [](char c){ myVFD.print(c); });
//    grid.print([](char c){ });
//}

TEST(grid, transform) {
    string sampleJson {R"cocksucker!({"bpi":{"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
    vector<double> expected {1697.3788,1718.2013,1802.1638,1887.3263,1968.1025,2051.735,2055.6175,2139.0275,2291.4775,2476.2963,2357.5038,2247.4825,2106.3075,2207.5775,2289.87,2197.2338,2330.2338,2452.1813,2517.4088,2555.6538,2552.8088,2736.595,2914.0825,2694.2188,2825.0313,2826.7,2942.345,3018.545,2682.595,2738.9313,2494.485,2456.9238,2528.1025,2663.9975,2576.1713,2641.665,2778.8275,2712.1575,2740.79,2738.2138};
    auto result = parseThatJsonToDoubles(sampleJson);
    ASSERT_EQ(expected, result);
}

//TEST(grid, two) {
//    string sampleJson {R"cocksucker!({"bpi":{"2017-05-15":1697.3788,"2017-05-16":1718.2013,"2017-05-17":1802.1638,"2017-05-18":1887.3263,"2017-05-19":1968.1025,"2017-05-20":2051.735,"2017-05-21":2055.6175,"2017-05-22":2139.0275,"2017-05-23":2291.4775,"2017-05-24":2476.2963,"2017-05-25":2357.5038,"2017-05-26":2247.4825,"2017-05-27":2106.3075,"2017-05-28":2207.5775,"2017-05-29":2289.87,"2017-05-30":2197.2338,"2017-05-31":2330.2338,"2017-06-01":2452.1813,"2017-06-02":2517.4088,"2017-06-03":2555.6538,"2017-06-04":2552.8088,"2017-06-05":2736.595,"2017-06-06":2914.0825,"2017-06-07":2694.2188,"2017-06-08":2825.0313,"2017-06-09":2826.7,"2017-06-10":2942.345,"2017-06-11":3018.545,"2017-06-12":2682.595,"2017-06-13":2738.9313,"2017-06-14":2494.485,"2017-06-15":2456.9238,"2017-06-16":2528.1025,"2017-06-17":2663.9975,"2017-06-18":2576.1713,"2017-06-19":2641.665,"2017-06-20":2778.8275,"2017-06-21":2712.1575,"2017-06-22":2740.79,"2017-06-23":2738.2138},"disclaimer":"This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD.","time":{"updated":"Jun 25, 2017 00:03:00 UTC","updatedISO":"2017-06-25T00:03:00+00:00"}})cocksucker!"};
//    vector<double> expected {1697.3788,1718.2013,1802.1638,1887.3263,1968.1025,2051.735,2055.6175,2139.0275,2291.4775,2476.2963,2357.5038,2247.4825,2106.3075,2207.5775,2289.87,2197.2338,2330.2338,2452.1813,2517.4088,2555.6538,2552.8088,2736.595,2914.0825,2694.2188,2825.0313,2826.7,2942.345,3018.545,2682.595,2738.9313,2494.485,2456.9238,2528.1025,2663.9975,2576.1713,2641.665,2778.8275,2712.1575,2740.79,2738.2138};
//    auto graphData = parseThatJson(sampleJson);
//    vector<double> data;
//    data.resize(graphData.size());
//    transform(begin(graphData), end(graphData), std::back_inserter(data), [](string s)->double{ return atof(s.c_str()); });
//    ASSERT_EQ(expected, data);
//}