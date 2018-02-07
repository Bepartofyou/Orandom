cmake_minimum_required(VERSION 3.0)
project(orandom)

add_definitions(-std=c++11 -g -Wall) 

set(SRC_LIST

orandom.hpp
hello.cpp
)

add_executable(orandom ${SRC_LIST})
