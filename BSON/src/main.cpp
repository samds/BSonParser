#include <iostream>
#include <exception>
#include "BsonFileParser.hpp"
#include "BsonDocument.hpp"
#include "BsonDocumentOutputter.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Hello, world!" << std::endl;
    if (argc != 2) {
        std::cerr << "BSON: illegal option" << std::endl;
        std::cerr << "usage: BSON [file]" << std::endl;
        return EXIT_FAILURE;
    }
    std::string file(argv[1]);
    
    try {
        BsonFileParser parser;
        BsonDocument doc = parser.parseFile(file);
        
        BsonDefaultOutputter formatter(doc);
        std::cout << formatter << std::endl;
    } catch (std::exception& err) {
        std::cerr << "Failed to parse Bson file at " << file << " with reason : " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
