#ifndef DATA_PACKET_LIBRARY_H
#define DATA_PACKET_LIBRARY_H
#include <cstdint>

namespace data_packet{
/**
 * @brief 包文件的协议头部，用于存储打包文件的meta信息
 */
class local_file_header {
public:
    enum class compression_method {
        None = 0, LZ77
    };

    enum class encryption_method {
        None = 0
    };

    local_file_header() = default;

    ~local_file_header();

    [[nodiscard]] uint64_t get_original_file_size() const {
        return original_file_size;
    }

    void set_original_file_size(const uint64_t original_file_size) {
        this->original_file_size = original_file_size;
    }

    [[nodiscard]] uint16_t get_last_modified_date() const {
        return last_modified_date;
    }

    void set_last_modified_date(const uint16_t last_modified_date) {
        this->last_modified_date = last_modified_date;
    }

    [[nodiscard]] uint16_t get_last_modified_time() const {
        return last_modified_time;
    }

    void set_last_modified_time(const uint16_t last_modified_time) {
        this->last_modified_time = last_modified_time;
    }

    [[nodiscard]] uint16_t get_creation_date() const {
        return creation_date;
    }

    void set_creation_date(const uint16_t creation_date) {
        this->creation_date = creation_date;
    }

    [[nodiscard]] uint16_t get_creation_time() const {
        return creation_time;
    }

    void set_creation_time(const uint16_t creation_time) {
        this->creation_time = creation_time;
    }

    [[nodiscard]] uint32_t get_crc() const {
        return CRC;
    }

    void set_crc(const uint32_t crc) {
        CRC = crc;
    }

    [[nodiscard]] uint16_t get_file_name_length() const {
        return file_name_length;
    }

    void set_file_name_length(const uint16_t file_name_length) {
        this->file_name_length = file_name_length;
    }

    [[nodiscard]] const char* get_file_name() const;

    void set_file_name(const char* file_name,uint16_t length);

private:
    uint64_t original_file_size = 0; // 指明文件的大小
    uint16_t last_modified_date = 0; // 最近修改的日期
    uint16_t last_modified_time = 0; // 最近修改的时间
    uint16_t creation_date = 0; // 创建日期
    uint16_t creation_time = 0; // 创建时间
    uint32_t CRC = 0; // 循环校验码
    uint16_t file_name_length = 0; // 文件名长度

    /* 定长file_header的长度为22字节 */

    char* file_name = nullptr; // 文件相对路径
};



}
#endif //DATA_PACKET_LIBRARY_H