//
// Created by DELL on 25-9-26.
//

#include "../include/header/file_header.h"

namespace {
    constexpr uint8_t COMPRESSION_METHOD_MASK = 0xc0;
    constexpr uint8_t ENCRYPTION_METHOD_MASK = 0x30;
}

data_packet::file_header::compression_method data_packet::file_header::get_compression_method() const {
    uint8_t code = this->compression_and_encryption_method & COMPRESSION_METHOD_MASK;

    code >>= 6;

    compression_method type;
    switch (code) {
        case 0x00:
            type = compression_method::None;
            break;
        case 0x01:
            type = compression_method::LZ77;
            break;
        default:
            type = compression_method::None;
    }

    return type;
}

void data_packet::file_header::set_compression_method(compression_method method) {
    auto code = static_cast<uint8_t>(method);

    code <<= 6;

    this->compression_and_encryption_method &= (~COMPRESSION_METHOD_MASK);

    this->compression_and_encryption_method |= code;
}

data_packet::file_header::encryption_method data_packet::file_header::get_encryption_method() const {
    uint8_t code = this->compression_and_encryption_method & ENCRYPTION_METHOD_MASK;

    code >>= 4;

    encryption_method type;
    switch (code) {
        case 0x00:
            type = encryption_method::None;
            break;
        default:
            type = encryption_method::None;
    }

    return type;
}

void data_packet::file_header::set_encryption_method(encryption_method method) {
    auto code = static_cast<uint8_t>(method);

    code <<= 4;

    this->compression_and_encryption_method &= (~ENCRYPTION_METHOD_MASK);

    this->compression_and_encryption_method |= code;
}
