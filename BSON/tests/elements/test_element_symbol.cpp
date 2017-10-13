#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementSymbolTest, ConstructorPlain)
{
    BsonElementSymbol elem("key", "code");
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("code", elem.symbol());
}

TEST(BsonElementSymbolTest, ExtractElementSymbol)
{
    std::vector<char> buffer = {'\x0E', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto res = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), res.count);
    EXPECT_NE(nullptr, res.value);
    auto symbElement = std::dynamic_pointer_cast<BsonElementSymbol>(res.value);
    EXPECT_NE(nullptr, symbElement);
    EXPECT_EQ("hello", symbElement->name());
    EXPECT_EQ("world", symbElement->symbol());
}
