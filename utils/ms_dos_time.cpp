//
// Created by DELL on 25-9-26.
//
#include "ms_dos_time.h"
#include <ctime>

namespace {
    /**
     * @brief 根据当前时间获取MS-DOS格式时间
     * @return MS-DOS日期与时间
     */
    std::pair<uint16_t,uint16_t> get_date_and_time(const std::time_t t) {

        const std::tm tm = *std::localtime(&t);

        unsigned int year = tm.tm_year + 1900;
        unsigned int month = tm.tm_mon + 1;
        unsigned int day = tm.tm_mday;
        unsigned int hour = tm.tm_hour;
        unsigned int minute = tm.tm_min;
        unsigned int second = tm.tm_sec;

        year -= 1980;

        uint16_t date = 0,time = 0;

        date |= ((year << 9) & 0xfe00);
        date |= ((month << 5) & 0x1e0);
        date |= (day & 0x1f);

        time |= ((hour << 11) & 0xf800);
        time |= ((minute << 5) & 0x7e0);
        time |= ((second >> 1) & 0x1f);

        return std::pair<uint16_t,uint16_t>{date,time};
    }
}


uint16_t data_packet::get_date() {
    return get_date_and_time(std::time(nullptr)).first;
}

uint16_t data_packet::get_time() {
    return get_date_and_time(std::time(nullptr)).second;
}

uint16_t data_packet::to_date(const std::time_t timestamp) {
    return get_date_and_time(timestamp).first;
}

uint16_t data_packet::to_time(const std::time_t timestamp) {
    return get_date_and_time(timestamp).second;
}
