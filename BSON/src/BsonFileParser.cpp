#include <iostream>
#include <fstream>
#include <ios>
#include <iomanip>
#include <string>

#include "BsonFileParser.hpp"
#include "BsonDocument.hpp"

BsonDocument BsonFileParser::parseFile(std::string fileName) {
    std::ifstream fstream(fileName, std::ios::binary);
    if (fstream) {
        return _streamParser.parseBinaryStream(fstream);
    }
    else {
        throw std::ios_base::failure("Couln'd open file \"" + fileName + "\".");
    }
}
