//
// Created by tao on 19-3-23.
//

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


#include "log_init.h"


//void test() {
////    unsigned char key[32] = {1};
//    std::string keyString = "taotaotao";
//    unsigned const char *key = (unsigned const char *) (keyString.c_str());
//    unsigned char iv[16] = {0};
//    unsigned char inStr[]{"puzzzzzzle"};
//    int inLen = strlen((char *) inStr);
//    int encLen = 0;
//    int outlen = 0;
//    unsigned char encData[1024];
//
//    printf("source: %s\n", inStr);
//
//    //加密
//    EVP_CIPHER_CTX *ctx;
//    ctx = EVP_CIPHER_CTX_new();
//
//    EVP_CipherInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv, 1);
//    EVP_CipherUpdate(ctx, encData, &outlen, inStr, inLen);
//    encLen = outlen;
//    EVP_CipherFinal(ctx, encData + outlen, &outlen);
//    encLen += outlen;
//    EVP_CIPHER_CTX_free(ctx);
//
//    printf("for bin :%s\n", printBinToStr(encData, encLen).c_str());
//
//    //解密
//    int decLen = 0;
//    outlen = 0;
//    unsigned char decData[1024];
//    EVP_CIPHER_CTX *ctx2;
//    ctx2 = EVP_CIPHER_CTX_new();
//    EVP_CipherInit_ex(ctx2, EVP_aes_256_ecb(), NULL, key, iv, 0);
//    EVP_CipherUpdate(ctx2, decData, &outlen, encData, encLen);
//    decLen = outlen;
//    EVP_CipherFinal(ctx2, decData + outlen, &outlen);
//    decLen += outlen;
//    EVP_CIPHER_CTX_free(ctx2);
//
//    decData[decLen] = '\0';
//    printf("decrypt: %s\n", decData);
//}
void initssl() {
//    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
}

AESAdaptor *AESAdaptor::encodeDataToData(size_t len, char *outputData, size_t &outlen) {
    std::string ketStr = "taotaotao";
    const unsigned char *key = (unsigned char *) ketStr.c_str();
    BIO *enc = nullptr, *out = nullptr;
    unsigned char iv[16] = {0};
//    unsigned char inStr[]{"puzzzzzzle"};
//    std::string inStrStr="puzzzzzzle";
//    const unsigned char *inStr = (const unsigned char *) inStrStr.c_str();
    const unsigned char *inStr = (const unsigned char *) data;
    size_t inStrLen = this->dataLen;
//    const unsigned char *inStr = (const unsigned char *)value.c_str();
    INFO("value str :" << inStr);
    INFO("from origin bio enc bin:" << printBinToStr(data, inStrLen))

    EVP_CIPHER_CTX *ctx = nullptr;

    BUF_MEM *bptr = nullptr;

//    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    out = BIO_new(BIO_s_mem());
    if (out == NULL) {
        ERROR_LOG("out null\n");
        return this;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return this;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return this;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, 1)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return this;
    }
//    unsigned char encData[1024];

    enc = BIO_push(enc, out);
    BIO_write(enc, inStr, inStrLen);
    BIO_flush(enc);
    BIO_get_mem_ptr(out, &bptr);
    if (bptr->length > len) {
        ERROR_LOG("out put mem is too small")
        return this;
    }
    memcpy(outputData, bptr->data, bptr->length);
    outlen = bptr->length;
    BIO_free_all(out);
    INFO("from bio enc bin:" << printBinToStr(outputData, outlen))
    return this;
}

AESAdaptor *AESAdaptor::encodeFileToFile(std::string fileName) {
    return nullptr;
}

AESAdaptor *AESAdaptor::decodeDataToData(size_t len, char *outputData, size_t &outlen) {
    std::string ketStr = "taotaotao";
    const unsigned char *key = (unsigned char *) ketStr.c_str();
    BIO *enc = nullptr, *out = nullptr;
    unsigned char iv[16] = {0};
//    unsigned char inStr[]{"puzzzzzzle"};
//    std::string inStrStr="puzzzzzzle";
//    const unsigned char *inStr = (const unsigned char *) inStrStr.c_str();
    const unsigned char *inStr = (const unsigned char *) data;
    size_t inStrLen = this->dataLen;
//    const unsigned char *inStr = (const unsigned char *)value.c_str();
    INFO("value str :" << inStr);
    INFO("from origin bio enc bin:" << printBinToStr(data, inStrLen))

    EVP_CIPHER_CTX *ctx = nullptr;

    BUF_MEM *bptr = nullptr;

//    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    out = BIO_new(BIO_s_mem());
    if (out == NULL) {
        ERROR_LOG("out null\n");
        return this;
    }
    if ((enc = BIO_new(BIO_f_cipher())) == nullptr) {
        ERROR_LOG("Cipher New BIO Error\n");
        return this;
    }
    BIO_get_cipher_ctx(enc, &ctx);
    if (ctx == nullptr) {
        ERROR_LOG("ctx null\n");
        return this;
    }

    if (!EVP_CipherInit_ex(ctx, EVP_des_cbc(), NULL, key, iv, 0)) {
        ERROR_LOG("EVP_CipherInit_ex Error\n");
        return this;
    }
    unsigned char encData[1024];

    enc = BIO_push(enc, out);
    BIO_write(enc, inStr, inStrLen);
    BIO_flush(enc);
    BIO_get_mem_ptr(out, &bptr);
    if (bptr->length > 1024) {
        ERROR_LOG("out put mem is too small")
        return this;
    }
    memcpy(encData, bptr->data, bptr->length);
    outlen = bptr->length;
    BIO_free_all(out);
    INFO("from bio dec bin:" << printBinToStr(encData, outlen))
    return this;

}

AESAdaptor *AESAdaptor::decodeFileToFile(std::string fileName) {
    return this;
}

AESAdaptor *AESAdaptor::encodeStringToString() {
    this->data = this->value.c_str();
    this->dataLen = this->value.length();
    CharBuffHandle buff(value.length() * 2);
    size_t outLen = 0;
    encodeDataToData(buff.len, *buff, outLen);
    this->strResult = printBinToStr(*buff, outLen);
    return this;
}

AESAdaptor *AESAdaptor::decodeStringToString() {
    this->data = this->value.c_str();
    this->dataLen = this->value.length();
    CharBuffHandle buff(value.length() * 2);
    size_t outLen = 0;
    decodeDataToData(buff.len, *buff, outLen);
    (*buff)[outLen] = '\0';
    this->strResult = std::string(*buff);
    return this;
}
