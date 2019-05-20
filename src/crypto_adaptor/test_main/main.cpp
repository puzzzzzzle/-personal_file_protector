/**************************************************
 * @FileName: main.cpp
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/

#include "common_includes.h"
#include "init_tests_yaml.h"

extern TestNodeTab tab;

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
#include "AES_adaptor.h"
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
#include "AES_func_adaptor.h"

#include "test.hpp"

void test() {
//    unsigned char key[32] = {1};
    std::string keyString = "taotaotao";
    unsigned const char *key = (unsigned const char *) (keyString.c_str());
    unsigned char iv[16] = {0};
    std::string inStrStr = "puzzzzzzle";
//    const unsigned char *inStr = (const unsigned char *)inStrStr.c_str();
    unsigned char inStr[1024]{};
    memcpy(inStr, inStrStr.c_str(), inStrStr.length());
    int inLen = strlen((char *) inStr);
    int encLen = 0;
    int outlen = 0;
    unsigned char encData[1024];

    printf("source: %s\n", inStr);

    //加密
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, 1);
    EVP_CipherUpdate(ctx, encData, &outlen, inStr, inLen);
    encLen = outlen;
    EVP_CipherFinal(ctx, encData + outlen, &outlen);
    encLen += outlen;
    EVP_CIPHER_CTX_free(ctx);

    printf("for bin :%s\n", printBinToStr(encData, encLen).c_str());

    //解密
    int decLen = 0;
    memset(iv, 0, sizeof(iv));
    outlen = 0;
    unsigned char decData[1024];
    EVP_CIPHER_CTX *ctx2;
    ctx2 = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx2, EVP_des_cbc(), NULL, key, iv, 0);
    EVP_CipherUpdate(ctx2, decData, &outlen, encData, encLen);
    decLen = outlen;
    EVP_CipherFinal(ctx2, decData + outlen, &outlen);
    decLen += outlen;
    EVP_CIPHER_CTX_free(ctx2);

    decData[decLen] = '\0';
    printf("decrypt: %s\n", decData);
}

void test_bio() {
    std::string ketStr = "taotaotao";
    const unsigned char *key = (unsigned char *) ketStr.c_str();
    BIO *enc = nullptr, *out = nullptr;
    unsigned char iv[16] = {0};
    std::string inStrStr = "puzzzzzzle";
    const unsigned char *inStr = (const unsigned char *) inStrStr.c_str();
    INFO("main value str :" << inStr);

    EVP_CIPHER_CTX *ctx = nullptr;

    BUF_MEM *bptr = nullptr;

//    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    out = BIO_new(BIO_s_mem());
    if (out == NULL) {
        ERROR_LOG("out null\n");
        return;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, 1)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return;
    }
    unsigned char encData[1024];

    enc = BIO_push(enc, out);
    BIO_write(enc, inStr, strlen((char *) inStr));
    BIO_flush(enc);
    BIO_get_mem_ptr(out, &bptr);
    if (bptr->length > 1024) {
        ERROR_LOG("out put mem is too small")
        return;
    }
    memcpy(encData, bptr->data, bptr->length);
    int outlen = bptr->length;
    BIO_free_all(out);
    INFO("from main bio enc bin:" << printBinToStr(encData, outlen))
    return;
}

void test2() {
    std::string ketStr = "taotaotao";
    const unsigned char *key = (unsigned char *) ketStr.c_str();
    BIO *enc = nullptr, *out = nullptr;
    unsigned char iv[16] = {0};
//    unsigned char inStr[]{"puzzzzzzle"};
    std::string inStrStr = "puzzzzzzle";
    const unsigned char *inStr = (const unsigned char *) inStrStr.c_str();
//    const unsigned char *inStr = (const unsigned char *)data;

//    const unsigned char *inStr = (const unsigned char *)value.c_str();
    INFO("value str :" << inStr);

    EVP_CIPHER_CTX *ctx = nullptr;

    BUF_MEM *bptr = nullptr;

//    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    out = BIO_new(BIO_s_mem());
    if (out == NULL) {
        ERROR_LOG("out null\n");
        return;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, 1)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return;
    }
    unsigned char encData[1024];

    enc = BIO_push(enc, out);
    BIO_write(enc, inStr, strlen((const char *) inStr));
    BIO_flush(enc);
    BIO_get_mem_ptr(out, &bptr);
    if (bptr->length > 1024) {
        ERROR_LOG("out put mem is too small")
        return;
    }
    memcpy(encData, bptr->data, bptr->length);
    int outlen = bptr->length;
    BIO_free_all(out);
    INFO("2from bio enc bin:" << printBinToStr(encData, outlen))
    return;
}

void test_bio_func() {
    char encData[1024];
    char decData[1024];
    std::string origin = "puzzzzzzle";
    std::string key = "taotaotao";
    size_t  outLen =  sizeof(encData);
    aesDataToData(key,origin.c_str(),origin.length(),encData,outLen,1);
    size_t  decOutLen =  sizeof(decData);
    aesDataToData(key,encData,outLen,decData,decOutLen,0);
    decData[decOutLen]='\0';
    INFO("finish："<<decData)

}

void test_bio_file_func() {
    std::string file = "configure/boost_log_settings.ini";
    std::string encFile = "configure/boost_log_settings.ini.enc";
    std::string decFile = "configure/boost_log_settings.ini.dec";
    std::string key = "taotaotao";
    size_t curr =0;
    aesFileToFile(key,file,encFile,1,curr);
    aesFileToFile(key,encFile,decFile,0,curr);

}

int main(int argc, char **argv) {
    if (log_init()) {
        return -1;
    }
    if (load_test()) {
        return -2;
    }
    testing::InitGoogleTest(&argc, argv);
    INFO("finish")

    test();
    test_bio();
    test2();
    return RUN_ALL_TESTS();

//    test_bio_func();
//    test_bio_file_func();
//
//    main_test_base64();
    return 0;
}