
add_definitions("-DBINARY_DIR=\"${PROJECT_SOURCE_DIR}/tests/binary\"")
file(COPY ${PROJECT_SOURCE_DIR}/tests/binary DESTINATION ${PROJECT_BINARY_DIR}/tests)

set(element_tests_srcs
	elements/test_element_double.cpp
	elements/test_element_string.cpp
	elements/test_element_embedded_document.cpp
	elements/test_element_array.cpp
	elements/test_element_binary_data.cpp
	elements/test_element_undefined.cpp
	elements/test_element_objectid.cpp
	elements/test_element_boolean.cpp
	elements/test_element_datetime.cpp
	elements/test_element_null.cpp
	elements/test_element_regex.cpp
	elements/test_element_dbpointer.cpp
	elements/test_element_javascriptcode.cpp
	elements/test_element_symbol.cpp
	elements/test_element_codews.cpp
	elements/test_element_integer.cpp
	elements/test_element_timestamp.cpp
	elements/test_element_integer64.cpp
	elements/test_element_decimal128.cpp
	elements/test_element_minmaxkey.cpp
    	elements/test_element_factory.cpp
)

set(parser_tests_srcs
	parser/test_file_parser.cpp
	parser/test_stream_parser.cpp
)


#qi_create_gtest(bar_tests ${mybsonparser_tests_srcs} DEPENDS GTEST mybsonparser TIMEOUT 240)

qi_create_gtest(test_bson_element ${element_tests_srcs} DEPENDS mybsonparser)
qi_create_gtest(test_bson_stream_parser ${parser_tests_srcs} DEPENDS mybsonparser)
