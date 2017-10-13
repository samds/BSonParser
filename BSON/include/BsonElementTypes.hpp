/*
* Copyright (c) 2012-2016 Aldebaran Robotics. All rights reserved.
* Use of this source code is governed by a BSD-style license that can be
* found in the COPYING file.
*/

#pragma once
#ifndef _BSON_ELEMENT_TYPES_H_
#define _BSON_ELEMENT_TYPES_H_

namespace Bson {
    
    //! Enum of Bson element types.
    enum class ElementType {
        EndOfDocument       = 0x00, //              End of document
        Double              = 0x01, // double       64-bit binary floating point
        String              = 0x02, // string       UTF-8 string
        EmbeddedDocument	= 0x03, // document     Embedded document
        Array               = 0x04, // document     Array
        Binary              = 0x05, // binary       Binary data
        Undefined           = 0x06, //              Undefined (value) — Deprecated
        ObjectId            = 0x07, // (byte*12)	ObjectId
        Boolean             = 0x08, // Boolean      "0x00" false or "0x01" true
        Datetime            = 0x09, // int64        UTC datetime
        Null                = 0x0A, //              Null value
        Regex               = 0x0B, // cstring*2    Regular expression - The first cstring is the regex pattern, the second is the regex options string. Options are identified by characters, which must be stored in alphabetical order. Valid options are 'i' for case insensitive matching, 'm' for multiline matching, 'x' for verbose mode, 'l' to make \w, \W, etc. locale dependent, 's' for dotall mode ('.' matches everything), and 'u' to make \w, \W, etc. match unicode.
        DBPointer           = 0x0C, // string       DBPointer — Deprecated
        JavascriptCode    	= 0x0D, // string       JavaScript code
        Symbol              = 0x0E, // string       Symbol. Deprecated
        JavascriptCodeWS    = 0x0F, // code_w_s     JavaScript code w/ scope
        Int32               = 0x10, // int32        32-bit integer
        Timestamp           = 0x11, // uint64       Timestamp
        Int64               = 0x12, // int64        64-bit integer
        Decimal128          = 0x13, // decimal128	128-bit decimal floating point
        MinKey              = 0xFF, //              Min key
        MaxKey              = 0x7F, //              Max key
    };
    
    //! List of all Bson element types.
    constexpr ElementType types[] = {
        ElementType::EndOfDocument,
        ElementType::Double,
        ElementType::String,
        ElementType::EmbeddedDocument,
        ElementType::Array,
        ElementType::Binary,
        ElementType::Undefined,
        ElementType::ObjectId,
        ElementType::Boolean,
        ElementType::Datetime,
        ElementType::Null,
        ElementType::Regex,
        ElementType::DBPointer,
        ElementType::JavascriptCode,
        ElementType::Symbol,
        ElementType::JavascriptCodeWS,
        ElementType::Int32,
        ElementType::Timestamp,
        ElementType::Int64,
        ElementType::Decimal128,
        ElementType::MinKey,
        ElementType::MaxKey
    };
    
    //! Enum of binary data subtypes.
    enum class BinarySubType {
        Generic             = 0x00, //	Generic binary subtype
        Function            = 0x01, // Function
        BinaryOld           = 0x02, // Binary (Old)
        UUIDOld             = 0x03, // UUID (Old)
        UUID                = 0x04, // UUID
        MD5                 = 0x05, // MD5
        UserDefined         = 0x80, // User defined
    };
    
    inline std::ostream& operator<<(std::ostream& os, ElementType c)
    {
        switch(c)
        {
            case ElementType::EndOfDocument      : os << "End of document";    break;
            case ElementType::Double             : os << "Double"; break;
            case ElementType::String             : os << "UTF-8 string";  break;
            case ElementType::EmbeddedDocument   : os << "Embedded document";  break;
            case ElementType::Array              : os << "Array";   break;
            case ElementType::Binary             : os << "Binary data";   break;
            case ElementType::Undefined          : os << "Undefined value";   break;
            case ElementType::ObjectId           : os << "ObjectId";   break;
            case ElementType::Int32              : os << "32-bit integer";   break;
            case ElementType::Regex              : os << "Regex expression";   break;
            default                              : os << static_cast<int>(c);//(std::ios_base::failbit);
        }
        return os;
    }
}

#endif // _BSON_ELEMENT_TYPES_H_
