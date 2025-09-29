//
// Created by DELL on 25-9-27.
//

#include "../include/packet/file_packet.h"
#include "../include/utils/byte.h"

#include <fstream>
#include <filesystem>

#include "../include/utils/CRC.h"
#include "../include/utils/ms_dos_time.h"

namespace {
    /**
     * @brief 根据指定地址写入文件头
     * @param header 被写入的文件头
     * @param data 指定地址
     */
    void read_header(data_packet::file_header& header, const char* data) {
        header.set_date(data_packet::get_word(data));
        data += 2;
        header.set_time(data_packet::get_word(data));
        data += 2;
        header.set_file_number(data_packet::get_dword(data));
        data += 4;
        header.set_original_files_size(data_packet::get_qword(data));
        data += 8;
        header.set_files_size(data_packet::get_qword(data));
        data += 8;
        header.set_compression_and_encryption_method(*data);
        data += 1;
        header.set_crc_32(data_packet::get_dword(data));
    }

}


data_packet::file_packet data_packet::make_file_packet(std::vector<local_file_packet> &&local_file_packets,
    uint64_t original_files_size, file_header::compression_method c_method, file_header::encryption_method e_method) {

    file_packet packet;
    packet.header.set_date(get_date());
    packet.header.set_time(get_time());
    packet.header.set_file_number(local_file_packets.size());
    packet.header.set_original_files_size(original_files_size);

    uint64_t files_size = 0;
    auto crc_buffer = std::make_unique<uint8_t[]>(packet.header.get_file_number()*4);
    uint32_t index = 0;
    for (auto& local_file_packet : local_file_packets) {
        files_size += local_file_packet.size();
        auto bytes = dword_to_byte(local_file_packet.get_crc());
        crc_buffer[index++] = std::get<0>(bytes);
        crc_buffer[index++] = std::get<1>(bytes);
        crc_buffer[index++] = std::get<2>(bytes);
        crc_buffer[index++] = std::get<3>(bytes);
    }

    packet.header.set_files_size(files_size);
    packet.header.set_compression_method(c_method);
    packet.header.set_encryption_method(e_method);
    packet.header.set_crc_32(CRC_calculate(crc_buffer.get(),index));

    packet.local_file_packets = std::move(local_file_packets);

    return packet;

}

std::unique_ptr<uint8_t[]> data_packet::file_header_buffer(const data_packet::file_header& header) {
    using namespace data_packet;

    auto buffer = std::make_unique<uint8_t[]>(data_packet::file_header::SIZE);

    size_t index = 0;

    auto write_to_buffer = [&buffer, &index](const auto&... elements) {((buffer[index++] = elements), ...);};

    std::apply(write_to_buffer, word_to_byte(header.get_date()));
    std::apply(write_to_buffer, word_to_byte(header.get_time()));
    std::apply(write_to_buffer, dword_to_byte(header.get_file_number()));
    std::apply(write_to_buffer, qword_to_byte(header.get_original_files_size()));
    std::apply(write_to_buffer, qword_to_byte(header.get_files_size()));
    buffer[index++] = header.get_compression_and_encryption_method();
    std::apply(write_to_buffer, dword_to_byte(header.get_crc_32()));

    return buffer;

}

data_packet::file_packet data_packet::read_file(const std::string &path) {

    file_packet packet; // 返回结果

    std::ifstream file(path, std::ios::binary); // 打开文件
    if (!file.is_open()) {
        throw std::runtime_error(path + " could not be opened");
    }

    /* 对文件长度进行初步判定，若不符合则报错 */

    auto file_size = std::filesystem::file_size(path);
    if (file_size <= 29) {
        throw std::runtime_error(path + " could not be formed");
    }

    /* 将文件读入内存 */

    std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(file_size); // 读入文件内存指针
    file.read(reinterpret_cast<char *>(buffer.get()), static_cast<long long>(file_size));

    /* 文件头读入 */
    read_header(packet.header,reinterpret_cast<const char *>(buffer.get()));

    /* CRC校验 */
    if (CRC_verify(packet.header.get_crc_32(),
        (buffer.get()+file_header::SIZE),
        packet.header.get_files_size())) {
        throw std::runtime_error(path + " CRC ERROR");
    }

    /* 分配本地文件包存储空间 */
    packet.local_file_packets.resize(packet.header.get_file_number());

    auto data = buffer.get()+file_header::SIZE; // 本地文件数据指针
    const auto end = buffer.get()+file_header::SIZE+packet.header.get_files_size(); // 文件结尾
    local_file_packet local_packet; //

    size_t index = 0;
    while (data < end) {
        local_packet.read_local_file(reinterpret_cast<const char*>(data));
        size_t local_size = local_packet.size();
        packet.local_file_packets[index++] = std::move(local_packet);
        data += local_size;
    }

    return packet;
}

void data_packet::write_file(const std::string &path, const file_packet &packet) {

    // 打开文件
    std::ofstream out(path,std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error(path + " could not be opened");
    }

    // 写入文件头
    auto buffer = file_header_buffer(packet.header);
    out.write(reinterpret_cast<const char *>(buffer.get()),file_header::SIZE);
    buffer.release();

    // 写入包文件
    for (const auto& local_packet : packet.local_file_packets) {
        if (local_packet.empty())
            throw std::runtime_error(path + " is empty");
        out.write(reinterpret_cast<const char*>(local_packet.header_buffer().get()),local_packet.header_size());
        out.write(reinterpret_cast<const char*>(local_packet.buffer()),
            static_cast<long long>(local_packet.file_size()));
    }

    out.close();
}
