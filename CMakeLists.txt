cmake_minimum_required(VERSION 3.24)
project(Carbonchat)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if(NOT AMALGAMATE_SRC)
find_package( PythonInterp 3.0 REQUIRED )
add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/include/json/dist/jsoncpp.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/build.py ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${PROJECT_SOURCE_DIR} src/server.cpp
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
set(AMALGAMATE_SRC TRUE CACHE BOOL "Are the JSON source files amalgamated?" FORCE)
endif()

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/boost_1_81_0") # TODO add if statment for Windows
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/local/boost_1_81_0/lib64-msvc-14.3")
find_package(Boost)
if (Boost_FOUND)
    message(STATUS "Boost was found")
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
else()
    message(AUTHOR_WARNING "Boost is required. Running the program will fail.")
    return()
endif()

## Enable all compiler warnings
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# Link against pthreads under Unix systems
if (NOT MSVC AND NOT MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# Under MinGW, statically link against the standard libraries
if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Force WebSocket++ to use 100% C++11 mode, so that it doesn't try to look for Boost
# (Note that under MinGW, Boost.Thread is still required, due to a bug in MinGW that prevents the C++11 version from being used)
add_definitions(-D_WEBSOCKETPP_CPP11_STRICT_)

# Add the third-party headers directory to the search path
include_directories(SYSTEM "include/websocketpp")
include_directories(SYSTEM "include/asio/asio/include/")
include_directories(SYSTEM "include/json/dist") #TODO: compile with python first

# Compile jsoncpp from source
add_library(jsoncpp STATIC include/json/dist/jsoncpp.cpp)

# Build the server executable
add_executable(server src/server.cpp src/WebsocketServer.cpp)
target_link_libraries (server jsoncpp)

set_target_properties(server PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")