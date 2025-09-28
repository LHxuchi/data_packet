//
// Created by DELL on 25-9-27.
//

#ifndef LOCAL_FILE_PACKET_H
#define LOCAL_FILE_PACKET_H

#include <memory>

#include "../header/file_header.h"
#include "../file_system/file_paths.h"

namespace data_packet {
    /**
     * @brief 用于打包单个文件并进行资源管理的类，资源独占
     */
    class local_file_packet {
    public:
        local_file_packet();

        /**
         * @brief 获取文件头的流
         * @return 文件头流
         */
        std::unique_ptr<uint8_t[]> header_buffer() const;

        /**
         * @brief 根据文件的info信息构建包文件
         * @param info 需要打包的文件的meta信息
         */
        explicit local_file_packet(const file_meta_info& info);

        /**
         * @brief 根据指定文件的info信息构建包文件
         * @param info 需要打包的文件的meta信息
         */
        void set_file(const file_meta_info& info);

        /**
         * @brief 清空当前文件头，释放内存
         */
        void clear() noexcept;

        /**
         * @brief 返回已打包的文件与文件头总长度
         * @return 文件头与文件长度总和
         */
        [[nodiscard]] uint64_t size() const noexcept;

        /**
         * @brief 返回原始文件的长度
         * @return 原始文件长度
         */
        [[nodiscard]] uint64_t file_size() const noexcept;

        /**
         * @brief 判断当前文件头与文件内容是否为空
         * @return 若空则返回真，不空则返回假
         */
        [[nodiscard]] bool empty() const noexcept;

        /**
         * @brief 验证文件完整性并读入文件
         * @param file_data 文件起始指针
         */
        void read_local_file(const char* file_data);

        ~local_file_packet() = default;

        local_file_packet(const local_file_packet&) = delete;

        local_file_packet& operator=(const local_file_packet&) = delete;

        /**
         * @brief 仅支持移动语义
         */
        local_file_packet(local_file_packet&&) noexcept ;

        /**
         * @brief 仅支持移动语义
         * @return *this
         */
        local_file_packet& operator=(local_file_packet&&) noexcept ;

        /**
         * @brief 前向数据迭代器，用于遍历文件数据，以字节为单位
         */
        class data_iterator {
        public:
            /* 前向迭代标志 */
            using iterator_category = std::forward_iterator_tag;

            /* 数据类型 */
            using value_type = uint8_t;

            data_iterator() = default;

            data_iterator(const data_iterator&) = default;

            data_iterator(data_iterator&&) = default;

            ~data_iterator() = default;

            data_iterator operator++() noexcept {
                ++current;
                return *this;
            }

            data_iterator operator++(int) noexcept {
                const auto tmp = data_iterator{current};
                ++current;
                return tmp;
            }

            data_iterator operator--() noexcept {
                --current;
                return *this;
            }

            data_iterator operator--(int) noexcept {
                const auto tmp = data_iterator{current};
                --current;
                return tmp;
            }

            value_type& operator*() noexcept {
                return *current;
            }

            data_iterator& operator=(const data_iterator& other) noexcept = default;

            data_iterator& operator=(data_iterator&& other) noexcept = default;

            bool operator==(const data_iterator& other) const noexcept {
                return this->current == other.current;
            }

            bool operator!=(const data_iterator& other) const noexcept {
                return this->current != other.current;
            }

        private:
            friend class local_file_packet;

            value_type* current = nullptr; // 迭代指针

            /**
             * @brief 用于local_file_packet封装数据
             * @param ptr 数据指针
             */
            explicit data_iterator(value_type* ptr) noexcept : current(ptr) {}
        };

        [[nodiscard]] data_iterator begin() const noexcept {
            return data_iterator(data.get());
        }

        [[nodiscard]] data_iterator end() const noexcept {
            return data_iterator{data.get()+header.original_file_size};
        }

    private:
        local_file_header header; // 文件头
        std::unique_ptr<uint8_t[]> data; // 文件内容
    };
}

#endif //LOCAL_FILE_PACKET_H
