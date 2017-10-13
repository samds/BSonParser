#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementBooleanTest, ConstructorPlain)
{
    {
        BsonElementBoolean elem("key", false);
        EXPECT_EQ("key", elem.name());
        EXPECT_EQ(false, elem.value());
    }
    {
        BsonElementBoolean elem("key", true);
        EXPECT_EQ("key", elem.name());
        EXPECT_EQ(true, elem.value());
    }
}

TEST(BsonElementBooleanTest, ExtractElementBooleanFalse)
{
    std::vector<char> buffer = {
        '\x08', 'k', 'e', 'y', '\x00', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto boolElement = std::dynamic_pointer_cast<BsonElementBoolean>(info.value);
    EXPECT_NE(nullptr, boolElement);
    EXPECT_EQ("key", boolElement->name());
    EXPECT_EQ(false, boolElement->value());
}

TEST(BsonElementBooleanTest, ExtractElementBooleanTrue)
{
    std::vector<char> buffer = {
        '\x08', 'k', 'e', 'y', '\x00', '\x01'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto boolElement = std::dynamic_pointer_cast<BsonElementBoolean>(info.value);
    EXPECT_NE(nullptr, boolElement);
    EXPECT_EQ("key", boolElement->name());
    EXPECT_EQ(true, boolElement->value());
}

TEST(BsonElementBooleanTest, ExtractElementBooleanOverflow)
{
    std::vector<char> buffer = {
        '\x08', 'k', 'e', 'y', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}

TEST(BsonElementBooleanTest, ExtractElementBooleanWrongValue)
{
    std::vector<char> buffer = {
        '\x08', 'k', 'e', 'y', '\x00', '\x02'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
