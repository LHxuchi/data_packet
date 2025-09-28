//
// Created by DELL on 25-9-27.
//

#ifndef FILE_PACKET_H
#define FILE_PACKET_H

#include <vector>

#include "../header/file_header.h"
#include "local_file_packet.h"

namespace data_packet {
    /**
     * @brief 解包后得到的文件包，共含有header与local_file_packets两个成员
     */
    struct file_packet {
        file_header header; ///<文件头
        std::vector<local_file_packet> local_file_packets; ///<本地文件包
    };

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
