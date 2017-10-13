#pragma once
#ifndef _BSON_ELEMENT_BASE_H_
#define _BSON_ELEMENT_BASE_H_

#include <string>
#include "BsonElementTypes.hpp"

/*!
 * \class BsonElement
 * \brief Base type for all Bson elements.
 */
class BsonElement {
protected:
    std::string _name;
    BsonElement() = default;
    BsonElement(std::string n) : _name(n) {}
public:
    
    /*!
     * \brief Element's name.
     * \return Element's name.
     */
    std::string name() const {
        return _name;
    }
    
    /*!
     * \brief Element's type id.
     * \return Element's type id.
     */
    virtual Bson::ElementType id() const = 0;
};

#endif // _BSON_ELEMENT_BASE_H_
