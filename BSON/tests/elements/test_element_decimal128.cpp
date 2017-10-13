#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementDecimal128Test, ConstructorPlain)
{
    BsonElementDecimal128 elem("key", 5.05);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(5.05, elem.decimal128());
}

TEST(BsonElementDecimal128Test, ExtractElementDecimal128)
{
    std::vector<char> buffer = {
        '\x13',
        'k', 'e', 'y', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\xA0',
        '\x02', '\x40', '\x40', '\x00', '\x00', '\x00', '\x00', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto decimalElement = std::dynamic_pointer_cast<BsonElementDecimal128>(info.value);
    EXPECT_NE(nullptr, decimalElement);
    EXPECT_EQ("key", decimalElement->name());
    EXPECT_EQ(10., decimalElement->decimal128());
}

TEST(BsonElementDecimal128Test, ExtractElementDecimal128Overflow)
{
    std::vector<char> buffer = {
        '\x13',
        'k', 'e', 'y', '\x00',
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
        '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));

    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
