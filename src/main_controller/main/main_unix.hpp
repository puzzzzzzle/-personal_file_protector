//
// Created by Administrator on 2019/4/18.
//

#ifndef PERSONAL_FILE_PROTECTOR_MAIN_UNIX_HPP
#define PERSONAL_FILE_PROTECTOR_MAIN_UNIX_HPP
#include "common_includes.h"
#include "main_controller.h"

int main_diy(int argc, char **argv) {
    MainController mainController;
    char buff[1024]{};
    while (scanf("%s",buff)){
        DEBUG("read :"<<buff)
        if (strcmp("bye",buff) != 0){
            break;
        }
    }

}
#endif //PERSONAL_FILE_PROTECTOR_MAIN_UNIX_HPP
