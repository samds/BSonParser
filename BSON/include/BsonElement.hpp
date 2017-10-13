
#pragma once
#ifndef _BSON_ELEMENT_H_
#define _BSON_ELEMENT_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <array>
#include <sstream>
#include <chrono>

#include "BsonTypes.hpp"
#include "BsonElementTypes.hpp"
#include "BsonByteOrder.hpp"
#include "BsonElementBase.hpp"
#include "BsonElementFactory.hpp"
#include "BsonStreamParser.hpp"


/*!
 * \class BsonElementEndOfDocument
 * \brief Represents the BSon end of document.
 */
class BsonElementEndOfDocument : public BsonElement {
public:
    
    BsonElementEndOfDocument() : BsonElement("") {
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::EndOfDocument;
    }
};

/*!
 * \class BsonElementDouble
 * \brief Represents the BSON Double type.
 */
class BsonElementDouble : public BsonElement {
public:
    typedef Bson::Double        ValueType;
private:
    ValueType _value;
public:
    
    BsonElementDouble(std::string name, ValueType value) : BsonElement(name), _value(value) {}
  
    /*!
     * \brief Gets the double value.
     * \return Returns the double value.
     */
    ValueType decimal64() const {
        return _value;
    }

    Bson::ElementType id() const override
    {
        return Bson::ElementType::Double;
    }
};

/*!
 * \class BsonElementString
 * \brief Represents the BSON String type.
 */
class BsonElementString : public BsonElement {
    std::string _value;
public:
    
    BsonElementString(std::string n, std::string s) : BsonElement(n), _value(s) {}
    
    /*!
     * \brief Gets the string value.
     * \return Returns the string value.
     */
    std::string text() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::String;
    }
};

/*!
 * \class BsonElementEmbeddedDocument
 * \brief Represents the BSON Embedded Document type.
 */
class BsonElementEmbeddedDocument : public BsonElement {
public:
    typedef std::vector<std::shared_ptr<BsonElement>>      ValueType;
private:
    ValueType _value;
public:
    BsonElementEmbeddedDocument(std::string n, ValueType arr) : BsonElement(n),  _value(arr) {}
    
    /*!
     * \brief Gets the Bson element with given name.
     * \return Returns the Bson element with given name or nullptr if does't exist.
     */
    std::shared_ptr<BsonElement> operator[](std::string name) const {
        auto it = std::find_if(_value.cbegin(), _value.cend(), [name](auto sp){ return sp->name() == name; });
        return it != _value.cend() ? *it : std::shared_ptr<BsonElement>();
    }
    
    /*!
     * \brief Gets the embedded document value.
     * \return Returns the embedded document value.
     */
    const ValueType& EmbeddedDocument() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::EmbeddedDocument;
    }
};

/*!
 * \class BsonElementArray
 * \brief Represents the BSON Array type.
 */
class BsonElementArray : public BsonElement {
public:
    typedef std::vector<std::shared_ptr<BsonElement>>      ValueType;
private:
    ValueType _value;
public:
    
    BsonElementArray(std::string n, ValueType arr) : BsonElement(n), _value(arr) {
        unsigned long long idx = 0;
        for (auto it = _value.cbegin(); it != _value.cend(); ++it) {
            if ((*it)->name() != std::to_string(idx++)) {
                throw std::logic_error("Array elements should be indexed starting with 0 and continuing sequentially.");
            }
        }
    }
    
    /*!
     * \brief Gets the array values.
     * \return Returns the array values.
     */
    ValueType Array() const {
        return _value;
    }
    
    /*!
     * \brief Gets the Bson element at given index.
     * \return Returns the Bson element at given index.
     */
    std::shared_ptr<BsonElement> operator[](std::size_t pos) const {
        return _value.at(pos);
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Array;
    }
};

/*!
 * \class BsonElementBinary
 * \brief Represents the BSON Binary data type.
 */
class BsonElementBinary : public BsonElement {
public:
    typedef std::vector<Bson::Byte>     ValueType;
private:
    Bson::BinarySubType _subType;
    ValueType _bytes;
public:
    
    BsonElementBinary(std::string n, Bson::BinarySubType bst, ValueType b) : BsonElement(n), _subType(bst), _bytes(b) {}
    
    /*!
     * \brief Gets the binary data value.
     * \return Returns the binary data value.
     */
    ValueType bytes() const {
        return _bytes;
    }
    
    /*!
     * \brief Gets the binary data subtype.
     * \return Returns the binary data subtype.
     */
    Bson::BinarySubType subType() const {
        return _subType;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Binary;
    }
};

