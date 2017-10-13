#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementBinaryDataTest, ConstructorPlain)
{
    BsonElementBinary::ValueType bytes = {{'\x01', '\x02'}};
    BsonElementBinary elem("key", Bson::BinarySubType::Generic, bytes);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(Bson::BinarySubType::Generic, elem.subType());
    EXPECT_EQ(bytes, elem.bytes());
}

TEST(BsonElementBinaryDataTest, ExtractElementBinary)
{
    std::vector<char> buffer = {
        '\x05',
        'k', 'e', 'y', '\x00',
        '\x02', '\x00', '\x00', '\x00',
        '\x00',
        '\x01', '\x02'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto binaryElement = std::dynamic_pointer_cast<BsonElementBinary>(info.value);
    EXPECT_NE(nullptr, binaryElement);
    EXPECT_EQ("key", binaryElement->name());
    EXPECT_EQ(Bson::BinarySubType::Generic, binaryElement->subType());
    BsonElementBinary::ValueType expected = {{'\x01', '\x02'}};
    EXPECT_EQ(expected, binaryElement->bytes());
}

TEST(BsonElementBinaryDataTest, ExtractElementBinaryThrowOverflow)
{
    std::vector<char> buffer = {
        '\x05',
        'k', 'e', 'y', '\x00',
        '\x03', '\x00', '\x00', '\x00',
        '\x00',
        '\x01', '\x02'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
