#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementCodeWSTest, ConstructorPlain)
{
    auto elem1 = std::make_shared<BsonElementDouble>("d", 3.14);
    auto elem2 = std::make_shared<BsonElementInt32>("i", 1986);
    BsonElementCodeWS::ValueType mapping{{elem1, elem2}};
    
    BsonElementCodeWS elem("key", "code", mapping);
    EXPECT_EQ("key", elem.name());
    EXPECT_EQ("code", elem.jsCode());
    EXPECT_EQ(3.14, std::dynamic_pointer_cast<BsonElementDouble>(elem["d"])->decimal64());
    EXPECT_EQ(1986, std::dynamic_pointer_cast<BsonElementInt32>(elem["i"])->int32());
}

TEST(BsonElementCodeWSTest, ExtractElementCodeWS)
{
    std::vector<char> buffer = {
        '\x0F',
        'k', 'e', 'y', '\x00', // e_name
        '\x21', '\x00', '\x00', '\x00', // int32
        '\x02', '\x00', '\x00', '\x00', 'x', '\x00', // string
        '\x17', '\x00', '\x00', '\x00', // doc ...
        '\x01', '\x30', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14', '\x40', // '0' : 5.05
        '\x10', '\x31', '\x00', '\xc2', '\x07', '\x00', '\x00', // '1' : 1986
        '\x00' // EOED
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto edElement = std::dynamic_pointer_cast<BsonElementCodeWS>(info.value);
    EXPECT_NE(nullptr, edElement);
    EXPECT_EQ("key", edElement->name());
    EXPECT_EQ("x", edElement->jsCode());

    const auto& vec = edElement->mapping();
    EXPECT_EQ(2, vec.size());
    auto doubleElem = std::dynamic_pointer_cast<BsonElementDouble>(vec[0]);
    EXPECT_EQ("0", doubleElem->name());
    EXPECT_EQ(5.05, doubleElem->decimal64());
    
    std::shared_ptr<BsonElement> elem = (*edElement)["0"];
    EXPECT_EQ(5.05, std::dynamic_pointer_cast<BsonElementDouble>(elem)->decimal64());
    
    elem = (*edElement)["1"];
    EXPECT_EQ(1986, std::dynamic_pointer_cast<BsonElementInt32>(elem)->int32());
    
    auto intElem = std::dynamic_pointer_cast<BsonElementInt32>(vec[1]);
    EXPECT_EQ("1", intElem->name());
    EXPECT_EQ(1986, intElem->int32());
}
/*
TEST(BsonElementEmbeddedDocumentTest, ExtractElementEmbeddedDocumentOverflow)
{
    std::vector<char> buffer = {
        // '\x21', '\x00', '\x00', '\x00', // size document
        '\x03', 'k', 'e', 'y', '\x00',
        '\x18', '\x00', '\x00', '\x00',     // size embedded document
        '\x01', '\x30', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14', '\x40', // '0' : 5.05
        '\x10', '\x31', '\x00', '\xc2', '\x07', '\x00', '\x00', // '1' : 1986
        // '\x00', // EOED
        //'\x00'  // EOD
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}

TEST(BsonElementEmbeddedDocumentTest, ExtractElementEmbeddedDocumentNotNullTerminated)
{
    std::vector<char> buffer = {
        // '\x21', '\x00', '\x00', '\x00', // size document
        '\x03', 'k', 'e', 'y', '\x00',
        '\x17', '\x00', '\x00', '\x00',     // size embedded document
        '\x01', '\x30', '\x00', '\x33', '\x33', '\x33', '\x33', '\x33', '\x33', '\x14', '\x40', // '0' : 5.05
        '\x10', '\x31', '\x00', '\xc2', '\x07', '\x00', '\x00', // '1' : 1986
        // '\x00', // EOED
        //'\x00'  // EOD
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    EXPECT_ANY_THROW(BsonElementFactory::extractElement(ss));
}
*/
