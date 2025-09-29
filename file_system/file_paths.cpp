//
// Created by DELL on 25-9-26.
//
#include "../include/file_system/file_paths.h"
#include "../include/utils/ms_dos_time.h"

#include <filesystem>
#include <iostream>
#include <queue>


std::vector<data_packet::file_meta_info> data_packet::get_file_metas(const std::string &path) {
    using namespace std::filesystem;

    std::vector<file_meta_info> metas;

    try {
        std::queue<directory_iterator> queue; // 层次遍历队列
        queue.emplace(path);
        while (!queue.empty()) {
            // 队列目录遍历
            for (const auto& entry : queue.front()) {
                if (entry.is_directory()) {
                    // 目录入队
                    queue.emplace(entry.path());
                }
                else if (entry.exists()) {
                    // 将 file_time_type 转换为 system_clock 时间点
                    auto sys_time =
                        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        entry.last_write_time() - file_time_type::clock::now() + std::chrono::system_clock::now()
                    );

                    // 转换为时间戳（自 1970-01-01 以来的秒数）
                    std::time_t timestamp = std::chrono::system_clock::to_time_t(sys_time);

                    metas.emplace_back(
                    entry.path().string(),
                    entry.file_size(),
                    to_date(timestamp),
                    to_time(timestamp));
                }
            }
            queue.pop();
        }
    }
    catch (const filesystem_error& err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    return metas;
}
