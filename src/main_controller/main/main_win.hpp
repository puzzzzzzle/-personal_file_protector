//
// Created by tao on 19-1-17.
//
#include <protocol_adaptor.hpp>
#include "common_includes.h"
#include "main_controller.h"

#include "pip_common_win.hpp"


//int test_svr() {
//    DEBUG("svr start")
//
//    MainController mainController;
//
//    int len = 0;
//    HANDLE hPipSvr;
//
//    if (pip_creat_svr_wait(hPipSvr, PIP_FILE)) {
//        pip_close(hPipSvr);
//        return -1;
//    }
//
//    char readBuff[1024]{};
//    while (true) {
//        scanf("%s", readBuff);
//        len = pip_write(hPipSvr, readBuff, strlen(readBuff));
//        DEBUG("write to cli len:\t" << len)
//
//        len = pip_read(hPipSvr, readBuff, sizeof(readBuff));
//        DEBUG("read from cli" << readBuff << "\tlen:\t" << len)
//        if (strcmp(readBuff, "bye!") == 0) {
//            break;
//        }
//    }
//    pip_close(hPipSvr);
//    return 0;
//}

//int test_cli() {
//
//    DEBUG("cli start")
//    int len = 0;
//
//    HANDLE hPipSvr;
//
//    if (pip_creat_cli(hPipSvr, PIP_FILE)) {
//        pip_close(hPipSvr);
//        return -1;
//    }
//
//    char readBuff[1024]{};
//    while (true) {
//        len = pip_read(hPipSvr, readBuff, sizeof(readBuff));
//        DEBUG("read from svr" << readBuff << "\tlen:\t" << len)
//
//        scanf("%s", readBuff);
//        len = pip_write(hPipSvr, readBuff, strlen(readBuff));
//        DEBUG("write to svr len:\t" << len)
//
//        if (strcmp(readBuff, "bye!") == 0) {
//            len = pip_write(hPipSvr, readBuff, strlen(readBuff));
//            break;
//        }
//    }
//    pip_close(hPipSvr);
//    return 0;
//}

int main_diy(int argc, char **argv) {

    MainController mainController;
//    std::string in[4]{};
    int currSize{};
    char buff[4096]{};
    if (mainController.checkInstall() == 1) {
        INFO("init finish")
        return 0;
    }
    if (mainController.pipAdaptor.pip_creat_svr_wait(PIP_FILE)) {
        ERROR_LOG("init pip fail")
        return -1;
    }
    while (true) {
        mainController.pipAdaptor.pip_read(buff, sizeof(buff));
        CommonReq req;
        req.dec(std::string(buff));
        if (req.cmd=="bye") {
            INFO("bye!")
            break;
        }
        mainController.processCmd(std::string(buff),currSize);
    }
    mainController.sync();
    return 0;
}