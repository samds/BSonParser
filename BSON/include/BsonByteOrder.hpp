/*
* Copyright (c) 2012-2016 Aldebaran Robotics. All rights reserved.
* Use of this source code is governed by a BSD-style license that can be
* found in the COPYING file.
*/

#pragma once
#ifndef _BSON_BYTE_ORDER_H_
#define _BSON_BYTE_ORDER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <iomanip>
#include <array>
#include <sstream>

// TODO Implement this feature with boost Endian library
namespace Bson {
    namespace ByteOrder { // Byte-Order Utilities
        inline int32_t int32_little_to_host(int32_t arg) {
            uint32_t result = arg;
            return result;
        }
        
        inline int64_t int64_little_to_host(int64_t arg) {
            int64_t result = arg;
            return result;
        }
        
        inline uint64_t uint64_little_to_host(uint64_t arg) {
            uint64_t result = arg;
            return result;
        }
        
        inline double double_little_to_host(double arg) {
            double result = arg;
            return result;
        }
        
        inline long double long_double_little_to_host(long double arg) {
            long double result = arg;
            return result;
        }
    }
}

#endif // _BSON_BYTE_ORDER_H_
