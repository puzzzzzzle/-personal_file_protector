//
// Created by Administrator on 2019/4/22.
//
#include "pip_common_win.hpp"
#include "main_controller.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <protocol_adaptor.hpp>

int main(int argc, char *argv[]) {
    srand(time(NULL));
    PipAdaptor pipe;
    CommonReq req;
    CommonRsp rsp;

    char buff[2048];
    namespace bf=boost::filesystem;
    bf::ifstream in;
    in.open(PIPE_FILE_NAME);
    if (!in) {
        ERROR_LOG("cannot open pipe file name")
        return -1;
    }
    std::string name;
    in >> name;
    INFO("pipe is " << name)
    pipe.pip_creat_cli(name.c_str());
    while (true) {
        req.loadFromCin();
        std::string pipenc = req.enc();
        pipe.pip_write(pipenc.c_str(), pipenc.size());
        if (req.cmd == "bye") {
            break;
        }
        pipe.pip_read(buff, sizeof(buff));
        rsp.dec(std::string(buff));
        INFO("receive:\n"<<rsp.toYamlStr())
//        if(req.cmd=="ls"){
//            YAML::Node ls = YAML::Load(rsp.data);
//            for(const auto &i:ls){
//                EncINode temp;
//                temp<<i;
//                printf("\n%s\t%d\n",temp.encName.c_str(),temp.encFileType);
//            }
//        }
    }
    return 0;
}