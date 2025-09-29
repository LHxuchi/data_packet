//
// Created by DELL on 25-9-25.
//

#include "../include/utils/CRC.h"

/* 此处的CRC特指CRC-32 / IEEE 802.3 标准的CRC-32，
 * 计算过程中包含输入数据反转与输出数据反转，生成多项式
 * 为0xEDB88320UL，初始值为0xFFFFFFFFUL，结果异或
 * 值为0xFFFFFFFFUL。
 */


uint32_t data_packet::CRC_calculate(const uint8_t *data, uint64_t size) {
    if (data == nullptr || size == 0) {
        return 0xffffffff;
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
