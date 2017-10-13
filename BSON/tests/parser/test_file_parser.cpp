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

#ifndef BINARY_DIR
#define BINARY_DIR "tests/binary"
#endif

TEST(BsonFileParserTest, ConstructorThrow)
{
    BsonFileParser parser;
    ASSERT_THROW(parser.parseFile("fake.bson"), std::ios_base::failure);
}

TEST(BsonFileParserTest, Constructor)
{
    BsonFileParser parser;
    ASSERT_NO_THROW(parser.parseFile(BINARY_DIR"/test.bson"));
    //BsonDocument doc = parser.parseFile("test.bson");
}
