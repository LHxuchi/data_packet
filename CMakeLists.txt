cmake_minimum_required(VERSION 3.31)
project(data_packet)

set(CMAKE_CXX_STANDARD 20)

add_library(data_packet SHARED
        utils/CRC.cpp
        include/utils/CRC.h
        utils/ms_dos_time.cpp
        include/utils/ms_dos_time.h
        include/file_system/file_paths.h
        file_system/file_paths.cpp
        header/file_header.cpp
        include/header/file_header.h
        packet/local_file_packet.cpp
        include/packet/local_file_packet.h
        include/packet/file_packet.h
        packet/file_packet.cpp
        utils/byte.cpp
        include/utils/byte.h)

add_executable(TEST testmain.cpp)

target_link_libraries(TEST PRIVATE data_packet)
