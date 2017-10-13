
#pragma once
#ifndef _BSON_DOCUMENT_OUTPUTTER_H_
#define _BSON_DOCUMENT_OUTPUTTER_H_

#include <string>
#include <vector>
#include "BsonDocument.hpp"
#include "BsonElement.hpp"

/*!
 * \class BsonOutputter
 * \brief Base class for Bson outputter.
 *        A subclass shall implement all required methods in
 *        order to dump properly a Bson document.
 */
class BsonOutputter {
    std::string _separator;
    const BsonDocument& _document;
public:
    
    /*!
     * \brief Default constructor.
     * \param doc : The Bson document to print. The class instance holds only a
     *              reference to the document and therefore, the ressource should not
     *              be freed while using the outputter.
     * \param del : The characters to print between the name and the value
     *              of a Bson element.
     */
    BsonOutputter(const BsonDocument& doc, std::string del) : _separator(del), _document(doc) {}
    
    virtual std::string toString(const BsonElement& elem) const = 0;
    virtual std::string elementName(const BsonElement& elem) const = 0;

    
    virtual std::string toString(const BsonElementDouble& elem) const = 0;
    virtual std::string toString(const BsonElementString& elem) const = 0;
    virtual std::string toString(const BsonElementEmbeddedDocument& elem) const = 0;
    virtual std::string toString(const BsonElementArray& elem) const = 0;
    virtual std::string toString(const BsonElementBinary& elem) const = 0;
    virtual std::string toString(const BsonElementUndefined& elem) const = 0;
    virtual std::string toString(const BsonElementObjectId&) const = 0;
    virtual std::string toString(const BsonElementBoolean&) const = 0;
    virtual std::string toString(const BsonElementDatetime&) const = 0;
    virtual std::string toString(const BsonElementNull&) const = 0;
    virtual std::string toString(const BsonElementRegex&) const = 0;
    virtual std::string toString(const BsonElementDBPointer&) const = 0;
    virtual std::string toString(const BsonElementJavascriptCode&) const = 0;
    virtual std::string toString(const BsonElementSymbol&) const = 0;
    virtual std::string toString(const BsonElementCodeWS&) const = 0;
    virtual std::string toString(const BsonElementInt32&) const = 0;
    virtual std::string toString(const BsonElementTimestamp&) const = 0;
    virtual std::string toString(const BsonElementInt64&) const = 0;
    virtual std::string toString(const BsonElementDecimal128&) const = 0;
    virtual std::string toString(const BsonElementMinKey&) const = 0;
    virtual std::string toString(const BsonElementMaxKey&) const = 0;
    
    std::string separator() const {
        return _separator;
    }
    
    const BsonDocument& document() const {
        return _document;
    }
};

/*!
 * \class BsonDefaultOutputter
 * \brief Generic Bson outputter.
 * \remark Print a Bson document with format\n
 *        {\n
 *            'name1': "string"\n
 *            'name2': 2.4\n
 *            'name3': [2.4, "example"]\n
 *             ...\n
 *        }\n
 */
class BsonDefaultOutputter : public BsonOutputter {
public:
    BsonDefaultOutputter(const BsonDocument& doc, std::string sep = ": ") : BsonOutputter(doc, sep) {}
    
    std::string elementName(const BsonElement& elem) const {
        return "\'" + elem.name() + "\'";
    }
    
    std::string toString(const BsonElementDouble& elem) const {
        std::ostringstream os;
        os << elem.decimal64();
        return os.str();
    }
    
    std::string toString(const BsonElementString& elem) const {
        std::ostringstream os;
        os << std::quoted(elem.text());
        return os.str();
    }
    
    std::string toString(const BsonElementBinary& elem) const {
        std::ostringstream os("0x", std::ios_base::ate);
        for(Bson::Byte b : elem.bytes()) {
            os << std::hex << std::setw(2) << std::setfill('0') << +b;
        }
        return os.str();
    }
    
    std::string toString(const BsonElementUndefined& elem) const {
        return "";
    }
    
