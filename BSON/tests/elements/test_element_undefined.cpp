#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementUndefinedTest, ConstructorPlain)
{
    BsonElementUndefined elem("key");
    EXPECT_EQ("key", elem.name());
}

TEST(BsonElementUndefinedTest, ExtractElementUndefined)
{
    std::vector<char> buffer = {
        '\x06',
        'k', 'e', 'y', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto undefElement = std::dynamic_pointer_cast<BsonElementUndefined>(info.value);
    EXPECT_NE(nullptr, undefElement);
    EXPECT_EQ("key", undefElement->name());
}
