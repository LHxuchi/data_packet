//
// Created by DELL on 25-9-25.
//

#include "../include/utils/CRC.h"

/* 此处的CRC特指CRC-32 / IEEE 802.3 标准的CRC-32，
 * 计算过程中包含输入数据反转与输出数据反转，生成多项式
 * 为0xEDB88320UL，初始值为0xFFFFFFFFUL，结果异或
 * 值为0xFFFFFFFFUL。
 */

namespace {
    constexpr uint32_t POLYNOMIAL = 0xEDB88320; // CRC-32的生成多项式
    constexpr uint32_t INIT = 0xffffffff; // CRC-32初始值
    constexpr uint32_t FINALXOR = 0xffffffff;

    // CRC-32的查表优化，来自于1字节数据的所有与生成多项式的计算结果
    uint32_t crc32_table[256];

    void init_crc32_table() {
        // 计算一字节可能的多项式计算结果
        for (int i= 0; i < 256; i++) {

            // 反转计算，故结果是从尾部开始试商
            uint32_t crc = i;
            for (int j = 0; j < 8; j++) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ POLYNOMIAL;
                }
                else
                    crc >>= 1;
            }

            // 存表
            crc32_table[i] = crc;
        }
    }

}



uint32_t data_packet::CRC_calculate(const uint8_t *data, uint64_t size) {
    // 初始化计算表
    static bool is_initialized = false;
    if (!is_initialized) {
        init_crc32_table();
    }

    uint32_t crc = INIT;

    // 查表运算
    for (uint64_t i = 0; i < size; i++) {
        auto index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[index];
    }

    return crc ^ FINALXOR;
}

bool data_packet::CRC_verify(const uint32_t CRC_code, const uint8_t *data, uint64_t size) {
    return CRC_code == CRC_calculate(data, size);
}
