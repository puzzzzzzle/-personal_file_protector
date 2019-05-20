/**************************************************
 * @FileName: tests.cpp
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/
#include "common_includes.h"
#include "hash_adaptor.h"
#include "init_tests_yaml.h"
#include "work_mode.h"
#include "AES_adaptor.h"

extern TestNodeTab tab;

#ifdef __CYGWIN__   //win cygwin
Platform currPlat = PLAT_CYGWIN;
std::string currPlatName{"win cygwin"};
#else       //linux
Platform currPlat = PLAT_LINIX;
std::string currPlatName{"linux"};
#endif

//< start hash test
TEST(hash, all) {
    int allCount{}, wrongCount{};
    for (const auto &n:tab.nodes) {
        if (
                (n.platform == 0 || n.platform == currPlat)
                && (n.cryptType < ADAPTOR_CRYPT_HASH_MAX)
                ) {
            ++allCount;
            HashAdaptor adaptor(WorkMode(n.workMode), n.source);
            if (adaptor.encode(CryptType(n.cryptType))->isWrong()) {
                ERROR_LOG("Wrong :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
            }
            EXPECT_STREQ(n.rightResult.c_str(), adaptor.getResult().c_str());
            if (n.rightResult != adaptor.getResult()) {
                ERROR_LOG("wrong test is :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
                ++wrongCount;
            }
        }
    }
    INFO("finish crypt test,\tall:" << allCount << "\twrong:" << wrongCount)
}
//< finish test

TEST(base64, all) {
    int allCount{}, wrongCount{};
    for (const auto &n:tab.nodes) {
        if (
                (n.platform == 0 || n.platform == currPlat)
                && (n.cryptType >= ADAPTOR_CRYPT_BASE64_ENCODE)
                && (n.cryptType < ADAPTOR_CRYPT_BASE64_MAX)
                ) {
            ++allCount;
            HashAdaptor adaptor(WorkMode(n.workMode), n.source);
            if (adaptor.encode(CryptType(n.cryptType))->isWrong()) {
                ERROR_LOG("Wrong :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
            }
            EXPECT_STREQ(n.rightResult.c_str(), adaptor.getResult().c_str());
            if (n.rightResult != adaptor.getResult()) {
                ERROR_LOG("wrong test is :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
                ++wrongCount;
            }
        }
    }
    INFO("finish crypt test,\tall:" << allCount << "\twrong:" << wrongCount)
}

TEST(aes, all) {
    std::string key = "taotaotao";
    int allCount{}, wrongCount{};
    for (const auto &n:tab.nodes) {
        if (
                (n.platform == 0 || n.platform == currPlat)
                && (n.cryptType >= ADAPTOR_CRYPT_AES_ENCODE)
                && (n.cryptType < ADAPTOR_CRYPT_AES_MAX)
                ) {
            ++allCount;
            if (n.cryptType == ADAPTOR_CRYPT_AES_ENCODE) {
                //加密到data
                AESAdaptor aes_do(key, n.source.c_str(), n.source.length());
                CharBuffHandle do_buff(n.source.length() * 2);
                CharBuffHandle done_buff(n.source.length() * 2);
                size_t len;
                if (aes_do.encodeDataToData(do_buff.len, *do_buff, len)->isWrong()) {
                    ERROR_LOG("aes Wrong :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
                }

                INFO("from test bio enc bin:" << aes_do.printBinToStr(*do_buff, len))

                AESAdaptor aes_done(key, *do_buff, len);
                if (aes_done.decodeDataToData(done_buff.len, *done_buff, len)->isWrong()) {
                    ERROR_LOG("aes Wrong :" << n.testGroup << ":" << n.testName << "\tstr:\t" << n.source)
                }
                (*done_buff)[len]='\0';
                std::string done(*done_buff);
                INFO(done)
//                EXPECT_STREQ(n.rightResult.c_str(), );
            }
        }
    }
    INFO("aes finish crypt test,\tall:" << allCount << "\twrong:" << wrongCount)

}
