//
// Created by DELL on 25-9-27.
//

#include "../include/packet/file_packet.h"
#include "../include/utils/byte.h"

#include <fstream>
#include <filesystem>

#include "../include/utils/CRC.h"

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
        header.set_compressed_files_size(data_packet::get_qword(data));
        data += 8;
        header.set_compression_and_encryption_method(*data);
        data += 1;
        header.set_crc_32(data_packet::get_dword(data));
    }

    std::unique_ptr<uint8_t[]> file_header_buffer() {

    }
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
        packet.header.get_compressed_files_size())) {
        throw std::runtime_error(path + " CRC ERROR");
    }

    /* 分配本地文件包存储空间 */
    packet.local_file_packets.reserve(packet.header.get_file_number());

    auto data = buffer.get()+file_header::SIZE; // 本地文件数据指针
    const auto end = buffer.get()+file_header::SIZE+packet.header.get_compressed_files_size(); // 文件结尾
    local_file_packet local_packet; //

    while (data < end) {
        local_packet.read_local_file(reinterpret_cast<const char*>(data));
        packet.local_file_packets.push_back(local_packet);
        data += local_packet.size();
    }

    return packet;
}

void data_packet::write_file(const std::string &path, const file_packet &packet) {
    std::ofstream out(path,std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error(path + " could not be opened");
    }



}