    std::string toString(const BsonElementRegex& elem) const {
        std::ostringstream os(std::ios_base::ate);
        os << 'r' << std::quoted(elem.regexPattern()) << " " << 'r' << std::quoted(elem.regexOptions());
        return os.str();
    }
    
    std::string toString(const BsonElementObjectId& elem) const {
        std::ostringstream os(std::ios_base::ate);
        for(Bson::Byte b : elem.objectId) {
            os << std::hex << std::setw(2) << std::setfill('0') << +b;
        }
        return os.str();
    }
    
    std::string toString(const BsonElementBoolean& elem) const {
        std::ostringstream os(std::ios_base::ate);
        os << std::boolalpha << elem.value();
        return os.str();
    }
    
    std::string toString(const BsonElementDatetime& elem) const {
        std::time_t end_time = std::chrono::system_clock::to_time_t(elem.timePoint());
        std::ostringstream os;
        os << std::put_time(std::gmtime(&end_time), "%c %Z");
        return os.str();
    }
    
    std::string toString(const BsonElementNull& elem) const {
        return "";
    }
    
    std::string toString(const BsonElementDBPointer& elem) const {
        std::ostringstream os(std::ios_base::ate);
        os << std::quoted(elem.desc()) << " ";
        auto dbpointer = elem.dBPointer();
        for(Bson::Byte b : dbpointer) {
            os << std::hex << std::setw(2) << std::setfill('0') << +b;
        }
        return os.str();
    }
    
    std::string toString(const BsonElementJavascriptCode& elem) const {
        std::ostringstream os;
        os << std::quoted(elem.code());
        return os.str();
    }
    
    std::string toString(const BsonElementSymbol& elem) const {
        std::ostringstream os;
        os << std::quoted(elem.symbol());
        return os.str();
    }
    
    std::string toString(const BsonElementCodeWS& arr) const
    {
        const auto& elements = arr.mapping();
        auto size = elements.size();
        
        std::string prefix = "javascript_code_ws(" + arr.jsCode() + ")";
        
        if (size == 0) {
            return prefix +"{}";
        }
        else if(size == 1) {
            return prefix + "{ " + toString(*elements[0]) + " }";
        }
        else {
            std::string s = prefix + "{";
            for(auto i = 0; i < size - 1; ++i) {
                s += toString(*elements[i]) + ", ";
            }
            s += toString(*elements.back()) + "}";
            return s;
        }
    }
    
    std::string toString(const BsonElementInt32& elem) const
    {
        std::ostringstream os;
        os << elem.int32();
        return os.str();
    }
    
    std::string toString(const BsonElementTimestamp& elem) const
    {
        std::ostringstream os;
        os << elem.increment() << elem.timestamp();
        return os.str();
    }
    
    std::string toString(const BsonElementInt64& elem) const
    {
        std::ostringstream os;
        os << elem.int64();
        return os.str();
    }
    
    std::string toString(const BsonElementDecimal128& elem) const
    {
        std::ostringstream os;
        os << std::setprecision(std::numeric_limits<BsonElementDecimal128::ValueType>::digits10 + 1) << elem.decimal128();
        return os.str();
    }
    
    std::string toString(const BsonElementMinKey& elem) const
    {
        return "";
    }
    
    std::string toString(const BsonElementMaxKey& elem) const
    {
        return "";
    }
    
    std::string toString(const BsonElementArray& arr) const
    {
        const auto& elements = arr.Array();
        auto size = elements.size();
        
        if (size == 0) {
            return "[]";
        }
        else if(size == 1) {
            return "[" + toString(*elements[0]) + "]";
        }
        else {
            std::string s = "[";
            for(auto i = 0; i < size - 1; ++i) {
                s += toString(*elements[i]) + ", ";
            }
            s += toString(*elements.back()) + "]";
            return s;
        }
    }
    
