#ifndef PERSONAL_FILE_PROTECTOR_AES_ADAPTOR_H
#define PERSONAL_FILE_PROTECTOR_AES_ADAPTOR_H

#include <cstring>
#include <string>
#include "adaptor_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

/**
 * base64
 * 实现
 *     ADAPTOR_MODE_STRING = 1,
 *     ADAPTOR_MODE_FILE = 2,
 *     ADAPTOR_MODE_DATE = 3,
 */
class AESAdaptor : public AdaptorBase {
    std::string key{};
public:
    AESAdaptor() : AdaptorBase() {
    }

    AESAdaptor(std::string _key,WorkMode mode, const std::string &strValue) : AdaptorBase(mode, strValue) ,key(_key){
    }

    AESAdaptor(std::string _key,const char *cstrData, size_t dataLen) : AdaptorBase(cstrData, dataLen),key(_key) {
    }

public:

    AESAdaptor *encodeDataToData(size_t len, char *outputData, size_t &outlen);

    AESAdaptor *encodeFileToFile(std::string fileName);

    AESAdaptor *encodeStringToString();


    AESAdaptor *decodeDataToData(size_t len, char *outputData, size_t &outlen);

    AESAdaptor *decodeFileToFile(std::string fileName);

    AESAdaptor *decodeStringToString();

};

#endif //PERSONAL_FILE_PROTECTOR_AES_ADAPTOR_H
