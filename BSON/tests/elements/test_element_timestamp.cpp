#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementTimestampTest, ConstructorPlain)
{
    BsonElementTimestamp elem("key", 1234, 5678);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(1234, elem.increment());
    EXPECT_EQ(5678, elem.timestamp());
}

TEST(BsonElementTimestampTest, BsonElementTimestamp)
{
    std::vector<char> buffer = {
        '\x11',
        'k', 'e', 'y', '\x00',
        '\xD2', '\x04', '\x00', '\x00', '\x2E', '\x16', '\x00', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto tsElement = std::dynamic_pointer_cast<BsonElementTimestamp>(info.value);
    EXPECT_NE(nullptr, tsElement);
    EXPECT_EQ("key", tsElement->name());
    EXPECT_EQ(1234, tsElement->increment());
    EXPECT_EQ(5678, tsElement->timestamp());
}
