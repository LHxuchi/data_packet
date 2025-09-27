//
// Created by DELL on 25-9-27.
//

#include "../include/packet/local_file_packet.h"
#include "../include/utils/ms_dos_time.h"

#include <fstream>

#include "../include/utils/CRC.h"

data_packet::local_file_packet::local_file_packet():header{},data(nullptr) {}

data_packet::local_file_packet::local_file_packet(const file_meta_info &info) {
    set_file(info);
}

void data_packet::local_file_packet::set_file(const file_meta_info &info) {
    /* 基本信息导入 */
    header.file_name = std::get<0>(info);
    header.original_file_size = std::get<1>(info);
    header.last_modified_date = std::get<2>(info);
    header.last_modified_time = std::get<3>(info);
    header.creation_date = to_date(std::time(nullptr));
    header.creation_time = to_time(std::time(nullptr));
    header.file_name_length = header.file_name.length();

    /* 分配内存 */
    data = std::make_unique<uint8_t[]>(header.original_file_size);

    /* 打开文件 */
    std::ifstream file(header.file_name, std::ios::binary);
    if (!file.is_open()) {
        throw std::invalid_argument("File could not be opened");
    }

    /* 读入文件 */
    file.read(reinterpret_cast<char*>(data.get()), static_cast<long long>(header.original_file_size));

    /* 计算CRC */
    header.CRC = CRC_calculate(data.get(), header.original_file_size);
    if (header.CRC == 0xffffffff) {
        throw std::runtime_error("CRC ERROR");
    }
}

void data_packet::local_file_packet::clear() noexcept {
    header.original_file_size = 0;
    header.last_modified_date = 0;
    header.last_modified_time = 0;
    header.creation_date = 0;
    header.creation_time = 0;
    header.file_name_length = 0;
    header.file_name.clear();
    header.CRC = 0;

    data.reset(nullptr);
}

uint64_t data_packet::local_file_packet::size() const noexcept {
    return header.original_file_size + header.file_name_length + local_file_header::SIZE;
}

uint64_t data_packet::local_file_packet::file_size() const noexcept {
    return header.original_file_size;
}

bool data_packet::local_file_packet::empty() const noexcept {
    return (header.file_name_length == 0 && header.last_modified_date == 0
    && header.last_modified_time == 0 && header.creation_date == 0
    && header.creation_time == 0 && header.CRC == 0
    && header.file_name.empty() && header.original_file_size == 0)
    || data == nullptr;
}

data_packet::local_file_packet::local_file_packet(local_file_packet &&other) noexcept {
    this->header = std::move(other.header);
    this->data = std::move(other.data);
}

data_packet::local_file_packet & data_packet::local_file_packet::operator=(local_file_packet &&other) noexcept {
    this->header = std::move(other.header);
    this->data = std::move(other.data);
    return *this;
}
