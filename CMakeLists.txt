cmake_minimum_required(VERSION 3.24)
project(WebsocketChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include_directories(websocketpp)
include_directories(${PROJECT_SOURCE_DIR}/seasocks/src/main/c)

add_executable(WebsocketChat main.cpp ws_test.cpp)
