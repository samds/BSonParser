#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementInt32Test, ConstructorPlain)
{
    BsonElementInt32 elem("key", 1986);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(1986, elem.int32());
}

TEST(BsonElementInt32Test, ExtractElementInt32)
{
    std::vector<char> buffer = {
        '\x10',
        'k', 'e', 'y', '\x00',
        '\xc2', '\x07', '\x00', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto undefElement = std::dynamic_pointer_cast<BsonElementInt32>(info.value);
    EXPECT_NE(nullptr, undefElement);
    EXPECT_EQ("key", undefElement->name());
    EXPECT_EQ(1986, undefElement->int32());
}
