//
// Created by Administrator on 2019/4/18.
//

#ifndef PERSONAL_FILE_PROTECTOR_PIP_COMMON_WIN_HPP
#define PERSONAL_FILE_PROTECTOR_PIP_COMMON_WIN_HPP

#include <windows.h>
#include <stdio.h>
#include "common_includes.h"


class PipAdaptor {
public:
    HANDLE hPip;
public:
    char pipGlobalBuff[4096]{};
public:
    ~PipAdaptor(){
        pip_close();
    }
public:
    int pip_creat_cli(const char *pip_file) {
        BOOL bRet;
        //WaitNamedPipe等待一个命名管道实例供自己使用
        bRet = WaitNamedPipe(pip_file, NMPWAIT_USE_DEFAULT_WAIT);
        if (bRet == 0) {
            printf("wait named pipe failed ! \n");
            return 1;
        }
        //CreateFile与服务端建立连接
        hPip = CreateFile(pip_file, GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hPip) {
            printf("INVALID_HANDLE_VALUE == hPip \n");
            hPip = NULL;
            return -2;
        }

        return 0;
    }

    int pip_creat_svr_wait(const char *pip_file) {

        //创建一个命名管道，在windows中\代表zhuan'yi两个\\代表一个
        hPip = CreateNamedPipeA(pip_file,
                                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                                PIPE_TYPE_BYTE, 1, 1024, 1024, 0, NULL);
        //检查是否创建成功
        if (hPip == INVALID_HANDLE_VALUE) {
            printf("create named pipe failed!\n");
        } else {
            printf("create named pipe success!\n");
        }
        //异步IO结构
        OVERLAPPED op;
        ZeroMemory(&op, sizeof(OVERLAPPED));
        //创建一个事件内核对象
        op.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        //等待一个客户端进行连接
        ConnectNamedPipe(hPip, &op);
        //当有客户端进行连接时，事件变成有信号的状态
        if (WaitForSingleObject(op.hEvent, INFINITE) == 0) {
            printf("client connect success!\n");
        } else {
            printf("client connect failed!\n");
        }
        return 0;
    }

    int pip_close() {
        //通信完之后，断开连接
        DisconnectNamedPipe(hPip);
        //关闭管道
        CloseHandle(hPip);
        return CloseHandle(hPip);
    }

    int pip_read(char *buff, DWORD buffLen) {
        DWORD nLen;
        //从管道读取信息
        ReadFile(hPip, buff, buffLen, &nLen, NULL);
        buff[nLen] = '\0';
        DEBUG("[read] read from pip:" << buff)
        return nLen;
    }

    int pip_read() {
        DWORD nLen;
        //从管道读取信息
        ReadFile(hPip, pipGlobalBuff, sizeof(pipGlobalBuff), &nLen, NULL);
        pipGlobalBuff[nLen] = '\0';
        DEBUG("[read] read from pip:" << pipGlobalBuff)

        return atoi(pipGlobalBuff);
    }

    int pip_write(const char *writeStr, DWORD writeStrLen) {
        DWORD nLen;
        //写入信息
        WriteFile(hPip, writeStr, writeStrLen, &nLen, NULL);
//        DEBUG("[write] write to pip:" << writeStr)
        return nLen;
    }

    int pip_write(int value) {
        DWORD nLen;
        snprintf(pipGlobalBuff, sizeof(pipGlobalBuff),"%d",value);
        //写入信息
        WriteFile(hPip, pipGlobalBuff, strlen(pipGlobalBuff), &nLen, NULL);
        DEBUG("[write] write to pip:" << pipGlobalBuff)
        return nLen;
    }
};

#endif //PERSONAL_FILE_PROTECTOR_PIP_COMMON_WIN_HPP
