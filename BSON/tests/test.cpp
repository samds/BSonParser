#include <iostream>
#include <cassert>
#include <sstream>
#include <cmath>

#include "BsonElement.hpp"
#include "BsonStreamParser.hpp"
#include "BsonFileParser.hpp"
#include "BsonDocumentOutputter.hpp"

int main()
{
    // Double
    /*{
        std::vector<char> buffer = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x14, 0x40};
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        BsonElementDouble elem("key", ss);
        assert(elem.name() == "key");
        assert(elem.Double() == 5.05);
    }*/
    
    
    //String
    {
        std::vector<char> buffer = {0x06, 0x00, 0x00, 0x00, 'w', 'o', 'r', 'l', 'd', 0x00};
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        BsonElementUTF8String elem("hello", ss);
        assert(elem.name() == "hello");
        assert(elem.value == "world");
    }

    
    // ObjectId
    {
        std::vector<unsigned char> buffer = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        BsonElementObjectId elem("BSON", ss);
        assert(elem.name() == "BSON");
        assert(elem.id() == Bson::ElementType::ObjectId);
        auto expected = std::vector<Bson::byte>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
        assert( std::equal(elem.objectId.cbegin(), elem.objectId.cend(), expected.cbegin(), expected.cend()));
    }
    /*
    // BsonElementDatetime
    {
        std::vector<unsigned char> buffer = {0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        BsonElementDatetime elem("BSON", ss);
        assert(elem.id() == Bson::ElementType::Datetime);
        assert(elem.name() == "BSON");
        std::time_t end_time = std::chrono::system_clock::to_time_t(elem.datetime);
        std::cout << "Time: " << std::asctime(std::gmtime(&end_time)) << std::endl;

        //std::chrono::duration<Bson::int64, std::milli> d(1);
        //auto expected = std::chrono::time_point<std::chrono::high_resolution_clock>(d);
        //assert(elem.time == expected);
    }*/
    
    // Int32
    /*{
        std::vector<unsigned char> buffer = {
            0xc2, 0x07, 0x00, 0x00};
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        BsonElementInt32 elem("int", ss);
        assert(elem.name() == "int");
        assert(elem.value == 1986);
    }*/
    
    {
        std::vector<unsigned char> buffer = {
            0x31, 0x00, 0x00, 0x00, 0x04, 'B', 'S', 'O', 'N', 0x00,
            0x26, 0x00, 0x00, 0x00, // size
            0x02, 0x30, 0x00, 0x08, 0x00, 0x00, 0x00, 'a', 'w', 'e', 's', 'o', 'm', 'e', 0x00, // string 'awesome'
            0x01, 0x31, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x14, 0x40, // double 5.05
            0x10, 0x32, 0x00, 0xc2, 0x07, 0x00, 0x00, // int32 1986
            0x00, 0x00}; // eod
        std::stringstream ss;
        std::copy(buffer.cbegin(), buffer.cend(),std::ostream_iterator<char>(ss));
        /*
        BsonStreamParser parser;
        BsonDocument doc = parser.parseBinaryStream(ss);
        BsonDocumentOutputter outputter;
        outputter.print(doc);
         */
    }
    
    // BsonOutputter
    {
        BsonDefaultOutputter outputter(" : ");
        {
            BsonElementUTF8String elem("key", "value");
            auto str = outputter.toString(&elem);
            auto expected = "'key' : \"value\"";
            assert(str == expected);
        }
        {
            BsonElementDouble elem("key", 3.14);
            auto str = outputter.toString(&elem);
            auto expected = "'key' : 3.14";
            assert(str == expected);
        }
        {
            std::shared_ptr<BsonElement> elem1(std::make_shared<BsonElementUTF8String>("txt", "hello"));
            std::shared_ptr<BsonElement> elem2(std::make_shared<BsonElementDouble>("pi", 3.14));
            BsonElementEmbeddedDocument arr("key", {elem1, elem2});
            auto str = outputter.toString(&arr);
            std::cout << "==>" << str << std::endl;
            //auto expected = "'key' : {\n\t'txt' : \"hello\",\n\t'pi' : 3.14\n}";
            auto expected = "'key' : {'txt' : \"hello\", 'pi' : 3.14}";
            assert(str == expected);
        }
        {
            std::shared_ptr<BsonElement> elem1(std::make_shared<BsonElementUTF8String>("0", "hello"));
            std::shared_ptr<BsonElement> elem2(std::make_shared<BsonElementDouble>("1", 3.14));
            BsonElementArray arr("key", {elem1, elem2});
            auto str = outputter.toString(&arr);
            std::cout << "==>" << str << std::endl;
            auto expected = "'key' : [\"hello\", 3.14]";
            assert(str == expected);
        }
        {
            auto v = std::vector<Bson::byte>{{0x01, 0x02, 0x03, 0x04}};
            BsonElementBinary elem("key", Bson::BinarySubType::Generic, v);
            auto str = outputter.toString(&elem);
            auto expected = "'key' : 0x01020304";
            assert(str == expected);
        }
        {
            BsonElementUndefined elem("key");
            auto str = outputter.toString(&elem);
            auto expected = "'key'";
            assert(str == expected);
        }
        {
            BsonElementObjectId elem("key", {{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C}});
            auto str = outputter.toString(&elem);
            auto expected = "'key' : 0102030405060708090a0b0c";
            assert(str == expected);
        }
        {
            {
                BsonElementBoolean elem("key", true);
                auto str = outputter.toString(&elem);
                auto expected = "'key' : true";
                assert(str == expected);
            }
            {
                BsonElementBoolean elem("key", false);
                auto str = outputter.toString(&elem);
                auto expected = "'key' : false";
                assert(str == expected);
            }
        }
        {
            // std::put_time(std::gmtime(&t), "%c %Z")
            
            // create tm with 1/1/2000:
            std::tm timeinfo = std::tm();
            timeinfo.tm_year = 100;   // year: 2000 (since 1900)
            timeinfo.tm_mon = 0;      // month: january
            timeinfo.tm_mday = 1;     // day: 1st
            timeinfo.tm_hour = 7;
            timeinfo.tm_min = 34;
            timeinfo.tm_sec = 56;
            std::time_t tt = std::mktime (&timeinfo);
            auto datetime = std::chrono::system_clock::from_time_t(tt);
            //auto epochTime = std::chrono::system_clock::
            
            BsonElementDatetime elem("key", std::chrono::duration_cast<BsonElementDatetime::ValueType>(datetime.time_since_epoch()));
            auto str = outputter.toString(&elem);
            //std::cout << "==>" << str << std::endl;
            auto expected = "'key' : Sat Jan  1 12:34:56 2000 UTC";
            assert(str == expected);
        }
        {
            BsonElementRegex elem("key", "pattern", "options");
            auto str = outputter.toString(&elem);
            std::cout << "==>" << str << std::endl;
            auto expected = "'key' : r\"pattern\" r\"options\"";
            assert(str == expected);
        }
        {
            BsonElementNull elem("key");
            auto str = outputter.toString(&elem);
            auto expected = "'key'";
            assert(str == expected);
        }
        {
            BsonElementDBPointer elem("key", "string", {{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C}});
            auto str = outputter.toString(&elem);
            auto expected = "'key' : \"string\" 0102030405060708090a0b0c";
            assert(str == expected);
        }
        {
            BsonElementJavascriptCode elem("key", u8"value€");
            auto str = outputter.toString(&elem);
            auto expected = u8"'key' : \"value€\"";
            assert(str == expected);
        }
        {
            BsonElementSymbol elem("key", "value$");
            auto str = outputter.toString(&elem);
            auto expected = "'key' : \"value$\"";
            assert(str == expected);
        }
        {
            BsonElementInt32 elem("key", 42);
            auto str = outputter.toString(&elem);
            auto expected = "'key' : 42";
            assert(str == expected);
        }
        {
            BsonElementInt64 elem("key", 0x8000000000000000);
            auto str = outputter.toString(&elem);
            auto expected = "'key' : -9223372036854775808";
            assert(str == expected);
        }
        {
            BsonElementDecimal128 elem("key", std::acos(-1.L));
            auto str = outputter.toString(&elem);
            auto expected = "'key' : 3.141592653589793239";
            assert(str == expected);
        }
        {
            BsonElementMinKey elem("key");
            auto str = outputter.toString(&elem);
            auto expected = "'key'";
            assert(str == expected);
        }
        {
            BsonElementMaxKey elem("key");
            auto str = outputter.toString(&elem);
            auto expected = "'key'";
            assert(str == expected);
        }
    }

    
    /*
    char buf[] = {0x16, 0x00, 0x00, 0x00, 0x02, 'h', 'e', 'l', 'l', 'o', 0x00, 0x06, 0x00, 0x00, 0x00, 'w', 'o', 'r', 'l', 'd', 0x00, 0x00};
    std::stringstream is;
    for (char c: buf) {
        is << c;
    }*/
    /*
    std::cout << "==\n";
    BsonFileParser parser;
    BsonDocument doc = parser.parseFile("/Users/Marie/Documents/Samuel/softbank/test2/bar/wb.bson");
    
    BsonDocumentOutputter outputter;
    outputter.print(doc);
    std::cout << "==\n";
    */
  return 0;
}
