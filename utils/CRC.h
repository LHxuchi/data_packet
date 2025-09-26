//
// Created by DELL on 25-9-25.
//

#ifndef CRC_H
#define CRC_H
#include <cstdint>

namespace data_packet {
    /**
     * @brief 用于计算文件对应的CRC校验码
     * @param data 数据指针，以字节为单位
     * @param size 数据长度
     * @return 计算得到的CRC-32校验码
     */
    uint32_t CRC_calculate(const uint8_t* data, uint64_t size);

    /**
     * @brief 使用已有的CRC校验码对数据进行校验
     * @param CRC_code 数据校验中需要使用的CRC-32校验码
     * @param data 数据指针，以字节为单位
     * @param size 数据长度
     * @return 校验结果
     */
    bool CRC_verify(uint32_t CRC_code, const uint8_t* data, uint64_t size);
}


#endif //CRC_H
