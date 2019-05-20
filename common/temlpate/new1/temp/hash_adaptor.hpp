/**************************************************
 * @FileName: md5_adaptor.h
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-15-下午10:51  1.0         create
**************************************************/

#ifndef PERSONAL_FILE_PROTECTOR_OPENSSL_MD5_H
#define PERSONAL_FILE_PROTECTOR_OPENSSL_MD5_H

#include "log_init.h"

enum WorkMode {
    ADAPTOR_NONE_HASH = 0,
    ADAPTOR_STRING_HASH,
    ADAPTOR_FILE_HASH,
    ADAPTOR_DATE_HASH,

    ADAPTOR_HASH_MAX = 10,
};
enum HashType {
    ADAPTOR_MD5 = 0,
    ADAPTOR_SHA256,

    ADAPTOR_MAX = 20,
};

/**
 * md5 hash 适配器
 */
class HashAdaptor {
private:
    WorkMode m_eMode{ADAPTOR_HASH_MAX};
    HashType m_eType{ADAPTOR_MAX};
    const std::string m_strValue{};
    size_t m_idataLen{};
    const char *m_cstrData{};
    int m_iStatus{-1};

    std::string m_strResult{};
public:
    HashAdaptor() {
        m_eMode = ADAPTOR_NONE_HASH;
    }

    HashAdaptor(WorkMode eMode, const std::string &strValue) : m_eMode(eMode), m_strValue(strValue) {

    }

    HashAdaptor(const char *cstrData, size_t iLen) :m_idataLen(iLen), m_cstrData(cstrData) {
        m_eMode = ADAPTOR_DATE_HASH;
    }

public:
    HashAdaptor *encode(HashType eType = ADAPTOR_SHA256);

    int isWrong() {
        return m_iStatus;
    }

    std::string getResult() {
        if (!isWrong()) {
            return m_strResult;
        }
        return nullptr;
    }

private:
    int md5_string();
    int md5_file();
    int md5_data();

private:
    int sha256_string();
    int sha256_file();
    int sha256_data();

private:
    std::string printHash(unsigned char *uc_pMd, int len);
};

#endif //PERSONAL_FILE_PROTECTOR_OPENSSL_MD5_H
