#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementOIDTest, ConstructorPlain)
{
    BsonElementObjectId::ValueType bytes = {'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'};
    BsonElementObjectId elem("key", bytes);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ(bytes , elem.objectID());
}

TEST(BsonElementOIDTest, ExtractElementOID)
{
    constexpr char typeChar = static_cast<char>(Bson::ElementType::ObjectId);
    std::vector<char> buffer = {typeChar, 'k', 'e', 'y', '\x00',
        '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto res = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), res.count);
    auto oidElement = std::dynamic_pointer_cast<BsonElementObjectId>(res.value);
    EXPECT_NE(nullptr, oidElement);
    BsonElementObjectId::ValueType expected{{'\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C'}};
    EXPECT_EQ("key", oidElement->name());
    EXPECT_EQ(expected, oidElement->objectID());
}

TEST(BsonElementOIDTest, ExtractElementOIDOverflow)
{
    constexpr char typeChar = static_cast<char>(Bson::ElementType::ObjectId);
    std::vector<char> buffer = {typeChar, 'k', 'e', 'y', '\x00',
        '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
