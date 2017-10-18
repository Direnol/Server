cmake_minimum_required(VERSION 3.9)
project(ServerForESP)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Server.cpp Server.h)
add_executable(ServerForESP ${SOURCE_FILES})