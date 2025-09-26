//
// Created by DELL on 25-9-26.
//

#ifndef TIME_H
#define TIME_H
#include <chrono>

namespace data_packet {
    /**
     * @brief 获取当前日期，格式为MS-DOS
     * @return MS-DOS日期
     */
    uint16_t get_date();

    /**
     * @brief 获取当前时间，格式为MS-DOS
     * @return MS-DOS时间
     */
    uint16_t get_time();

    /**
     * @brief 根据已有的时间戳得到MS-DOS格式的日期
     * @param timestamp 传入的指定时间戳
     * @return MS-DOS日期
     */
    uint16_t to_date(std::time_t timestamp);

    /**
     * @brief 根据已有的时间戳得到MS-DOS格式的时间
     * @param timestamp 传入的指定时间戳
     * @return MS-DOS时间
     */
    uint16_t to_time(std::time_t timestamp);
}
#endif //TIME_H
