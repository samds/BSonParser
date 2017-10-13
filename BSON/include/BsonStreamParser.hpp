
#pragma once
#ifndef _BSON_STREAM_PARSER_H_
#define _BSON_STREAM_PARSER_H_

#include <istream>
#include <vector>

class BsonDocument;

/*!
 * \class BsonStreamParser
 * \brief Helper class to parse a binary stream from a Bson file.
 */
class BsonStreamParser {
public:
    /*!
     * \brief Parse a binary stream
     * \param istream : a Bson file as a bytes stream.
     * \return A Bson document. 
     *         Undefined behavior if the stream was not open in binary mode.
     */
    BsonDocument parseBinaryStream(std::istream& istream) const;
    
    /*!
     * \brief Parse a bytes array.
     * \param bytes : a Bson file as an array of bytes.
     * \return A Bson document.
     */
    BsonDocument parseBinaryArray(const std::vector<char>& bytes) const;
};

#endif // _BSON_STREAM_PARSER_H_
