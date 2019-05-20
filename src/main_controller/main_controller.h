//
// Created by Administrator on 2019/4/15.
//

#ifndef PERSONAL_FILE_PROTECTOR_MAIN_CONTROLLER_H
#define PERSONAL_FILE_PROTECTOR_MAIN_CONTROLLER_H

#include <string>
#include "file_controller_adaptor.h"
#include "web_adaptor.h"
#include "file_controller_adaptor.h"
#include "get_usr_passwd.h"
#include "file_key_controller.h"
#include <map>

#ifdef __CYGWIN__

#include "pip_common_win.hpp"

#else
#include "pip_common_unix.hpp"
#endif
#define INIT_FILE "./sysdata/init.data"
#define PIPE_FILE_NAME  "./sysdata/pipe.data"

//#define PIP_FILE "\\\\.\\pipe\\persoal_file_pip1"
//#define PIP_SIZE_FILE "\\\\.\\pipe\\persoal_file_pip_size"
extern char PIP_FILE[1024];
enum OPERATIONS {
    NONE = 0,
    REGISTER,
    LOGIN,
    LS,
    MV,
    CP,
    RM,
    ADD,
    DEC,
    ADD_KEY ,
    DEL_KEY = 10,
    SYNC,
    LS_KEY,
    START_WEB,
    STOP_WEB,
    STATUS_WEB,
    QUIT,
};

class MainController {
public:
    MainController();

public:
    FileKeyTable *fileKeyTable = FileKeyTable::Instance;
    UserKeyTable *userKeyTable = UserKeyTable::Instance;
    MainFileController *fileController = MainFileController::Instance;
    WebController *webController = WebController::Instance;

public:
    int userKey{1};
    PipAdaptor pipAdaptor;
//    PipAdaptor pipSizeAdaptor;
    std::mutex pipLock{};
    int status{-1};
public:
    static std::map<std::string, int> *STR_TO_ENUM;
public:
    int checkInstall();

    int initAndLogin(std::string name, std::string passwd);

//    template<class... ARGS>
    int processCmd(std::string cmd, std::string para1, std::string para2, std::string para3, int &outInt);

    int processCmd(std::string yaml, int &outInt);

//    template<class... ARGS>
//    int processCmd(int cmd, std::string para1, std::string para2, std::string para3, int &out, ARGS... args);

    int startPipSvrAndWait();

public:
    int clear(std::string key) {

        return 0;
    }

    int clearAll() {

        return 0;
    }

    int sync() {
        int iRet{};
        int iResult{};
        iRet = fileKeyTable->savrKeyTab();
        if (iRet) {
            ERROR_LOG("err happen when save tab")
            iResult = iRet;
        }
        iRet = userKeyTable->savrKeyTab();
        if (iRet) {
            ERROR_LOG("err happen when save tab")
            iResult = iRet;
        }
        iRet = fileController->saveTable();
        if (iRet) {
            ERROR_LOG("err happen when save tab")
            iResult = iRet;
        }
        return iResult;
    }
};

#endif //PERSONAL_FILE_PROTECTOR_MAIN_CONTROLLER_H
