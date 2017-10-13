/*
* Copyright (c) 2012-2016 Aldebaran Robotics. All rights reserved.
* Use of this source code is governed by a BSD-style license that can be
* found in the COPYING file.
*/

#pragma once
#ifndef _BSON_TYPES_H_
#define _BSON_TYPES_H_

#include <cstdint>
#include <limits>

//! Bson namespace
namespace Bson {
    
    static_assert(CHAR_BIT == 8, "Platform not supported");
    static_assert(sizeof(char) == 1, "Platform not supported");
    static_assert(sizeof(double) == 8, "Platform not supported");
    static_assert(std::numeric_limits<double>::is_iec559, "Platform not supported");
    static_assert(sizeof(long double) == 16, "Platform not supported");
    static_assert(std::numeric_limits<long double>::is_iec559, "Platform not supported");

    //! 1 byte (8-bits)
    typedef uint8_t     Byte;
    //! 4 bytes (32-bit signed integer, two's complement)
    typedef int32_t     Int32;
    //! 8 bytes (64-bit signed integer, two's complement)
    typedef int64_t     Int64;
    //! 8 bytes (64-bit unsigned integer)
    typedef uint64_t    UInt64;
    //! 8 bytes (64-bit IEEE 754-2008 binary floating point)
    typedef double      Double;
    //! 16 bytes (128-bit IEEE 754-2008 decimal floating point)
    typedef long double Decimal128;
}

#endif // _BSON_TYPES_H_
