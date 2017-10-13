#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementStringTest, ConstructorPlain)
{
    BsonElementString elem("key", "3.14");
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("3.14", elem.text());
}

TEST(BsonElementStringTest, ExtractElementUTF8String)
{
    std::vector<char> buffer = {'\x02', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto res = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), res.count);
    EXPECT_NE(nullptr, res.value);
    auto stringElement = std::dynamic_pointer_cast<BsonElementString>(res.value);
    EXPECT_NE(nullptr, stringElement);
    EXPECT_EQ("hello", stringElement->name());
    EXPECT_EQ("world", stringElement->text());
}

TEST(BsonElementStringTest, ExtractElementUTF8StringOverflow)
{
    std::vector<char> buffer = {'\x02', 'h', 'e', 'l', 'l', 'o', '\x00', '\x07', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}

TEST(BsonElementStringTest, ExtractElementUTF8StringNotNullTerminated)
{
    std::vector<char> buffer = {'\x02', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '!'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
