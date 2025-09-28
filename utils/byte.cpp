//
// Created by DELL on 25-9-28.
//
#include "../include/utils/byte.h"

namespace data_packet {
    uint16_t get_word(const char* data) {
        uint16_t dword = 0;
        dword += static_cast<uint16_t>(*data);
        ++data;
        dword <<= 8;
        dword += static_cast<uint16_t>(*data);
        return dword;
    }

    uint32_t get_dword(const char* data) {
        uint32_t qword = 0;
        qword += static_cast<uint32_t>(*data);
        ++data;
        qword <<= 8;
        qword += static_cast<uint32_t>(*data);
        ++data;
        qword <<= 8;
        qword += static_cast<uint32_t>(*data);
        ++data;
        qword <<= 8;
        qword += static_cast<uint32_t>(*data);
        return qword;
    }

    uint64_t get_qword(const char* data) {
        uint64_t qword64 = 0;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        ++data;
        qword64 <<= 8;
        qword64 += static_cast<uint64_t>(*data);
        return qword64;
    }

    std::tuple<uint8_t, uint8_t> word_to_byte(uint16_t word) {
        uint8_t byte1 = 0,byte2 = 0;
        byte1 = static_cast<uint8_t>((word >> 8) & 0xFF);
        byte2 = static_cast<uint8_t>(word & 0xFF);
        return std::make_tuple(byte1, byte2);
    }

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> dword_to_byte(uint32_t dword) {
        uint8_t byte1 = 0,byte2 = 0,byte3 = 0, byte4 = 0;
        byte1 = static_cast<uint8_t>((dword >> 24) & 0xFF);
        byte2 = static_cast<uint8_t>((dword >> 16) & 0xFF);
        byte3 = static_cast<uint8_t>((dword >> 8) & 0xFF);
        byte4 = static_cast<uint8_t>(dword & 0xFF);
        return std::make_tuple(byte1, byte2, byte3, byte4);
    }

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> qword_to_byte(uint64_t qword) {
        uint8_t byte1 = 0,byte2 = 0,byte3 = 0,byte4 = 0,byte5 = 0 ,byte6 = 0,byte7 = 0,byte8 = 0;
        byte1 = static_cast<uint8_t>((qword >> 56) & 0xFF);
        byte2 = static_cast<uint8_t>((qword >> 48) & 0xFF);
        byte3 = static_cast<uint8_t>((qword >> 40) & 0xFF);
        byte4 = static_cast<uint8_t>((qword >> 32) & 0xFF);
        byte5 = static_cast<uint8_t>((qword >> 24) & 0xFF);
        byte6 = static_cast<uint8_t>((qword >> 16) & 0xFF);
        byte7 = static_cast<uint8_t>((qword >> 8) & 0xFF);
        byte8 = static_cast<uint8_t>(qword & 0xFF);
        return std::make_tuple(byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8);
    }
}
