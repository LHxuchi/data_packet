//
// Created by DELL on 25-9-26.
//

#ifndef FILE_HEADER_H
#define FILE_HEADER_H
#include <cstdint>
#include <string>

namespace data_packet {
    class file_header {
    public:
        file_header() = default;

        ~file_header() = default;

        enum class compression_method {
            None = 0,LZ77
        };

        enum class encryption_method {
            None = 0
        };

        [[nodiscard]] compression_method get_compression_method() const;

        void set_compression_method(compression_method method);

        [[nodiscard]] encryption_method get_encryption_method() const;

        void set_encryption_method(encryption_method method);

        [[nodiscard]] uint8_t get_compression_and_encryption_method() const {
            return compression_and_encryption_method;
        }

        void set_compression_and_encryption_method(const uint8_t compression_and_encryption_method) {
            this->compression_and_encryption_method = compression_and_encryption_method;
        }

        [[nodiscard]] uint32_t get_file_number() const {
            return file_number;
        }

        void set_file_number(const uint32_t file_number) {
            this->file_number = file_number;
        }

        [[nodiscard]] uint64_t get_original_files_size() const {
            return original_files_size;
        }

        void set_original_files_size(const uint64_t original_files_size) {
            this->original_files_size = original_files_size;
        }

        [[nodiscard]] uint64_t get_compressed_files_size() const {
            return compressed_files_size;
        }

        void set_compressed_files_size(const uint64_t compressed_files_size) {
            this->compressed_files_size = compressed_files_size;
        }

        [[nodiscard]] uint32_t get_crc_32() const {
            return CRC_32;
        }

        void set_crc_32(const uint32_t crc_32) {
            CRC_32 = crc_32;
        }

        [[nodiscard]] uint16_t get_date() const {
            return date;
        }

        void set_date(const uint16_t date) {
            this->date = date;
        }

        [[nodiscard]] uint16_t get_time() const {
            return time;
        }

        void set_time(const uint16_t time) {
            this->time = time;
        }

        static constexpr unsigned int SIZE = 29;
    private:
        uint8_t compression_and_encryption_method = 0; // 压缩与加密方法，压缩方法占6-7bits，加密方法占4-5bits
        uint32_t file_number = 0; // 包中文件个数
        uint64_t original_files_size = 0; // 包中原始文件与文件头长度总和
        uint64_t compressed_files_size = 0; // 已压缩后包文件与文件头长度总和
        uint32_t CRC_32 = 0; // CRC-32校验码
        uint16_t date = 0; // MS-DOS日期
        uint16_t time = 0;// MS-DOS时间

        /* 文件头总长度为29B */
    };

    /**
     * @brief 包内每个文件的文件头
     */
    struct local_file_header {
        uint64_t original_file_size = 0; ///<指明文件的大小
        uint16_t last_modified_date = 0; ///<最近修改的日期
        uint16_t last_modified_time = 0; ///<最近修改的时间
        uint16_t creation_date = 0; ///<创建日期
        uint16_t creation_time = 0; ///<创建时间
        uint32_t CRC = 0; ///<循环校验码
        uint16_t file_name_length = 0; ///< 件名长度
        std::string file_name; ///< 文件绝对路径

        static constexpr unsigned int SIZE = 22;
    };

    /* 定长file_header的长度为22字节 */

}


#endif //FILE_HEADER_H
