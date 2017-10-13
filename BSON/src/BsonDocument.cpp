
#include "BsonDocument.hpp"
#include <vector>
#include "BsonElement.hpp"

BsonDocument::ValueType BsonDocument::operator[](std::string name) const {
    auto it = std::find_if(_elements.cbegin(), _elements.cend(), [name](BsonDocument::ValueType elem){ return elem->name() == name; });
    return it != _elements.cend() ? *it : ValueType{};
}

BsonDocument::Iterator BsonDocument::begin() {
    return _elements.begin();
}

BsonDocument::ConstIterator BsonDocument::begin() const {
    return _elements.begin();
}

BsonDocument::ConstIterator BsonDocument::cbegin() const {
    return _elements.cbegin();
}

BsonDocument::Iterator BsonDocument::end() {
    return _elements.end();
}

BsonDocument::ConstIterator BsonDocument::end() const {
    return _elements.end();
}

BsonDocument::ConstIterator BsonDocument::cend() const {
    return _elements.cend();
}
