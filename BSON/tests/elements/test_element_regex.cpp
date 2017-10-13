#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementRegexTest, ConstructorPlain)
{
    BsonElementRegex elem("key", "[a-z]+\\.bson", "i");
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("[a-z]+\\.bson", elem.regexPattern());
    EXPECT_EQ("i", elem.regexOptions());
}

TEST(BsonElementRegexTest, ExtractElementRegex)
{
    std::vector<char> buffer = {
        '\x0B',
        'k', 'e', 'y', '\x00',
        'e', '\x00',
        'i', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto regexElement = std::dynamic_pointer_cast<BsonElementRegex>(info.value);
    EXPECT_NE(nullptr, regexElement);
    EXPECT_EQ("key", regexElement->name());
    EXPECT_EQ("e", regexElement->regexPattern());
    EXPECT_EQ("i", regexElement->regexOptions());
}
