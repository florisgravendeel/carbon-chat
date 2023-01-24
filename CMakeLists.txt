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

if(WIN32)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/boost_1_81_0")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/local/boost_1_81_0/lib64-msvc-14.3")
endif()
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

find_package(Boost COMPONENTS system filesystem thread unit_test_framework REQUIRED)
add_executable(unit_test test/carbonchat_test.cpp)
target_link_libraries(unit_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test tester)

# Force WebSocket++ to use 100% C++11 mode, as required in documentation
add_definitions(-D_WEBSOCKETPP_CPP11_STRICT_)

# Add the third-party headers directory to the search path
include_directories(SYSTEM "include/websocketpp")
include_directories(SYSTEM "include/asio/asio/include/")
include_directories(SYSTEM "include/json/dist")

# Compile jsoncpp from source
add_library(jsoncpp STATIC include/json/dist/jsoncpp.cpp)

# Build the executable
add_executable(carbonchat src/main.cpp)
target_link_libraries(carbonchat jsoncpp ${Boost_LIBRARIES})

set_target_properties(carbonchat PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")