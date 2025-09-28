//
// Created by DELL on 25-9-27.
//

#include "../include/packet/local_file_packet.h"
#include "../include/utils/ms_dos_time.h"
#include "../include/utils/CRC.h"
#include "../include/utils/byte.h"

#include <fstream>


data_packet::local_file_packet::local_file_packet():header{},data(nullptr) {}

std::unique_ptr<uint8_t[]> data_packet::local_file_packet::header_buffer() const {
    auto buffer = std::make_unique<uint8_t[]>(local_file_header::SIZE+header.file_name_length);

    size_t index = 0;

    auto write_to_buffer = [&buffer, &index](const auto&... elements) {((buffer[index++] = elements), ...);};

    std::apply(write_to_buffer,word_to_byte(header.creation_date));
    std::apply(write_to_buffer,word_to_byte(header.creation_time));
    std::apply(write_to_buffer,word_to_byte(header.last_modified_date));
    std::apply(write_to_buffer,word_to_byte(header.last_modified_time));
    std::apply(write_to_buffer,qword_to_byte(header.original_file_size));
    std::apply(write_to_buffer,word_to_byte(header.file_name_length));
    std::apply(write_to_buffer,dword_to_byte(header.CRC));

    for (const auto& c:header.file_name) {
        buffer[index++] = c;
    }

    return buffer;
}

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

    file.close();
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

void data_packet::local_file_packet::read_local_file(const char *file_data) {
    if (!file_data) {
        throw std::invalid_argument("Data could not be found at "+
            std::to_string(reinterpret_cast<unsigned long long>(file_data)));
    }

    /* 文件头读入 */
    header.creation_date = get_word(file_data);
    file_data +=2;
    header.creation_time = get_word(file_data);
    file_data += 2;
    header.last_modified_date = get_word(file_data);
    file_data += 2;
    header.last_modified_time = get_word(file_data);
    file_data += 2;
    header.original_file_size = get_qword(file_data);
    file_data += 8;
    header.file_name_length = get_word(file_data);
    file_data += 2;
    header.CRC = get_dword(file_data);
    file_data += 4;
    header.file_name.reserve(static_cast<size_t>(header.file_name_length));
    for (uint16_t i = 0; i < header.file_name_length; ++i) {
        header.file_name.push_back(file_data[i]);
    }

    /* CRC校验 */
    if (!CRC_verify(header.CRC,reinterpret_cast<const uint8_t*>(file_data),header.original_file_size))
        throw std::runtime_error(header.file_name + " CRC ERROR");

    /* 文件数据拷贝 */
    this->data = std::make_unique<uint8_t[]>(static_cast<size_t>(header.original_file_size));
    memcpy(this->data.get(), file_data, static_cast<size_t>(header.original_file_size));
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
