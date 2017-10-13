#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementJavascriptCodeTest, ConstructorPlain)
{
    BsonElementJavascriptCode elem("key", "code");
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("code", elem.code());
}

TEST(BsonElementJavascriptCodeTest, ExtractElementJavascriptCode)
{
    std::vector<char> buffer = {'\x0D', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto res = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), res.count);
    EXPECT_NE(nullptr, res.value);
    auto javaElement = std::dynamic_pointer_cast<BsonElementJavascriptCode>(res.value);
    EXPECT_NE(nullptr, javaElement);
    EXPECT_EQ("hello", javaElement->name());
    EXPECT_EQ("world", javaElement->code());
}
