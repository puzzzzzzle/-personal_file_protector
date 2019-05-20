/**************************************************
 * @FileName: MD5_adaptor.cpp
 * @Author: zhangtao(2359173906@qq.com)
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>      <mail>
 * zhangtao   19-3-15-下午10:51  1.0         create        2359173906@qq.com
**************************************************/

#include "hash_adaptor.hpp"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#define FILE_READ_BUFF 4096     //4k

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
    MD5((unsigned char *) m_cstrData, m_idataLen, md);
    this->m_strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::md5_string() {
    unsigned char md[MD5_DIGEST_LENGTH];
    MD5((unsigned char *) m_strValue.c_str(), m_strValue.length(), md);
    this->m_strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::md5_file() {
    namespace bf=boost::filesystem;
    bf::path file(m_strValue);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        ERROR("file not exists :" << m_strValue)
        return -1;
    }

    char fileBuff[FILE_READ_BUFF]{};
    unsigned char md[MD5_DIGEST_LENGTH];
    bf::ifstream inFile(file);

    MD5_CTX ctx{};;
    MD5_Init(&ctx);
    while (!inFile.eof()) {
        inFile.read(fileBuff, sizeof(fileBuff));
        MD5_Update(&ctx, fileBuff, inFile.gcount());
    }
    MD5_Final(md, &ctx);
    this->m_strResult = printHash(md, MD5_DIGEST_LENGTH);
    return 0;
}

HashAdaptor *HashAdaptor::encode(HashType eType) {
    this->m_eType = eType;
    if (eType == ADAPTOR_MD5) {
        switch (m_eMode) {
            case ADAPTOR_STRING_HASH:
                m_iStatus = md5_string();
                break;
            case ADAPTOR_FILE_HASH:
                m_iStatus = md5_file();
                break;
            case ADAPTOR_DATE_HASH:
                m_iStatus = md5_data();
                break;
            default:
                m_iStatus = -2;
        }
    }
    if (eType == ADAPTOR_SHA256) {
        switch (m_eMode) {
            case ADAPTOR_STRING_HASH:
                m_iStatus = sha256_string();
                break;
            case ADAPTOR_FILE_HASH:
                m_iStatus = sha256_file();
                break;
            case ADAPTOR_DATE_HASH:
                m_iStatus = sha256_data();
                break;
            default:
                m_iStatus = -2;
        }
    }

    return this;
}

int HashAdaptor::sha256_string() {
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) m_strValue.c_str(), m_strValue.length(), md);
    this->m_strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::sha256_file() {
    namespace bf=boost::filesystem;
    bf::path file(m_strValue);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        ERROR("file not exists :" << m_strValue)
        return -1;
    }

    char fileBuff[FILE_READ_BUFF]{};
    unsigned char md[SHA256_DIGEST_LENGTH];
    bf::ifstream inFile(file);

    SHA256_CTX ctx{};;
    SHA256_Init(&ctx);
    while (!inFile.eof()) {
        inFile.read(fileBuff, sizeof(fileBuff));
        SHA256_Update(&ctx, fileBuff, inFile.gcount());
    }
    SHA256_Final(md, &ctx);
    this->m_strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}

int HashAdaptor::sha256_data() {
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *) m_cstrData, m_idataLen, md);
    this->m_strResult = printHash(md, SHA256_DIGEST_LENGTH);
    return 0;
}
