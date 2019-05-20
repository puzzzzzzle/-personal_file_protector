/**************************************************
 * @FileName: base64_adaptor.h
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/

#ifndef PERSONAL_FILE_PROTECTOR_BASE64_ADAPTOR_H
#define PERSONAL_FILE_PROTECTOR_BASE64_ADAPTOR_H

#include <cstring>
#include "adaptor_base.h"

/**
 * base64
 * 实现
 *     ADAPTOR_MODE_STRING = 1,
 *     ADAPTOR_MODE_FILE = 2,
 *     ADAPTOR_MODE_DATE = 3,
 */
class Base64Adaptor : public AdaptorBase {
public:
    Base64Adaptor() : AdaptorBase() {
    }

    Base64Adaptor(WorkMode mode, const std::string &strValue) : AdaptorBase(mode, strValue) {
    }

    Base64Adaptor(const char *cstrData, size_t dataLen) : AdaptorBase(cstrData, dataLen) {
    }

public:
    Base64Adaptor *encodeToString();

    Base64Adaptor *encodeToFile(std::string fileName);

    Base64Adaptor *encodeToData(size_t len, char *outputData, size_t &outlen);

    Base64Adaptor *decodeToString();

    Base64Adaptor *decodeToFile(std::string fileName);

    Base64Adaptor *decodeToData(size_t len, char *outputData, size_t &outlen);

    static size_t base64Encode(const char *inStr, size_t inLen, char *outStr, size_t outLen);

    static size_t base64Decode(const char *inStr, size_t inLen, char *outStr, size_t outLen);

    static size_t base64Encode(std::istream in, const std::ostream, size_t &finishLen);

    static size_t base64Decode(std::istream in, const std::ostream, size_t &finishLen);

};

#endif //PERSONAL_FILE_PROTECTOR_BASE64_ADAPTOR_H
