/**************************************************
 * @FileName: base64_adaptor.cpp
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/


#include "base64_adaptor.h"

#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "common_funcs.h"
#include "boost/filesystem.hpp"

namespace bf=boost::filesystem;

size_t Base64Adaptor::base64Encode(const char *inStr, size_t inLen, char *outStr, size_t outLen) {
    BIO *b64, *bio;
    BUF_MEM *bptr = nullptr;
    size_t size = 0;

    if (inStr == nullptr || outStr == nullptr) {
        ERROR_LOG("int out can not be null")
        return -1;
    }

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, inStr, inLen);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    if (bptr->length > outLen) {
        ERROR_LOG("out put mem is too small")
        return 3;
    }
    memcpy(outStr, bptr->data, bptr->length);
    outStr[bptr->length] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}

size_t Base64Adaptor::base64Decode(const char *inStr, size_t inLen, char *outStr, size_t outLen) {
    BIO *b64, *bio;
    size_t size = 0;

    if (inStr == nullptr || outStr == nullptr) {
        ERROR_LOG("int out can not be null")
        return -1;
    }

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bio = BIO_new_mem_buf(inStr, inLen);
    bio = BIO_push(b64, bio);

    size = BIO_read(bio, outStr, inLen);
    if (size > outLen) {
        ERROR_LOG("out put mem is too small")
        return 3;
    }
    outStr[size] = '\0';

    BIO_free_all(bio);
    return size;
}

Base64Adaptor *Base64Adaptor::encodeToData(size_t len, char *outputData, size_t &outlen) {
    if (workMode == ADAPTOR_MODE_STRING) {
        outlen = base64Encode(value.c_str(), value.length(), outputData, len);
        if (outlen > 0) {
            status = 0;
        }
    }
    if (workMode == ADAPTOR_MODE_DATE) {
        outlen = base64Encode(data, dataLen, outputData, len);
        if (outlen > 0) {
            status = 0;
        }
    }
    if (workMode == ADAPTOR_MODE_FILE) {
        //todo
    }
    return this;
}

Base64Adaptor *Base64Adaptor::decodeToData(size_t len, char *outputData, size_t &outlen) {
    if (workMode == ADAPTOR_MODE_STRING) {
        outlen = base64Decode(value.c_str(), value.length(), outputData, len);
        if (outlen > 0) {
            status = 0;
        }
    }
    if (workMode == ADAPTOR_MODE_DATE) {
        outlen = base64Decode(data, dataLen, outputData, len);
        if (outlen > 0) {
            status = 0;
        }
    }
    if (workMode == ADAPTOR_MODE_FILE) {
        //todo
    }
    return this;
}

Base64Adaptor *Base64Adaptor::encodeToString() {
    if (workMode == ADAPTOR_MODE_STRING) {
        CharBuffHandle buff(value.length() * 2);
        size_t outlen = base64Encode(value.c_str(), value.length(), *buff, buff.len);
        if (outlen > 0) {
            status = 0;
            strResult += *buff;
        }
    }
    if (workMode == ADAPTOR_MODE_DATE) {
        CharBuffHandle buff(value.length() * 2);
        size_t outlen = base64Encode(data, dataLen, *buff, buff.len);
        if (outlen > 0) {
            status = 0;
            strResult += *buff;
        }
    }
    if (workMode == ADAPTOR_MODE_FILE) {
        //todo
    }
    return this;
}

Base64Adaptor *Base64Adaptor::decodeToString() {
    if (workMode == ADAPTOR_MODE_STRING) {
        CharBuffHandle buff(value.length() * 2);
        size_t outlen = base64Decode(value.c_str(), value.length(), *buff, buff.len);
        if (outlen > 0) {
            status = 0;
            strResult += *buff;
        }

    }
    if (workMode == ADAPTOR_MODE_DATE) {
        CharBuffHandle buff(value.length() * 2);
        size_t outlen = base64Decode(data, dataLen, *buff, buff.len);
        if (outlen > 0) {
            status = 0;
            strResult += *buff;
        }
    }
    if (workMode == ADAPTOR_MODE_FILE) {
        //todo
    }
    return this;
}

size_t Base64Adaptor::base64Encode(std::istream in, const std::ostream, size_t &finishLen) {
    char buff[FILE_READ_BUFF]{};
    while (!in.eof()) {
        in.read(buff, sizeof(buff));

    }
    return 0;
}

