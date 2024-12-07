cmake_minimum_required(VERSION 3.10)

project(Hell VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-DUNICODE -D_UNICODE")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Hell ${SOURCES})