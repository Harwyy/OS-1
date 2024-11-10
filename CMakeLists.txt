cmake_minimum_required(VERSION 3.10)

project(HELL VERSION 1.0 LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Hell ${SOURCES})