//
// Created by Administrator on 2019/4/15.
//

#include "main_controller.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include "hash_adaptor.h"
#include "protocol_adaptor.hpp"

char PIP_FILE[1024];

std::map<std::string, int> *MainController::STR_TO_ENUM = new std::map<std::string, int>();

int MainController::checkInstall() {
    namespace bf=boost::filesystem;

    bf::path initFilePath(INIT_FILE);
    if (!bf::exists(initFilePath)) {
        bf::ofstream initOf;
        initOf.open(initFilePath);
        if (!initOf) {
            ERROR_LOG("open init  file err!")
            return -2;
        }
        initOf << "init finish" << std::endl;
        return 1;
    }
    return 0;
}

int MainController::initAndLogin(std::string name, std::string passwd) {
    if (userKeyTable->loadKeyTab()) {
        ERROR_LOG("init user key table faild!")
        return -11;
    }

    userKey = userKeyTable->checkLogin(name, passwd);
    if (userKey == -10) {
        ERROR_LOG("no such user")
        return 10;
    }
    if (userKey == -9) {
        ERROR_LOG("login passwd err")
        return -9;
    }
    if (userKey < 0) {
        ERROR_LOG("login fai;")
        return userKey;
    }
    fileKeyTable->userID = userKey;
    fileKeyTable->loadKeyTab();

    fileController->userID = userKey;
    fileController->loadTable();

    webController->status = 1;

    this->status = 0;
    INFO("user login success :" << name)
    return 0;
}

int MainController::startPipSvrAndWait() {
    DEBUG("svr start")
    pipAdaptor.pip_creat_svr_wait(PIP_FILE);
    return 0;
}

MainController::MainController() {
    (*STR_TO_ENUM)["REGISTER"] = 1;
    (*STR_TO_ENUM)["LOGIN"] = 2;
    (*STR_TO_ENUM)["LS"] = 3;
    (*STR_TO_ENUM)["MV"] = 4;
    (*STR_TO_ENUM)["CP"] = 5;
    (*STR_TO_ENUM)["RM"] = 6;
    (*STR_TO_ENUM)["ADD"] = 7;
    (*STR_TO_ENUM)["DEC"] = 8;
    (*STR_TO_ENUM)["ADD_KEY"] = 9;
    (*STR_TO_ENUM)["SYNC"] = 11;

    (*STR_TO_ENUM)["register"] = 1;
    (*STR_TO_ENUM)["login"] = 2;
    (*STR_TO_ENUM)["ls"] = 3;
    (*STR_TO_ENUM)["mv"] = 4;
    (*STR_TO_ENUM)["cp"] = 5;
    (*STR_TO_ENUM)["rm"] = 6;
    (*STR_TO_ENUM)["add"] = 7;
    (*STR_TO_ENUM)["dec"] = 8;
    (*STR_TO_ENUM)["add_key"] = 9;
    (*STR_TO_ENUM)["sync"] = 11;

    char buff[128]{R"(\\.\pipe\)"};
    for (int i = 9; i < 19; ++i) {
        buff[i] = randChar();
    }
    buff[23] = 0;
    INFO("pip file is " << buff);
    strncpy(PIP_FILE, buff, sizeof(PIP_FILE));
    namespace bf=boost::filesystem;
    bf::ofstream out;
    out.open(PIPE_FILE_NAME, std::ios::trunc | std::ios::out);
    if (!out) {
        ERROR_LOG("cannot open pipe file")
    }
    out << PIP_FILE;
    //auto close out
}

//template<class... ARGS>
int MainController::processCmd(std::string cmd, std::string para1, std::string para2, std::string para3, int &outInt) {
    CommonRsp rsp;
    int iRet{};

    DEBUG("cmd is \t" << cmd << "\t" << para1 << "\t" << "\t" << para2 << "\t" << para3)
    if (status) {
        auto cmdi = (*STR_TO_ENUM)[cmd];
        if (cmdi != REGISTER
            && cmdi != LOGIN) {
            ERROR_LOG("cannot do more while not login")
            iRet = -1000;
            rsp.result = iRet;
            rsp.data = "";
            std::string rspStr = rsp.enc();
            if (rspStr != "") {
                pipAdaptor.pip_write(rspStr.c_str(), rspStr.length());
            } else {
                ERROR_LOG("err happen when enc ")
            }
            return -1000;
        }
    }

    std::map<std::string, EncINode> nodes;
    EncINode *origin;
    EncINode dest;
    std::string delName;
    size_t currSize{};
    std::ostringstream oStr;

    switch ((*STR_TO_ENUM)[cmd]) {
        case REGISTER://done
            if (userKeyTable->loadKeyTab()) {
                ERROR_LOG("init user key table faild!")
                iRet = -1002;
                break;
            }
            iRet = userKeyTable->registor(para1, para2);
            userKeyTable->savrKeyTab();
            break;
        case LOGIN://done
            iRet = initAndLogin(para1, para2);
            break;
        case LS://done
            nodes.clear();
            fileController->selectFileWithDirByName(para1, &nodes);
            iRet = 0;
            rsp.data=MainFileController::writeList(&nodes);
            if(rsp.data.empty()){
                rsp.data="none";
            }
            break;
        case MV://done
            origin = nullptr;
            origin = fileController->selectByName(para1);
            if (origin == nullptr) {
                iRet=-1003;
                ERROR_LOG("no file named " << para1)
                break;
            }
            origin->encName = para2;
            iRet=0;
            break;
        case CP://done
            origin = nullptr;
            origin = fileController->selectByName(para1);
            if (origin == nullptr) {
                iRet=-1003;
                ERROR_LOG("no file named " << para1)
                break;
            }
            dest.copy(*origin);
            dest.encName = para2;
            iRet = fileController->table.insert(dest);
            if(iRet>0){
                iRet=0;
            }
            break;
        case RM://done
            origin = nullptr;
            origin = fileController->selectByName(para1);
            delName = origin->encCryptFileName;
            iRet = fileController->remove(para1);
            clear(delName);
            break;
        case ADD://done
            currSize = 1;
            iRet = fileController->encAndInsertFile(currSize, userKeyTable->randKey(), para1, para2);
            break;
        case DEC:
            currSize = 0;
            origin = nullptr;
            origin = fileController->selectByName(para1);
            if(origin== nullptr){
                iRet =-100;
                break;
            }
            iRet = fileController->decToFile(currSize, origin->encFileKey, origin->encCryptFileName, para2);
            break;
        case ADD_KEY://done
            iRet = fileKeyTable->insertKey(para1);
            break;
        case SYNC://done
            iRet = sync();
            break;
        default:
            ERROR_LOG("not exits cmd\t" << cmd)
            iRet = -1001;
            break;
    }
    rsp.result=iRet;
    INFO("write back \n"<<rsp.toYamlStr())
    std::string rspStr = rsp.enc();
//    if (!rspStr.empty()) {
////        pipAdaptor.pip_write(rspStr.c_str(), rspStr.length());
//    } else {
//        ERROR_LOG("err happen when enc ")
//
//    }
    pipAdaptor.pip_write(rspStr.c_str(), rspStr.length());
    return iRet;
}

int MainController::processCmd(std::string yaml, int &outInt){
    CommonReq req;
    req.dec(yaml);
    return processCmd(req.cmd,req.para1,req.para2,req.para3,outInt);
}