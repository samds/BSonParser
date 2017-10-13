#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementDBPointerTest, ConstructorPlain)
{
    BsonElementDBPointer::ValueType v{'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'};
    BsonElementDBPointer elem("key", "desc", v);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("desc", elem.desc());
    EXPECT_EQ(v, elem.dBPointer());
}

TEST(BsonElementDBPointerTest, ExtractElementDBPointer)
{
    std::vector<char> buffer = {
        '\x0C',
        'k', 'e', 'y', '\x00',
        '\x02', '\x00', '\x00', '\x00', 'x', '\x00',
        '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto dbPointerElement = std::dynamic_pointer_cast<BsonElementDBPointer>(info.value);
    EXPECT_NE(nullptr, dbPointerElement);
    EXPECT_EQ("key", dbPointerElement->name());
    EXPECT_EQ("x", dbPointerElement->desc());
    
    BsonElementDBPointer::ValueType expected{'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'};

    EXPECT_EQ(expected, dbPointerElement->dBPointer());
}
