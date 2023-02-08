cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

project(MyCollection VERSION 0.1 DESCRIPTION "A class with an std::vector and multiple algorithms for sorting and searching")
add_executable(Test "Test.cpp")

add_subdirectory(MyPrintLibrary)

list(APPEND EXTRA_LIBS MyPrintLibrary)

target_link_libraries(Test PUBLIC ${EXTRA_LIBS})
target_include_directories(Test PUBLIC "${PROJECT_SOURCE_DIR}/includes")
