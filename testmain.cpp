//
// Created by DELL on 25-9-25.
//

#include <iostream>
#include <fstream>

#include "include/utils/ms_dos_time.h"
#include "include/file_system/file_paths.h"
#include "include/packet/file_packet.h"
#include "include/packet/local_file_packet.h"

using namespace data_packet;

int main() {

    const auto metas = get_file_metas(R"(F:\Clioncxx\data_packet\测试文件夹)");

    std::vector<local_file_packet> packets;

    for (const auto& meta:metas) {
        std::cout<<std::format("name:{}\nsize:{}\ndate:{}\ntime:{}",std::get<0>(meta),std::get<1>(meta),std::get<2>(meta),std::get<3>(meta))<<std::endl;
        packets.emplace_back(meta);
    }

    auto packet = make_file_packet(std::move(packets),0);

    write_file(R"(F:\Clioncxx\data_packet\目标文件夹\mypacket.lhxc)",packet);

    packet = read_file(R"(F:\Clioncxx\data_packet\目标文件夹\mypacket.lhxc)");

    for (const auto& local_packet:packet.local_file_packets) {
        std::cout<<std::format("name:{}\nsize:{}\ndate:{}\ntime:{}\n",local_packet.get_file_name(),local_packet.get_file_size(),local_packet.get_last_modified_date(),local_packet.get_last_modified_time())<<std::endl;
    }

    return 0;
}