#include <gtest/gtest.h>
#include <exception>
#include "BsonElement.hpp"
#include <stdexcept>


TEST(BsonElementNullTest, ConstructorPlain)
{
    BsonElementNull elem("key");
    EXPECT_EQ("key", elem.name());
}

TEST(BsonElementNullTest, ExtractElementNull)
{
    std::vector<char> buffer = {
        '\x0A',
        'k', 'e', 'y', '\x00'
    };
    std::stringstream ss;
    std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
    
    auto info = BsonElementFactory::extractElement(ss);
    EXPECT_EQ(buffer.size(), info.count);
    auto nullElement = std::dynamic_pointer_cast<BsonElementNull>(info.value);
    EXPECT_NE(nullptr, nullElement);
    EXPECT_EQ("key", nullElement->name());
}
