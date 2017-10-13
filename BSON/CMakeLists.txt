cmake_minimum_required(VERSION 2.8)
project(BSON)

find_package(qibuild)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(BSON_LIBRARY_SRCS
	src/BsonElementFactory.cpp
	src/BsonStreamParser.cpp
	src/BsonFileParser.cpp
	src/BsonDocument.cpp
)

set(BSON_LIBRARY_H
	include/BsonTypes.hpp
	include/BsonElementTypes
	include/BsonByteOrder.hpp
	include/BsonElementBase.hpp
	include/BsonElementFactory.hpp
	include/BsonElement.hpp
	include/BsonDocument.hpp
	include/BsonStreamParser.hpp
	include/BsonFileParser.hpp
	include/BsonDocumentOutputter.hpp
)

include_directories(src)
include_directories(include)

qi_create_lib(mybsonparser SRC ${BSON_LIBRARY_SRCS} ${BSON_LIBRARY_H})
qi_use_lib(mybsonparser)
qi_stage_lib(mybsonparser)

# Create an executable named bar
# with the source file: main.cpp
qi_create_bin(BSON SRC src/main.cpp DEPENDS mybsonparser)

# Add a simple test:
add_subdirectory(tests)
