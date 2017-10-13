#include <iostream>
#include <vector>
#include <exception>
#include <sstream>
#include <iomanip>
#include <array>
#include <sstream>

#include "BsonTypes.hpp"
#include "BsonElementTypes.hpp"
#include "BsonElement.hpp"
#include "BsonDocument.hpp"

namespace Bson {

    BsonExtractInfo<Bson::Int64> extractInt64(std::istream& is)
    {
        Bson::Int64 size;
        constexpr int bufferSize = sizeof(Bson::Int64);
        char data[bufferSize];
        is.read(data, bufferSize);
        if (!is || (is.gcount() != bufferSize)) {
            throw std::logic_error("Could'n extract Int64 from stream.");
        }
        
        BsonExtractInfo<Bson::Int64> info{};
        info.count = bufferSize;
        info.value = Bson::ByteOrder::int64_little_to_host( *reinterpret_cast<Bson::Int64*>(data) );
        return info;
    }
    
    BsonExtractInfo<Bson::UInt64> extractUInt64(std::istream& is)
    {
        Bson::UInt64 size;
        constexpr int bufferSize = sizeof(Bson::UInt64);
        char data[bufferSize];
        is.read(data, bufferSize);
        if (!is || (is.gcount() != bufferSize)) {
            throw std::logic_error("Could'n extract UInt64 from stream.");
        }
        
        BsonExtractInfo<Bson::UInt64> info{};
        info.count = bufferSize;
        info.value = Bson::ByteOrder::uint64_little_to_host( *reinterpret_cast<Bson::UInt64*>(data) );
        return info;
    }
    
    BsonExtractInfo<Bson::BinarySubType> extractBinarySubType(std::istream& is)
    {
        char data;
        is.read(&data, sizeof(data));
        if (!is || is.gcount()!=1) {
            throw std::logic_error("");
        }
        
        Bson::BinarySubType subType = static_cast<Bson::BinarySubType>( static_cast<Bson::Byte>(data) );

        BsonExtractInfo<Bson::BinarySubType> info{};
        info.count = 1;
        info.value = subType;
        return info;
    }
    
    BsonExtractInfo<std::vector<Bson::Byte>> extractBytes(std::istream& is, size_t n)
    {
        BsonExtractInfo<std::vector<Bson::Byte>> info{};
        size_t cnt = 0;
   
        std::vector<Bson::Byte> bytes;
        {
            std::vector<char> data(n);
            is.read(data.data(), n);
            
            if (!is || is.gcount()!=n) {
                throw std::logic_error("Could't extract " + std::to_string(n) + " bytes from stream.");
            }
            
            cnt += n;
            std::transform(data.cbegin(), data.cend(), std::back_inserter(bytes), [](char c){ return static_cast<Bson::Byte>(c); });
        }
        
        info.count = cnt;
        info.value = bytes;
        return info;
    }
    
    BsonExtractInfo<BsonDocumentInfo> extractDocument(std::istream& is)
    {
        BsonExtractInfo<BsonDocumentInfo> info{};
        
        Bson::Int32 size;
        {
            BsonExtractInfo<Bson::Int32> integerInfo = Bson::extractInt32(is);
            size = integerInfo.value;
            info.count = integerInfo.count;
        }
        
        std::vector<std::shared_ptr<BsonElement>> vec;
        
        while (true) {
            auto tmpInfo = BsonElementFactory::extractElement(is);
            info.count += tmpInfo.count;
            if (tmpInfo.value->id() == Bson::ElementType::EndOfDocument)
                break;
            vec.emplace_back(tmpInfo.value);
        }
        
        if (size != info.count)
            throw std::logic_error("Size document different from number of bytes extracted from stream.");
        
        info.value.size = size;
        info.value.elements = vec;
        return info;
    }
    
    template<class T, size_t S>
    BsonExtractInfo<std::array<T, S>> extractBytes(std::istream& is)
    {
        std::array<T, S> data;
        if( !is.read(reinterpret_cast<char*>(data.data()), S) )
            throw std::runtime_error("Cannot extract bytes value from stream");
        
        BsonExtractInfo<std::array<T, S>> res{};
        res.value = data;
        res.count = data.size();
        return res;
    }
    
    BsonExtractInfo<Bson::Double> extractDouble(std::istream& is)
    {
        Bson::Double d;
        if( !is.read(reinterpret_cast<char*>(&d), sizeof(d)) )
            throw std::runtime_error("Cannot extract double value from stream");
        d = Bson::ByteOrder::double_little_to_host(d);
        
        BsonExtractInfo<Bson::Double> res{};
        res.value = d;
        res.count = sizeof(d);
        return res;
    }
    
