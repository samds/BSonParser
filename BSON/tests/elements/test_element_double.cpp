#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementDoubleTest, ConstructorPlain)
{
    BsonElementDouble elem("key", 3.14);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(3.14, elem.decimal64());
}

TEST(BsonElementDoubleTest, ExtractElementDouble)
{
    std::vector<char> buffer = {'\x01', 'k', 'e', 'y', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14', '\x40'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto res = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), res.count);
    EXPECT_NE(nullptr, res.value);
    auto doubleElement = std::dynamic_pointer_cast<BsonElementDouble>(res.value);
    EXPECT_NE(nullptr, doubleElement);
    EXPECT_EQ("key", doubleElement->name());
    EXPECT_EQ(5.05, doubleElement->decimal64());
}

TEST(BsonElementDoubleTest, ExtractElementDoubleOverflow)
{
    std::vector<char> buffer = {'\x01', 'k', 'e', 'y', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
