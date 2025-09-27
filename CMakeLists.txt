cmake_minimum_required(VERSION 3.31)
project(data_packet)

set(CMAKE_CXX_STANDARD 20)

add_library(data_packet SHARED header/local_file_header.cpp
        utils/CRC.cpp
        include/utils/CRC.h
        utils/ms_dos_time.cpp
        include/utils/ms_dos_time.h
        include/file_system/file_paths.h
        file_system/file_paths.cpp
        header/file_header.cpp
        include/header/file_header.h)

add_executable(TEST testmain.cpp)

target_link_libraries(TEST PRIVATE data_packet)
