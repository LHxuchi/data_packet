#include "local_file_header.h"

#include <string.h>

data_packet::local_file_header::~local_file_header() {
    if (!file_name)
        delete[] file_name;
    file_name = nullptr;
}

const char * data_packet::local_file_header::get_file_name() const {
    return file_name;
}

void data_packet::local_file_header::set_file_name(const char *file_name, uint16_t length) {
    this->file_name = new char[length + 1];
    for (uint16_t i = 0; i < length; i++) {
        this->file_name[i] = file_name[i];
    }
    this->file_name[length] = '\0';
}
