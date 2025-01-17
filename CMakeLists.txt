cmake_minimum_required(VERSION 3.27)

project(lab2)

set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})