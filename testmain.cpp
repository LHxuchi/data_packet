//
// Created by DELL on 25-9-25.
//

#include <iostream>
#include <fstream>

#include "include/utils/CRC.h"
#include "include/utils/ms_dos_time.h"
#include "include/file_system/file_paths.h"

using namespace data_packet;

int main() {

    const auto metas = get_file_metas(R"(F:\Clioncxx\data_packet\utils)");

    for (const auto& meta:metas) {
        std::cout<<std::format("name:{}\nsize:{}\ndate:{}\ntime:{}",std::get<0>(meta),std::get<1>(meta),std::get<2>(meta),std::get<3>(meta))<<std::endl;
    }

    return 0;
}