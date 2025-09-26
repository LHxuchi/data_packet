cmake_minimum_required(VERSION 3.31)
project(data_packet)

set(CMAKE_CXX_STANDARD 20)

add_library(data_packet SHARED header/local_file_header.cpp
        utils/CRC.cpp
        utils/CRC.h
        utils/ms_dos_time.cpp
        utils/ms_dos_time.h
        file_system/file_paths.h
        file_system/file_paths.cpp)

add_executable(TEST testmain.cpp)

target_link_libraries(TEST PRIVATE data_packet)
