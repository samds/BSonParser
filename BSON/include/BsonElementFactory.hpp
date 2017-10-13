#pragma once
#ifndef _BSON_ELEMENT_FACTORY_H_
#define _BSON_ELEMENT_FACTORY_H_

#include <iostream>
#include <string>
#include <chrono>
#include "BsonTypes.hpp"
#include "BsonElementTypes.hpp"

class BsonElement;
class BsonDocument;

/*!
 * \struct BsonExtractInfo
 * \brief Helper class to store the result of an extraction from a stream.
 */
template<class T>
struct BsonExtractInfo {
    typedef std::size_t         SizeType;
    typedef T                   ValueType;
    ValueType value; // value extracted
    SizeType count; // number of bytes extracted
};

/*!
 * \struct BsonDocumentInfo
 * \brief Helper class used to store the elements and size of a Bson document.
 */
struct BsonDocumentInfo {
    typedef std::vector<std::shared_ptr<BsonElement>>       ValueType;
    
    Bson::Int32 size; // document's size in bytes
    ValueType elements; // Bson elements
};

// helper methods
namespace Bson {
    /*!
     * \brief Extracts a Bson document type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson document type.
     */
    BsonExtractInfo<BsonDocumentInfo> extractDocument(std::istream& is);
    
    /*!
     * \brief Extracts a Bson Int32 type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson Int32 type.
     */
    BsonExtractInfo<Bson::Int32> extractInt32(std::istream& is);
    
    /*!
     * \brief Extracts a Bson Int64 type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson Int64 type.
     */
    BsonExtractInfo<Bson::Int64> extractInt64(std::istream& is);
    
    /*!
     * \brief Extracts a Bson UInt64 type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson UInt64 type.
     */
    BsonExtractInfo<Bson::UInt64> extractUInt64(std::istream& is);
    
    /*!
     * \brief Extracts a Bson Double type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson Double type.
     */
    BsonExtractInfo<Bson::Double> extractDouble(std::istream& is);
    
    /*!
     * \brief Extracts a Bson Decimal128 type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson Decimal128 type.
     */
    BsonExtractInfo<Bson::Decimal128> extractDecimal128(std::istream& is);
    
    /*!
     * \brief Extracts Bson binary subtype from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson binary subtype.
     */
    BsonExtractInfo<Bson::BinarySubType> extractBinarySubType(std::istream& is);
    
    /*!
     * \brief Extracts an array of raw bytes from stream.
     * \param is : Input binary stream.
     * \param n : Number of bytes to extract
     * \return Returns number of bytes extracted and an array of raw bytes.
     */
    BsonExtractInfo<std::vector<Bson::Byte>> extractBytes(std::istream& is, size_t n);
    
    /*!
     * \brief Extracts the Bson element type from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a Bson element type.
     */
    BsonExtractInfo<Bson::ElementType> extractElementType(std::istream& is);
    
    /*!
     * \brief Extracts a string from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a string.
     */
    BsonExtractInfo<std::string> extractString(std::istream& is);
    
    /*!
     * \brief Extracts a cstring from stream.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and a string.
     */
    BsonExtractInfo<std::string> extractCString(std::istream& is);

    /*!
     * \brief Extracts an array of raw bytes from stream.
     *        The size should be specified at compile time.
     * \param is : Input binary stream.
     * \return Returns number of bytes extracted and an array of raw bytes.
     */
    template<class T, size_t S>
    BsonExtractInfo<std::array<T, S>> extractBytes(std::istream& is);
} // end namespace Bson

/*!
 * \class BsonElementFactory
 * \brief Helper class used to store the elements and size of a Bson document.
 */
class BsonElementFactory {
public:
    
    /*!
     * \brief Extracts a Bson element from stream.
     * \param is : a binary stream.
     * \return Returns a newly created Bson element.
     */
    static BsonExtractInfo<std::shared_ptr<BsonElement>> extractElement(std::istream& is);
};

#endif // _BSON_ELEMENT_FACTORY_H_