/*!
 * \class BsonElementUndefined
 * \deprecated Deprecated since v1.1 use BsonElementNull instead.
 * \brief Represents the BSON Undefined value type.
 */
class BsonElementUndefined : public BsonElement {
public:
    
    BsonElementUndefined(std::string n) : BsonElement(n) {};
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Undefined;
    }
};

/*!
 * \class BsonElementObjectId
 * \brief Represents the BSON ObjectId type.
 */
class BsonElementObjectId : public BsonElement {
public:
    typedef std::array<Bson::Byte, 12>      ValueType;
    ValueType objectId;
    
    BsonElementObjectId(std::string n, ValueType arr) : BsonElement(n), objectId(arr) {}
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::ObjectId;
    }
    
    /*!
     * \brief Gets the ObjectId value.
     * \return Returns the ObjectId value.
     */
    const ValueType& objectID() const {
        return objectId;
    }
    
};

/*!
 * \class BsonElementBoolean
 * \brief Represents the BSON Boolean type.
 */
class BsonElementBoolean : public BsonElement {
    bool _value;
public:
    
    BsonElementBoolean(std::string n, bool b) : BsonElement(n), _value(b) {}

    /*!
     * \brief Gets the boolean value.
     * \return Returns the boolean value.
     */
    bool value() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Boolean;
    }
};

/*!
 * \class BsonElementDatetime
 * \brief Represents the BSON datetime type.
 */
class BsonElementDatetime : public BsonElement {
public:
    typedef std::chrono::duration<Bson::Int64, std::milli>          ValueType;
private:
    ValueType _datetime;
public:
    
    BsonElementDatetime(std::string n, ValueType dt) : BsonElement(n), _datetime(dt) {}
    
    /*!
     * \brief Gets the duration since Unix epoch in ms.
     * \return Returns the duration since Unix epoch in ms.
     */
    ValueType datetime() const {
        return _datetime;
    }
    
    /*!
     * \brief Gets the datetime value.
     * \return Returns the datetime value.
     */
    std::chrono::system_clock::time_point timePoint() const {
        return std::chrono::system_clock::time_point( _datetime );
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Datetime;
    }
};

/*!
 * \class BsonElementNull
 * \brief Represents the BSON Null type.
 */
class BsonElementNull : public BsonElement {
public:
    BsonElementNull(std::string n) : BsonElement(n) {}
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Null;
    }
};

/*!
 * \class BsonElementRegex
 * \brief Represents the BSON Regular expression type.
 */
class BsonElementRegex : public BsonElement {
    std::string _regexPattern, _regexOptions;
public:
    
    BsonElementRegex(std::string n, std::string regexPattern, std::string regexOptions) : BsonElement(n), _regexPattern(regexPattern), _regexOptions(regexOptions) {};
    
    /*!
     * \brief Gets the regex pattern value.
     * \return Returns the regex pattern value.
     */
    std::string regexPattern() const {
        return _regexPattern;
    }
    
    /*!
     * \brief Gets the regex options value.
     * \return Returns the regex options value.
     */
    std::string regexOptions() const {
        return _regexOptions;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Regex;
    }
};

/*!
 * \class BsonElementDBPointer
 * \deprecated Deprecated since v1.1
 * \brief Represents the BSON DBPointer type.
 */
class BsonElementDBPointer : public BsonElement {
public:
    typedef std::array<Bson::Byte, 12>      ValueType;
private:
    std::string _desc;
    ValueType _dpPointer;
public:
    BsonElementDBPointer(std::string n, std::string s, ValueType dbp) : BsonElement(n), _desc(s), _dpPointer(dbp) {}
    
    /*!
     * \brief Gets the DBPointer value.
     * \return Returns the DBPointer value.
     */
    ValueType dBPointer() const {
        return _dpPointer;
    }
    
    /*!
     * \brief
     * \return
     */
    std::string desc() const {
        return _desc;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::DBPointer;
    }
};

/*!
 * \class BsonElementJavascriptCode
 * \brief Represents the BSON Javascript code type.
 */
class BsonElementJavascriptCode : public BsonElementString {
public:
    BsonElementJavascriptCode(std::string n, std::string s) : BsonElementString(n, s) {}
    
    /*!
     * \brief Gets the javascript code value.
     * \return Returns the javascript code value.
     */
    std::string code() const {
        return text();
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::JavascriptCode;
    }
};

/*!
 * \class BsonElementSymbol
 * \deprecated Deprecated since v1.1
 * \brief Represents the BSON Symbol type.
 */
class BsonElementSymbol : public BsonElementString {
public:
    BsonElementSymbol(std::string n, std::string s) : BsonElementString(n, s) {}
    
