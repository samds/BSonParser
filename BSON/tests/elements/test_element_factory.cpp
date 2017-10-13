#include <gtest/gtest.h>
#include <exception>
#include "BsonElementTypes.hpp"
#include "BsonElement.hpp"
#include "BsonElementFactory.hpp"
#include <stdexcept>
#include <chrono>


TEST(BsonElementFactoryTest, ExtractElementType)
{
    std::vector<char> buffer = {'\x02', 'h', 'e', 'l', 'l', 'o', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractElementType(ss);
    EXPECT_EQ(Bson::ElementType::String, info.value);
}

TEST(BsonElementFactoryTest, ExtractElementTypeThrow)
{
    std::vector<char> buffer;
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    EXPECT_ANY_THROW(Bson::extractElementType(ss));
}

TEST(BsonElementFactoryTest, ExtractsElementTypeThrowUnknownType)
{
    std::vector<char> buffer = { '\x77' };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    EXPECT_ANY_THROW(Bson::extractElementType(ss));
}

TEST(BsonElementFactoryTest, ExtractInt32)
{
    std::vector<char> buffer = {'\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractInt32(ss);
    EXPECT_EQ(4, info.count);
    EXPECT_EQ(6, info.value);
}

TEST(BsonElementFactoryTest, ExtractInt32Overflow)
{
    std::vector<char> buffer = {'\x06', '\x00', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(Bson::extractInt32(ss));
}

TEST(BsonElementFactoryTest, ExtractCString)
{
    std::vector<char> buffer = {'w', 'o', 'r', 'l', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractCString(ss);
    EXPECT_EQ(6, info.count);
    EXPECT_EQ("world", info.value);
}

TEST(BsonElementFactoryTest, ExtractCStringNull)
{
    std::vector<char> buffer = {'\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractCString(ss);
    EXPECT_EQ(1, info.count);
    EXPECT_EQ("", info.value);
}

TEST(BsonElementFactoryTest, ExtractCStringNotNullTerminated)
{
    std::vector<char> buffer = {'w', 'o', 'r', 'l', 'd'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(Bson::extractCString(ss));
}

TEST(BsonElementFactoryTest, ExtractCStringEmpty)
{
    std::vector<char> buffer{};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(Bson::extractCString(ss));
}

TEST(BsonElementFactoryTest, ExtractString)
{
    std::vector<char> buffer = {'\x05', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'd', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractString(ss);
    EXPECT_EQ(9, info.count);
    EXPECT_EQ("word", info.value);
}

TEST(BsonElementFactoryTest, ExtractStringNull)
{
    std::vector<char> buffer = {'\x01', '\x00', '\x00', '\x00', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = Bson::extractString(ss);
    EXPECT_EQ(5, info.count);
    EXPECT_EQ("", info.value);
}

TEST(BsonElementFactoryTest, ExtractStringNotNullTerminated)
{
    std::vector<char> buffer = {'\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(Bson::extractString(ss));
}

TEST(BsonElementFactoryTest, ExtractBytes)
{
    std::vector<char> buffer = {'\x01', '\x02', '\x03', '\x04'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    std::vector<Bson::Byte> expected = {'\x01', '\x02', '\x03', '\x04'};
    auto info = Bson::extractBytes(ss, 4);
    EXPECT_EQ(buffer.size(), info.count);
    EXPECT_EQ(expected, info.value);
}

TEST(BsonElementFactoryTest, ExtractBytesOverflow)
{
    std::vector<char> buffer = {'\x01', '\x02', '\x03', '\x04'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(Bson::extractBytes(ss, 5));
}

TEST(BsonElementFactoryTest, ExtractInt64)
{
    std::vector<char> buffer = {'\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\x7F'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto info = Bson::extractInt64(ss);
    EXPECT_EQ(buffer.size(), info.count);
    EXPECT_EQ(9223372036854775807LL, info.value);
}

TEST(BsonElementFactoryTest, ExtractUInt64)
{
    std::vector<char> buffer = {'\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto info = Bson::extractUInt64(ss);
    EXPECT_EQ(buffer.size(), info.count);
    EXPECT_EQ(18446744073709551615ULL, info.value);
}

TEST(BsonElementFactoryTest, ExtractDecimal128)
{
    std::vector<char> buffer = {'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x80', '\xFF', '\x3F', '\x40', '\x00', '\x00','\x00', '\x00', '\x00'};
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto info = Bson::extractDecimal128(ss);
    EXPECT_EQ(buffer.size(), info.count);
    EXPECT_EQ(1.0, info.value);
}


TEST(BsonElementFactoryTest, ExtractDocument)
{
    std::vector<char> buffer = {
        '\x17', '\x00', '\x00', '\x00',     // size document
        '\x01', '\x31', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14', '\x40', // '0' : 5.05
        '\x10', '\x32', '\x00', '\xc2', '\x07', '\x00', '\x00', // '1' : 1986
        '\x00' // EOD
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    BsonElement* elem = nullptr;
    auto info = Bson::extractDocument(ss);
    EXPECT_EQ(buffer.size(), info.count);
    EXPECT_EQ(23, info.value.size);
    EXPECT_NE(nullptr, info.value.elements[0]);
    EXPECT_NE(nullptr, info.value.elements[1]);
}
