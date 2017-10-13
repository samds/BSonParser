#include <exception>
#include <iterator>
#include <sstream>

#include "BsonDocument.hpp"
#include "BsonElementFactory.hpp"
#include "BsonStreamParser.hpp"

BsonDocument BsonStreamParser::parseBinaryStream(std::istream& istream) const
{
    BsonExtractInfo<BsonDocumentInfo> infoDoc = Bson::extractDocument(istream);
    auto sizeDocument = infoDoc.value.size;
    auto nbBytesExtracted = infoDoc.count;
    
    if (sizeDocument != nbBytesExtracted) {
        throw std::logic_error("Bson document's size incorrect.");
    }
    
    return BsonDocument{ infoDoc.value.elements };
}

BsonDocument BsonStreamParser::parseBinaryArray(const std::vector<char>& bytes) const
{
    std::stringstream ss;
    std::copy(bytes.cbegin(), bytes.cend(), std::ostream_iterator<char>(ss));
    return parseBinaryStream(ss);
}
