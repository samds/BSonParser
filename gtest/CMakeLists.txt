# CMake file for gtest

cmake_minimum_required(VERSION 2.8)
project(gtest)
find_package(qibuild)

include_directories("." "include")
qi_create_lib(gtest src/gtest-all.cc)
qi_use_lib(gtest PTHREAD)
qi_stage_lib(gtest)

qi_create_lib(gtest_main src/gtest_main.cc)
qi_use_lib(gtest_main GTEST)
qi_stage_lib(gtest_main)

add_subdirectory(include)

