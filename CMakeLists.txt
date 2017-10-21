cmake_minimum_required(VERSION 3.8)
project(ServerForESP)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Server.cpp Server.h)
add_executable(ServerForESP ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} pthread)