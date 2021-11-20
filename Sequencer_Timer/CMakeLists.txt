cmake_minimum_required(VERSION 3.10)
project(RT_Generic_Sequencer C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

set(SOURCES main.c threads.c threads.h types.h sequencer.c sequencer.h)

add_executable(RT_Generic_Sequencer ${SOURCES})

target_link_libraries (RT_Generic_Sequencer pthread rt)
