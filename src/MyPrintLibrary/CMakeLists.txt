cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
project(PrettyPrintingLib VERSION 0.1 DESCRIPTION "A lib with hopefully different printing functions")
add_library(MyPrintLibrary SHARED "PrettyPrinting.cpp") # pour compiler un .so
target_include_directories(MyPrintLibrary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")
