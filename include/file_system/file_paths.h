//
// Created by DELL on 25-9-26.
//

#ifndef FILE_TREE_H
#define FILE_TREE_H

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace data_packet {
    /**
     * @brief 文件meta数据包括文件相对路径、文件大小(8B)、文件最近修改日期(2B)、文件最近修改时间(2B)，依照该顺序打包
     */
    using file_meta_info = std::tuple<std::string,uint64_t,uint16_t,uint16_t>;

    /**
     * @brief 获取指定路径下展开后所有文件的相对路径
     * @param path 指定路径
     * @return 所有文件的相对路径
     */
    std::vector<file_meta_info> get_file_metas(const std::string &path);
}

#endif //FILE_TREE_H
