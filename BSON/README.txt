BSON
=======

BSON is a Bson file parser using the version 1.1 (http://bsonspec.org/spec.html).
The executable takes a path as parameter and dump the result to the standard output.

Compilation
===========

To build the project you need qibuild (which use CMake), a C++ 14 compiler with the STL, Doxygen to generate the documentation. GTest is provided with the project, so don’t hesitate to run the tests.

cd workspace/BSON
qibuild configure
qibuild make
qitest run
qidoc build

Example
=======

The following example shows some features of the framework, please refer to the documentation for further details.

#include <iostream>
#include <exception>
#include "BsonFileParser.hpp"
#include "BsonDocument.hpp"
#include "BsonDocumentOutputter.hpp"

int main(int argc, char* argv[])
{
  std::string file = "example.bson";

  BsonFileParser parser;
  BsonDocument doc = parser.parseFile(file); // parse the bson file
  
  auto elem = doc["_id"];
  if (elem) { // true if "_id" present in doc 
  	// cast to appropriate type
	// check elem->id() value
  }

  BsonDefaultOutputter formatter(doc);
  std::cout << formatter << std::endl; // dump bson file with default output format
}