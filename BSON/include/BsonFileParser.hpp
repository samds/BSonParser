
#pragma once
#ifndef _BSON_FILE_PARSER_H_
#define _BSON_FILE_PARSER_H_

#include "BsonStreamParser.hpp"

class BsonDocument;
class string;
/*!
 * \class BsonFileParser
 * \brief Helper class to parse a bson file.
 */
class BsonFileParser {
    BsonStreamParser _streamParser;
public:
    BsonFileParser(BsonStreamParser streamParser = BsonStreamParser()) : _streamParser(streamParser) {}
    
    /*!
     * \brief Parses the Bson file with the given name.
     * \param fileName : Bson's file name.
     * \return A Bson document.
     */
    BsonDocument parseFile(std::string fileName);
};

#endif // _BSON_FILE_PARSER_H_
