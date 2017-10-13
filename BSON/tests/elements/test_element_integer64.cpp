#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementInt64Test, ConstructorPlain)
{
    BsonElementInt64 elem("key", 0x7FFFFFFFFFFFFFFF);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, elem.int64());
}

TEST(BsonElementInt64Test, ExtractElementInt64)
{
    std::vector<char> buffer = {
        '\x12',
        'k', 'e', 'y', '\x00',
        '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\x7F'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto int64Element = std::dynamic_pointer_cast<BsonElementInt64>(info.value);
    EXPECT_NE(nullptr, int64Element);
    EXPECT_EQ("key", int64Element->name());
    EXPECT_EQ(0x7FFFFFFFFFFFFFFF, int64Element->int64());
}