    std::string toString(const BsonElementEmbeddedDocument& arr) const
    {
        const auto& elements = arr.EmbeddedDocument();
        auto size = elements.size();
        
        if (size == 0) {
            return "{}";
        }
        else if(size == 1) {
            return "{ " + toString(*elements[0]) + " }";
        }
        else {
            std::string s = "{";
            for(auto i = 0; i < size - 1; ++i) {
                s += toString(*elements[i]) + ", ";
            }
            s += toString(*elements.back()) + "}";
            return s;
        }
    }

    virtual std::string toString(const BsonElement& elem) const {
        
        std::string vec{};
        vec.append(elementName(elem));
        vec.append(separator());
        
        switch (elem.id()) {
            case Bson::ElementType::Double :
                vec.append(toString(dynamic_cast<const BsonElementDouble&>(elem)));
                break;
            case Bson::ElementType::String :
                vec.append(toString(dynamic_cast<const BsonElementString&>(elem)));
                break;
            case Bson::ElementType::EmbeddedDocument :
                vec.append(toString(dynamic_cast<const BsonElementEmbeddedDocument&>(elem)));
                break;
            case Bson::ElementType::Array :
                vec.append(toString(dynamic_cast<const BsonElementArray&>(elem)));
                break;
            case Bson::ElementType::Binary :
                vec.append(toString(dynamic_cast<const BsonElementBinary&>(elem)));
                break;
            case Bson::ElementType::Undefined :
                vec.append(toString(dynamic_cast<const BsonElementUndefined&>(elem)));
                break;
            case Bson::ElementType::Regex :
                vec.append(toString(dynamic_cast<const BsonElementRegex&>(elem)));
                break;
            case Bson::ElementType::ObjectId :
                vec.append(toString(dynamic_cast<const BsonElementObjectId&>(elem)));
                break;
            case Bson::ElementType::Boolean :
                vec.append(toString(dynamic_cast<const BsonElementBoolean&>(elem)));
                break;
            case Bson::ElementType::Datetime :
                vec.append(toString(dynamic_cast<const BsonElementDatetime&>(elem)));
                break;
            case Bson::ElementType::Null :
                vec.append(toString(dynamic_cast<const BsonElementUndefined&>(elem)));
                break;
            case Bson::ElementType::DBPointer :
                vec.append(toString(dynamic_cast<const BsonElementDBPointer&>(elem)));
                break;
            case Bson::ElementType::JavascriptCode :
                vec.append(toString(dynamic_cast<const BsonElementJavascriptCode&>(elem)));
                break;
            case Bson::ElementType::Symbol :
                vec.append(toString(dynamic_cast<const BsonElementSymbol&>(elem)));
                break;
            case Bson::ElementType::JavascriptCodeWS :
                vec.append(toString(dynamic_cast<const BsonElementSymbol&>(elem)));
                break;
            case Bson::ElementType::Int32 :
                vec.append(toString(dynamic_cast<const BsonElementInt32&>(elem)));
                break;
            case Bson::ElementType::Timestamp :
                vec.append(toString(dynamic_cast<const BsonElementTimestamp&>(elem)));
                break;
            case Bson::ElementType::Int64 :
                vec.append(toString(dynamic_cast<const BsonElementInt64&>(elem)));
                break;
            case Bson::ElementType::Decimal128 :
                vec.append(toString(dynamic_cast<const BsonElementDecimal128&>(elem)));
                break;
            case Bson::ElementType::MinKey :
                vec.append(toString(dynamic_cast<const BsonElementMinKey&>(elem)));
                break;
            case Bson::ElementType::MaxKey :
                vec.append(toString(dynamic_cast<const BsonElementMaxKey&>(elem)));
                break;
            default:
                vec.append("N/A");
                break;
        }
        return vec;
    }
};


std::ostream& operator<<(std::ostream& os, const BsonOutputter& formatter)
{
    os << "{" << std::endl;
    const auto& doc = formatter.document();
    for(const auto& it: doc) {
        os << '\t' << formatter.toString(*it) << '\n';
    }
    os << "}";
    return os;
}

#endif // _BSON_DOCUMENT_OUTPUTTER_H_
