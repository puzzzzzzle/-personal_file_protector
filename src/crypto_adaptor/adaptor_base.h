/**************************************************
 * @FileName: adaptor_base.h
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/


#ifndef PERSONAL_FILE_PROTECTOR_ADAPTOR_BADE_H
#define PERSONAL_FILE_PROTECTOR_ADAPTOR_BADE_H

#include "work_mode.h"
#include "cstring"
#include <string>
#include <sstream>

#define FILE_READ_BUFF 4096     //4k
typedef unsigned long int __uintmax_t;

class CharBuffHandle {
public:
    char *buff{};
    size_t len{};
public:
    CharBuffHandle(size_t len) {
        this->len = len;
        this->buff = (char *) malloc(len);
    }

    CharBuffHandle() = delete;

    CharBuffHandle(CharBuffHandle &) = delete;

    CharBuffHandle(CharBuffHandle &&) = delete;

    void *operator new(size_t) = delete;

    void *operator new[](size_t) = delete;

    void operator delete(void *) = delete;

    void operator delete[](void *) = delete;

    ~CharBuffHandle() {
        free(buff);
    }

    char *operator*() {
        return buff;
    }
};

class AdaptorBase {
protected:
    WorkMode workMode{ADAPTOR_MODE_MAX};
    CryptType cryptType{ADAPTOR_MAX};
    const std::string value{};
    size_t dataLen{};
    const char *data{};
    int status{-1};
    std::string strResult{""};

    __uintmax_t totalSize{0};
    __uintmax_t finishSize{0};

public:
    virtual size_t getTotalSize() {
        return totalSize;
    }

    virtual size_t getFinishSize() {
        return finishSize;
    }

public:
    AdaptorBase() {
        workMode = ADAPTOR_MODE_NONE;
    }

    AdaptorBase(WorkMode eMode, const std::string &strValue) : workMode(eMode), value(strValue) {

    }

    AdaptorBase(const char *cstrData, size_t iLen) : dataLen(iLen), data(cstrData) {
        workMode = ADAPTOR_MODE_DATE;
    }

public:
    virtual int isWrong() {
        return status;
    }

    /**
     * 为了减少空指针错误,这里不会回nullptr,而是错误回""
     * @return
     */
    virtual std::string getResult() {
        return strResult;
    }

    std::string printBinToStr(const unsigned char *uc_pMd, int len) {
        std::stringbuf resBuff{};
        char tmp[5];
        int i = 0;
        for (i = 0; i < len; i++) {
            snprintf(tmp, sizeof(tmp), "%02x", uc_pMd[i]);
            resBuff.sputn(tmp, strlen(tmp));
        }
        return resBuff.str();
    }

    std::string printBinToStr(const char *uc_pMd, int len) {
        std::stringbuf resBuff{};
        char tmp[5];
        int i = 0;
        for (i = 0; i < len; i++) {
            snprintf(tmp, sizeof(tmp), "%02x", uc_pMd[i]);
            resBuff.sputn(tmp, strlen(tmp));
        }
        return resBuff.str();
    }
};

#endif //PERSONAL_FILE_PROTECTOR_ADAPTOR_BADE_H
