//
// Created by DELL on 25-9-28.
//

#ifndef BYTE_H
#define BYTE_H
#include <cstdint>
#include <tuple>

namespace data_packet {
    /**
     * @brief 将指定地址的数据按照大端字节序转化为字
     * @param data 指定地址
     * @return 大端字节序的字
     */
    uint16_t get_word(const char* data);

    /**
     * @brief 将指定地址的数据按照大端字节序转化为双字
     * @param data 指定地址
     * @return 大端字节序的双字
     */
    uint32_t get_dword(const char* data);

    /**
     * @brief 将指定地址的数据按照大端字节序转化为四字
     * @param data 指定地址
     * @return 大端字节序的四字
     */
    uint64_t get_qword(const char* data);

    /**
     * @brief 将字依照大端字节序拆分为字节
     * @param word 被拆分字
     * @return 大端字节序的字节元组
     */
    std::tuple<uint8_t,uint8_t> word_to_byte(uint16_t word);

    /**
     * @brief 将双字依照大端字节序拆分为字节
     * @param dword 被拆分双字
     * @return 大端字节序的字节元组
     */
    std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> dword_to_byte(uint32_t dword);

    /**
     * @brief 将四字依照大端字节序拆分为字节
     * @param qword 被拆分四字
     * @return 大端字节序的字节元组
     */
    std::tuple<uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t> qword_to_byte(uint64_t qword);

}

#endif //BYTE_H

