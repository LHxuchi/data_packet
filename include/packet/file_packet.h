//
// Created by DELL on 25-9-27.
//

#ifndef FILE_PACKET_H
#define FILE_PACKET_H

#include <vector>

#include "../header/file_header.h"
#include "local_file_packet.h"
#include "../header/file_header.h"
#include "../header/file_header.h"
#include "../header/file_header.h"
#include "../header/file_header.h"

namespace data_packet {
    /**
     * @brief 解包后得到的文件包，共含有header与local_file_packets两个成员
     */
    struct file_packet {
        file_header header; ///<文件头
        std::vector<local_file_packet> local_file_packets; ///<本地文件包
    };

    /**
     * @brief 将包文件组打包为文件包
     * @param local_file_packets 包文件组
     * @param c_method 压缩方法，默认不压缩
     * @param e_method 加密方法，默认不加密
     * @param original_files_size 原始文件总长度
     * @return 文件包
     */
    file_packet make_file_packet(std::vector<local_file_packet>&& local_file_packets,
                                 uint64_t original_files_size,
                                 file_header::compression_method c_method = file_header::compression_method::None,
                                 file_header::encryption_method e_method = file_header::encryption_method::None);

    /**
     * @brief 将文件头整理为流
     * @param header 指定文件头
     * @return 数据流
     */
    std::unique_ptr<uint8_t[]> file_header_buffer(const data_packet::file_header& header);

    /**
     * @brief 读取文件信息并分配文件头
     * @param path 读取文件的文件路径
     * @return 文件包
     */
    file_packet read_file(const std::string& path);

    /**
     * @brief 将文件包写入指定路径中
     * @param path 需要被写入文件的指定路径
     * @param packet 文件包
     */
    void write_file(const std::string& path, const file_packet& packet);
}

#endif //FILE_PACKET_H
