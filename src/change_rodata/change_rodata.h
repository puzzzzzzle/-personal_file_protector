//
// Created by Administrator on 2019/4/14.
//

#ifndef PERSONAL_FILE_PROTECTOR_CHANGE_RODATA_H
#define PERSONAL_FILE_PROTECTOR_CHANGE_RODATA_H

#ifdef __CYGWIN__
#define SO_FILE "cygmain_passwd_dev.dll"
#define POS 4096
#else
#define SO_FILE "main_passwd_dev.so"
#define POS 1472
#endif

int rand_rodata();

#endif //PERSONAL_FILE_PROTECTOR_CHANGE_RODATA_H
