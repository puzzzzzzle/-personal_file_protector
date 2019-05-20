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
#include "work_mode.h"
#include "adaptor_base.h"

/**
 * hash 适配器
 * 实现：
 *     ADAPTOR_MODE_STRING = 1,
 *     ADAPTOR_MODE_FILE = 2,
 *     ADAPTOR_MODE_DATE = 3,
 *     ADAPTOR_MODE_DATE_BASE64 = 4,
 */
class HashAdaptor : public AdaptorBase {
public:
    HashAdaptor() : AdaptorBase() {
    }

    HashAdaptor(WorkMode mode, const std::string &strValue) : AdaptorBase(mode, strValue) {
    }

    HashAdaptor(const char *cstrData, size_t dataLen) : AdaptorBase(cstrData, dataLen) {
    }

public:
    HashAdaptor *encode(CryptType eType);

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
