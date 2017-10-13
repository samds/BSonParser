#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementMinMaxKeyTest, ConstructorPlain)
{
    BsonElementUndefined elem("key");
    EXPECT_EQ("key", elem.name());
}

TEST(BsonElementMinMaxKeyTest, ExtractElementMinKey)
{
    std::vector<char> buffer = {
        '\xFF',
        'k', 'e', 'y', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto element = std::dynamic_pointer_cast<BsonElementMinKey>(info.value);
    EXPECT_NE(nullptr, element);
    EXPECT_EQ("key", element->name());
}

TEST(BsonElementMinMaxKeyTest, ExtractElementMaxKey)
{
    std::vector<char> buffer = {
        '\x7F',
        'k', 'e', 'y', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto element = std::dynamic_pointer_cast<BsonElementMaxKey>(info.value);
    EXPECT_NE(nullptr, element);
    EXPECT_EQ("key", element->name());
}