    BsonExtractInfo<Bson::ElementType> extractElementType(std::istream& is)
    {
        char data;
        if(!is || !is.get(data)) {
            throw std::logic_error("Could't extract element type from stream");
        }
        
        auto value = static_cast<Bson::ElementType>(static_cast<unsigned char>(data));
        if( std::find(std::begin(Bson::types), std::end(Bson::types), value) == std::end(Bson::types) ) {
            std::ostringstream os;
            os << "Element type with value " << std::hex << std::showbase << static_cast<unsigned int>(data) << " not supported/Unknown";
            throw std::logic_error(os.str());
        }
        
        BsonExtractInfo<Bson::ElementType> info{};
        info.count = 1;
        info.value = value;
        return info;
    }
    
    BsonExtractInfo<Bson::Int32> extractInt32(std::istream& is)
    {
        Bson::Int32 size;
        constexpr int bufferSize = sizeof(Bson::Int32);
        char data[bufferSize];
        is.read(data, bufferSize);
        if (!is || (is.gcount() != bufferSize)) {
            throw std::logic_error("Could'n extract Int32 from stream.");
        }
        Bson::Int32 outValue = Bson::ByteOrder::int32_little_to_host( *reinterpret_cast<Bson::Int32*>(data) );
        
        BsonExtractInfo<Bson::Int32> info{};
        info.count = bufferSize;
        info.value = outValue;
        return info;
    }
    
    BsonExtractInfo<std::string> extractString(std::istream& is)
    {
        BsonExtractInfo<std::string> info{};
        BsonExtractInfo<Bson::Int32> int32Info = Bson::extractInt32(is);
        BsonExtractInfo<std::string> strInfo = Bson::extractCString(is);
        
        if (int32Info.value != strInfo.count) {
            throw std::logic_error("Number of bytes in String incorrect.");
        }
        
        info.count = int32Info.count + strInfo.count;
        info.value = strInfo.value;
        return info;
    }
    
    BsonExtractInfo<std::string> extractCString(std::istream& is)
    {
        BsonExtractInfo<std::string> info{};
        
        BsonExtractInfo<std::string>::SizeType cnt = 0;
        std::string buffer;
        char tmp = 0x00;
        while (is.get(tmp)) {
            cnt += 1;
            if (tmp!=0x00)
                buffer.push_back(tmp);
            else
                break;
        }
        
        if (!is) {
            throw std::logic_error("CString trailing '\x00' missing.");
        }
        
        info.count = cnt;
        info.value = buffer;
        return info;
    }
    
    BsonExtractInfo<Bson::Decimal128> extractDecimal128(std::istream& is)
    {
        constexpr int bufferSize = sizeof(Bson::Decimal128);
        char data[bufferSize];
        is.read(data, bufferSize);
        if (!is || (is.gcount() != bufferSize)) {
            throw std::logic_error("Could'n extract Decimal128 from stream.");
        }
        
        Bson::Decimal128 outValue = Bson::ByteOrder::long_double_little_to_host( *reinterpret_cast<Bson::Decimal128*>(data) );
        
        BsonExtractInfo<Bson::Decimal128> info{};
        info.count = bufferSize;
        info.value = outValue;
        return info;
    }
}

