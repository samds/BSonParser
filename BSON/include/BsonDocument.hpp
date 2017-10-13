
#pragma once
#ifndef _BSON_DOCUMENT_H_
#define _BSON_DOCUMENT_H_

#include <vector>
#include <memory>

class BsonElement;

/*!
 * \class BsonDocument
 * \brief Represents a Bson file.
 */
class BsonDocument {
public:
    typedef std::shared_ptr<BsonElement>                    ValueType;
    typedef std::vector<ValueType>                          ContainerType;
    typedef ContainerType::iterator                         Iterator;
    typedef ContainerType::const_iterator                   ConstIterator;
private:
    ContainerType _elements;

public:
    BsonDocument(const ContainerType& elements) : _elements(elements) {};
    BsonDocument(ContainerType&& elements) : _elements(std::move(elements)) {};
    
    /*!
     * \brief Access element with given name.
     * \param name : Element's name.
     * \return A shared pointer to the element with the given name 
     *         or an empty shared pointer if the name doesn't exist.
     */
    ValueType operator[](std::string name) const;
    
    Iterator begin();
    ConstIterator begin() const;
    ConstIterator cbegin() const;
    Iterator end();
    ConstIterator end() const;
    ConstIterator cend() const;
};

#endif // _BSON_DOCUMENT_H_
