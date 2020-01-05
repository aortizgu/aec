cmake_minimum_required(VERSION 2.8.9)

set (TARGET "aec")
project (${TARGET})

#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_BUILD_TYPE release)

message ("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

include_directories(
    ./
    ./src
    third_part/miniaudio
    third_part/easyloggingpp/src
)

link_directories(
    third_part/easyloggingpp/build
)

set (SOURCES
    src/main.cpp
    src/audio_device.cpp
    third_part/easyloggingpp/src/easylogging++.cc
)

set (HEADERS
    third_part/easyloggingpp/src/easylogging++.h
)

add_executable (
    ${TARGET}
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries (
    ${TARGET}
    pthread 
    m 
    dl
)