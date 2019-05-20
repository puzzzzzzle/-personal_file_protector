//
// Created by tao on 19-3-25.
//

#include "AES_func_adaptor.h"
#include "log_init.h"

#include <cstring>
#include <string>
#include "adaptor_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sstream>
#include <iosfwd>
#define BUFF_SIZE_4K 4096

std::string printBinToStr(const unsigned char *uc_pMd, int len) {
    std::stringbuf resBuff;
    char tmp[5];
    int i = 0;
    for (i = 0; i < len; i++) {
        snprintf(tmp, sizeof(tmp), "%02x", uc_pMd[i]);
        resBuff.sputn(tmp, strlen(tmp));
    }
    return resBuff.str();
}

std::string printBinToStr(const char *uc_pMd, int len) {
    std::stringbuf resBuff;
    char tmp[5];
    int i = 0;
    for (i = 0; i < len; i++) {
        snprintf(tmp, sizeof(tmp), "%02x", uc_pMd[i]);
        resBuff.sputn(tmp, strlen(tmp));
    }
    return resBuff.str();
}


int checkFile(std::string fileStr) {
    namespace bf=boost::filesystem;
    bf::path file(fileStr);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        ERROR_LOG("file not exists :" << fileStr)
        return -1;
    }
    return 0;
}

int trunkFile(std::string fileStr) {
    namespace bf=boost::filesystem;
    bf::path file(fileStr);
    if ((!bf::exists(file)) || (!bf::is_regular(file))) {
        return 0;
    }
    bf::remove(file);
    return 0;
}

int aesDataToData(std::string keyStr, const char *inData, size_t inLen, char *outData, size_t &outlen, int type) {
    const unsigned char *key = (unsigned char *) keyStr.c_str();
    BIO *enc = nullptr, *out = nullptr;
    unsigned char iv[16] = {0};
    const unsigned char *inStr = (const unsigned char *) inData;

    EVP_CIPHER_CTX *ctx = nullptr;
    BUF_MEM *bptr = nullptr;

    OpenSSL_add_all_algorithms();
    out = BIO_new(BIO_s_mem());
    if (out == NULL) {
        ERROR_LOG("out null\n");
        return -1;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return -1;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return -1;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, type)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return -1;
    }

    enc = BIO_push(enc, out);
    BIO_write(enc, inStr, inLen);
    BIO_flush(enc);
    BIO_flush(out);
    BIO_get_mem_ptr(out, &bptr);
    if (bptr->length > outlen) {
        ERROR_LOG("out put mem is too small")
        return -1;
    }
    memcpy(outData, bptr->data, bptr->length);
    outlen = bptr->length;
    BIO_free_all(out);
    return 0;
}

int aesFileToFile(std::string keyStr, std::string inFile, std::string outFile, int type, size_t &curr_size) {
    const unsigned char *key = (unsigned char *) keyStr.c_str();
    BIO *enc = nullptr, *out_bio_file = nullptr;
    unsigned char iv[16] = {0};
    curr_size = 0;
    namespace bf=boost::filesystem;

    char buff[BUFF_SIZE_4K];
    EVP_CIPHER_CTX *ctx = nullptr;

    OpenSSL_add_all_algorithms();

    out_bio_file = BIO_new(BIO_s_file());
    if (checkFile(inFile)) {
        ERROR_LOG("in file not exists\n");
        return -1;
    }
    trunkFile(outFile);

    BIO_write_filename(out_bio_file, (void *) outFile.c_str());

    if (out_bio_file == NULL) {
        ERROR_LOG("out_bio_file null\n");
        return -1;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return -1;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return -1;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, type)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return -1;
    }

    enc = BIO_push(enc, out_bio_file);
    bf::ifstream file(inFile);

    while (!file.eof()) {
        file.read(buff, sizeof(buff));
        BIO_write(enc, buff, file.gcount());
        curr_size += file.gcount();
    }

    BIO_flush(enc);
    BIO_flush(out_bio_file);
    BIO_free_all(out_bio_file);
    return 0;
}

int aesFdToFd(std::string keyStr, int inFd, int outFd, int type, size_t &curr_size) {
    const unsigned char *key = (unsigned char *) keyStr.c_str();
    BIO *enc = nullptr, *out_bio_file = nullptr;
    unsigned char iv[16] = {0};
    curr_size = 0;
    namespace bf=boost::filesystem;

    char buff[BUFF_SIZE_4K];
    EVP_CIPHER_CTX *ctx = nullptr;

    OpenSSL_add_all_algorithms();

    out_bio_file = BIO_new(BIO_s_fd());

    out_bio_file = BIO_new_fd(outFd, BIO_NOCLOSE);

    if (out_bio_file == NULL) {
        ERROR_LOG("out_bio_file null\n");
        return -1;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return -1;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return -1;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, type)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return -1;
    }

    enc = BIO_push(enc, out_bio_file);
    int readLen = 0;
    while ((readLen = read(inFd, buff, sizeof(buff))) > 0) {
        BIO_write(enc, buff, readLen);
        curr_size += readLen;
    }

    BIO_flush(enc);
    BIO_flush(out_bio_file);
    BIO_free_all(out_bio_file);
    return 0;
}
