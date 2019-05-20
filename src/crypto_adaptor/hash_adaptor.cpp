/**************************************************
 * @FileName: MD5_adaptor.cpp
 * @Author: zhangtao(2359173906@qq.com)
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>      <mail>
 * zhangtao   19-3-15-下午10:51  1.0         create        2359173906@qq.com
**************************************************/

#include "hash_adaptor.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

std::string HashAdaptor::printHash(unsigned char *uc_pMd, int len) {
    std::stringbuf resBuff{};
    char tmp[5];
    int i = 0;
    for (i = 0; i < len; i++) {
        snprintf(tmp, sizeof(tmp), "%02x", uc_pMd[i]);
        resBuff.sputn(tmp, strlen(tmp));
    }
    return resBuff.str();
}

int HashAdaptor::md5_data() {
    unsigned char md[MD5_DIGEST_LENGTH];
    MD5((unsigned char *) data, dataLen, md);
    this->strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::md5_string() {
    unsigned char md[MD5_DIGEST_LENGTH];
    MD5((unsigned char *) value.c_str(), value.length(), md);
    this->strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::md5_file() {
    namespace bf=boost::filesystem;
    bf::path file(value);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        ERROR_LOG("file not exists :" << value)
        return -1;
    }

    char fileBuff[FILE_READ_BUFF]{};
    unsigned char md[MD5_DIGEST_LENGTH];
    bf::ifstream inFile(file);
    totalSize = bf::file_size(file);
    MD5_CTX ctx{};;
    MD5_Init(&ctx);
    while (!inFile.eof()) {
        inFile.read(fileBuff, sizeof(fileBuff));
        MD5_Update(&ctx, fileBuff, inFile.gcount());
        finishSize+=inFile.gcount();
    }
    MD5_Final(md, &ctx);
    this->strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

HashAdaptor *HashAdaptor::encode(CryptType eType) {
    this->cryptType = eType;
    if (eType == ADAPTOR_CRYPT_MD5) {
        switch (workMode) {
            case ADAPTOR_MODE_STRING:
                status = md5_string();
                break;
            case ADAPTOR_MODE_FILE:
                status = md5_file();
                break;
            case ADAPTOR_MODE_DATE:
                status = md5_data();
                break;
            case ADAPTOR_MODE_DATE_BASE64:
                //todo
                status = -10;
                break;
            default:
                status = -2;
        }
    }
    if (eType == ADAPTOR_CRYPT_SHA256) {
        switch (workMode) {
            case ADAPTOR_MODE_STRING:
                status = sha256_string();
                break;
            case ADAPTOR_MODE_FILE:
                status = sha256_file();
                break;
            case ADAPTOR_MODE_DATE:
                status = sha256_data();
                break;
            case ADAPTOR_MODE_DATE_BASE64:
                //todo
                status = -10;
                break;
            default:
                status = -2;
        }
    }

    return this;
}

int HashAdaptor::sha256_string() {
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) value.c_str(), value.length(), md);
    this->strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::sha256_file() {
    namespace bf=boost::filesystem;
    bf::path file(value);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        ERROR_LOG("file not exists :" << value)
        return -1;
    }

    char fileBuff[FILE_READ_BUFF]{};
    unsigned char md[SHA256_DIGEST_LENGTH];
    bf::ifstream inFile(file);
    totalSize = bf::file_size(file);

    SHA256_CTX ctx{};;
    SHA256_Init(&ctx);
    while (!inFile.eof()) {
        inFile.read(fileBuff, sizeof(fileBuff));
        SHA256_Update(&ctx, fileBuff, inFile.gcount());
        finishSize+=inFile.gcount();
    }
    SHA256_Final(md, &ctx);
    this->strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::sha256_data() {
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) data, dataLen, md);
    this->strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}