BsonExtractInfo<std::shared_ptr<BsonElement>> BsonElementFactory::extractElement(std::istream& is)
{
    std::size_t cnt = 0;
    
    auto typeInfo = Bson::extractElementType(is);
    auto typeElement = typeInfo.value;
    cnt += typeInfo.count;
    
    if (typeElement == Bson::ElementType::EndOfDocument) {
        BsonExtractInfo<std::shared_ptr<BsonElement>> info{};
        info.count = 1;
        info.value = std::make_shared<BsonElementEndOfDocument>();
        return info;
    }
    std::string name;

    auto cstrInfo = Bson::extractCString(is);
    cnt += cstrInfo.count;
    name = cstrInfo.value;

    // TODO really ugly
    // find another way
    BsonElement* elem = nullptr;
    BsonExtractInfo<Bson::Int32> integerInfo, integerInfo2;
    BsonExtractInfo<std::string> strInfo, strInfo2;
    BsonExtractInfo<Bson::Int64> int64Info;
    BsonExtractInfo<Bson::UInt64> uint64Info;
    BsonExtractInfo<Bson::Double> doubleInfo;
    BsonExtractInfo<Bson::Decimal128> decimal128Info;
    BsonExtractInfo<std::array<Bson::Byte, 12>> oidInfo;
    BsonExtractInfo<std::array<Bson::Byte, 1>> boolInfo;
    BsonExtractInfo<Bson::BinarySubType> bstInfo;
    BsonExtractInfo<std::vector<Bson::Byte>> bytesInfo;
    BsonExtractInfo<BsonDocumentInfo> dInfo;

    switch (typeElement) {
        case Bson::ElementType::Double:
            doubleInfo = Bson::extractDouble(is);
            cnt += doubleInfo.count;
            elem = new BsonElementDouble(name, doubleInfo.value);
            break;
        case Bson::ElementType::String:
            strInfo = Bson::extractString(is);
            cnt += strInfo.count;
            elem = new BsonElementString(name, strInfo.value);
            break;
        case Bson::ElementType::EmbeddedDocument:
            dInfo = Bson::extractDocument(is);
            cnt += dInfo.count;
            elem = new BsonElementEmbeddedDocument(name, dInfo.value.elements);
            break;
        case Bson::ElementType::Array:
            dInfo = Bson::extractDocument(is);
            cnt += dInfo.count;
            elem = new BsonElementArray(name, dInfo.value.elements);
            break;
        case Bson::ElementType::Binary: // element	::= "\x05" e_name binary, binary ::= int32 subtype (byte*), subtype ::= '\x**'
            integerInfo = Bson::extractInt32(is);
            cnt += integerInfo.count;
            bstInfo = Bson::extractBinarySubType(is);
            bytesInfo = Bson::extractBytes(is, integerInfo.value);
            cnt += bstInfo.count;
            cnt += bytesInfo.count;
            elem = new BsonElementBinary(name, bstInfo.value, bytesInfo.value);
            break;
        case Bson::ElementType::Undefined:
            elem = new BsonElementUndefined(name);
            break;
        case Bson::ElementType::ObjectId:
            oidInfo = Bson::extractBytes<Bson::Byte, 12>(is);
            cnt += oidInfo.count;
            elem = new BsonElementObjectId(name, oidInfo.value);
            break;
        case Bson::ElementType::Boolean:
            boolInfo = Bson::extractBytes<Bson::Byte, 1>(is);
            cnt += boolInfo.count;
            if (boolInfo.value[0] == 0 || boolInfo.value[0] == 1)
                elem = new BsonElementBoolean(name, boolInfo.value[0]);
            else
                throw std::logic_error("Could't extract bool value from stream. Expect value \x00 or \x01.");
            break;
        case Bson::ElementType::Datetime:
            int64Info = Bson::extractInt64(is);
            cnt += int64Info.count;
            elem = new BsonElementDatetime(name, BsonElementDatetime::ValueType{int64Info.value});
            break;
        case Bson::ElementType::Null:
            elem = new BsonElementNull(name);
            break;
        case Bson::ElementType::Regex:
            strInfo = Bson::extractCString(is);
            strInfo2 = Bson::extractCString(is);
            cnt += strInfo.count + strInfo2.count;
            elem = new BsonElementRegex(name, strInfo.value, strInfo2.value);
            break;
        case Bson::ElementType::DBPointer:
            strInfo = Bson::extractString(is);
            cnt += strInfo.count;
            oidInfo = Bson::extractBytes<Bson::Byte, 12>(is);
            cnt += oidInfo.count;
            elem = new BsonElementDBPointer(name, strInfo.value, oidInfo.value);
            break;
        case Bson::ElementType::JavascriptCode:
            strInfo = Bson::extractString(is);
            cnt += strInfo.count;
            elem = new BsonElementJavascriptCode(name, strInfo.value);
            break;
        case Bson::ElementType::Symbol:
            strInfo = Bson::extractString(is);
            cnt += strInfo.count;
            elem = new BsonElementSymbol(name, strInfo.value);
            break;
        case Bson::ElementType::JavascriptCodeWS:
            integerInfo = Bson::extractInt32(is);
            cnt += integerInfo.count;
            strInfo = Bson::extractString(is);
            cnt += strInfo.count;
            dInfo = Bson::extractDocument(is);
            cnt += dInfo.count;
            elem = new BsonElementCodeWS(name, strInfo.value, dInfo.value.elements);
            break;
        case Bson::ElementType::Int32:
            integerInfo = Bson::extractInt32(is);
            cnt += integerInfo.count;
            elem = new BsonElementInt32(name, integerInfo.value);
            break;
        case Bson::ElementType::Timestamp:
            integerInfo = Bson::extractInt32(is);
            integerInfo2 = Bson::extractInt32(is);
            cnt += integerInfo.count + integerInfo2.count;
            elem = new BsonElementTimestamp(name, integerInfo.value, integerInfo2.value);
            break;
        case Bson::ElementType::Int64:
            int64Info = Bson::extractInt64(is);
            cnt += int64Info.count;
            elem = new BsonElementInt64(name, int64Info.value);
            break;
        case Bson::ElementType::Decimal128:
            decimal128Info = Bson::extractDecimal128(is);
            cnt += decimal128Info.count;
            elem = new BsonElementDecimal128(name, decimal128Info.value);
            break;
        case Bson::ElementType::MinKey:
            elem = new BsonElementMinKey(name);
            break;
        case Bson::ElementType::MaxKey:
            elem = new BsonElementMaxKey(name);
            break;
        default:
            throw std::runtime_error("Element id not supported by the application.");
            break;
    }
    
    BsonExtractInfo<std::shared_ptr<BsonElement>> info{};
    info.value = std::shared_ptr<BsonElement>(elem);
    info.count = cnt;
    return info;
}
