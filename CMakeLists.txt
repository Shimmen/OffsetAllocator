cmake_minimum_required(VERSION 3.10)

project(offsetAllocator)

add_library(${PROJECT_NAME} src/offsetAllocator.cpp include/offsetAllocator/offsetAllocator.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if(MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