    /*!
     * \brief
     * \return
     */
    std::string symbol() const {
        return text();
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Symbol;
    }
};

/*!
 * \class BsonElementCodeWS
 * \brief Represents the BSON JavaScript code w/ scope type.
 */
class BsonElementCodeWS : public BsonElement {
public:
    typedef std::vector<std::shared_ptr<BsonElement>>      ValueType;
private:
    ValueType _mapping;
    std::string _jsCode;
public:
    BsonElementCodeWS(std::string n, std::string jsCode, ValueType mapping) : BsonElement(n),  _mapping(mapping), _jsCode(jsCode) {}
    
    /*!
     * \brief Mapping from identifiers to values, 
     *        representing the scope in which the string should be evaluated.
     * \return Returns a pointer to a BsonElement for the given identifier or a nullptr if doesn't exist.
     */
    std::shared_ptr<BsonElement> operator[](std::string name) const {
        auto it = std::find_if(_mapping.cbegin(), _mapping.cend(), [name](auto sp){ return sp->name() == name; });
        return it != _mapping.cend() ? *it : std::shared_ptr<BsonElement>();
    }
    
    /*!
     * \brief Gets the JavaScript code value.
     * \return Returns the JavaScript code value.
     */
    std::string jsCode() const {
        return _jsCode;
    }
    
    /*!
     * \brief Gets the mapping value.
     * \return Returns the mapping value.
     */
    const ValueType& mapping() const {
        return _mapping;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::JavascriptCodeWS;
    }
};

/*!
 * \class BsonElementInt32
 * \brief Represents the BSON 32-bit integer type.
 */
class BsonElementInt32 : public BsonElement {
public:
    typedef Bson::Int32         ValueType;
private:
    ValueType _value;
public:
    
    BsonElementInt32(std::string n, ValueType integer) : BsonElement(n), _value(integer) {}
    
    /*!
     * \brief Gets the 32-bit integer value.
     * \return Returns the 32-bit integer value.
     */
    ValueType int32() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Int32;
    }
};

/*!
 * \class BsonElementTimestamp
 * \brief Represents the BSON 64-bit integer type.
 */
class BsonElementTimestamp : public BsonElement {
public:
    typedef Bson::Int32         ValueType;
private:
    ValueType _increment, _timestamp;
public:
    BsonElementTimestamp(std::string n, ValueType increment, ValueType timestamp) : BsonElement(n), _increment(increment), _timestamp(timestamp) {}
    
    /*!
     * \brief Gets the increment value.
     * \return Returns the increment value.
     */
    ValueType increment() const {
        return _increment;
    }
    
    /*!
     * \brief Gets the timestamp value.
     * \return Returns the timestamp value.
     */
    ValueType timestamp() const {
        return _timestamp;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Timestamp;
    }
};

/*!
 * \class BsonElementInt64
 * \brief Represents the BSON 64-bit integer type.
 */
class BsonElementInt64 : public BsonElement {
public:
    typedef Bson::Int64         ValueType;
private:
    ValueType _value;
public:
    BsonElementInt64(std::string n, ValueType integer) : BsonElement(n), _value(integer) {}
    
    /*!
     * \brief Gets the 64-bit integer value.
     * \return Returns the 64-bit integer value.
     */
    ValueType int64() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Int64;
    }
};

/*!
 * \class BsonElementDecimal128
 * \brief Represents the BSON 128-bit decimal floating point type.
 */
class BsonElementDecimal128 : public BsonElement {
public:
    typedef Bson::Decimal128         ValueType;
private:
    ValueType _value;
public:
    BsonElementDecimal128(std::string n, ValueType d) : BsonElement(n), _value(d) {}
    
    /*!
     * \brief Gets the 128-bit decimal floating point value.
     * \return Returns the 128-bit decimal floating point value.
     */
    ValueType decimal128() const {
        return _value;
    }
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::Decimal128;
    }
};

/*!
 * \class BsonElementMinKey
 * \brief Represents the BSON Min Key type.
 *        Special type which compares lower than all other possible BSON element values.
 */
class BsonElementMinKey : public BsonElement {
public:
    BsonElementMinKey(std::string n) : BsonElement(n) {}
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::MinKey;
    }
};

/*!
 * \class BsonElementMaxKey
 * \brief Represents the BSON Max Key type.
 *        Special type which compares higher than all other possible BSON element values.
 */
class BsonElementMaxKey : public BsonElement {
public:
    BsonElementMaxKey(std::string n) : BsonElement(n) {}
    
    Bson::ElementType id() const override
    {
        return Bson::ElementType::MaxKey;
    }
};

#endif // _BSON_ELEMENT_H_
