cmake_minimum_required(VERSION 3.24)
project(WebsocketChat)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(websocketpp)

add_executable(WebsocketChat main.cpp)
