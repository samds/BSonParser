/*
 * Copyright (c) 2012-2016 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */
#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>
#include "BsonFileParser.hpp"
#include "BsonDocument.hpp"
#include "BsonElement.hpp"

TEST(BsonStreamParserTest, ConstructorVector)
{
    BsonStreamParser parser;
    auto bytes = {
        '\x16', '\x00', '\x00', '\x00',                                     // total document size
        '\x02',                                                             // 0x02 = type String
        'h', 'e', 'l', 'l', 'o', '\x00',                                    // field name
        '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00',    // field value
        '\x00'};
    auto doc = parser.parseBinaryArray(bytes);
    std::shared_ptr<BsonElementString> elem = std::dynamic_pointer_cast<BsonElementString>(doc["hello"]);
    EXPECT_EQ("world", elem->text());
}

TEST(BsonStreamParserTest, ConstructorThrowIfCantExtractSize)
{
    {
        BsonStreamParser parser;
        auto bytes = {'\x16', '\x00', '\x00'};
        EXPECT_ANY_THROW(parser.parseBinaryArray(bytes));
    }
    
    {
        BsonStreamParser parser;
        std::vector<char> bytes;
        EXPECT_ANY_THROW(parser.parseBinaryArray(bytes));
    }
}

TEST(BsonStreamParserTest, ConstructorThrowIfCantExtractEOO)
{
    BsonStreamParser parser;
    auto bytes = { '\x16', '\x00', '\x00', '\x00', '\x02', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00'};
    EXPECT_EQ(0x15, bytes.size());
    EXPECT_ANY_THROW(parser.parseBinaryArray(bytes));
}

TEST(BsonStreamParserTest, ConstructorThrowIfOverflow)
{
    BsonStreamParser parser;
    auto bytes = { '\x17', '\x00', '\x00', '\x00', '\x02', 'h', 'e', 'l', 'l', 'o', '\x00', '\x06', '\x00', '\x00', '\x00', 'w', 'o', 'r', 'l', 'd', '\x00', '\x00'};
    EXPECT_EQ(0x16, bytes.size());
    //EXPECT_ANY_THROW(parser.parseBinaryArray(bytes));
}
