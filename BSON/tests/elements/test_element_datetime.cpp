#include <gtest/gtest.h>
#include <exception>
#include <chrono>
#include <stdexcept>
#include "BsonElement.hpp"
#include "BsonElementFactory.hpp"

using Clock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<Clock>;

TEST(BsonElementDateTimeTest, ConstructorPlain)
{
    std::chrono::milliseconds dur(10000);
    BsonElementDatetime elem("key", dur);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(dur, elem.datetime());

    std::tm timeinfo = std::tm();
    timeinfo.tm_year = 70;   // year: 1970 (since 1900)
    timeinfo.tm_mon = 0;      // month: january
    timeinfo.tm_mday = 1;     // day: 1st
    timeinfo.tm_hour = -5;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 10;
    std::time_t tt = std::mktime (&timeinfo);
    std::cout << "Time expected: " << std::asctime(std::gmtime(&tt)) << std::endl;
    auto expected = std::chrono::system_clock::from_time_t(tt);
    EXPECT_EQ(expected, elem.timePoint());
}
