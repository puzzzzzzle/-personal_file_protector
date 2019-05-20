//
// Created by tao on 19-1-17.
//
#include "common_includes.h"
#include "hash_adaptor.h"

TEST(md5_string, hello) {
    std::string str("hello");
    std::string right("5d41402abc4b2a76b9719d911017c592");
    HashAdaptor adaptor(ADAPTOR_STRING_HASH, str);
    if (adaptor.encode(ADAPTOR_MD5)->isWrong()) {
        ERROR("!adaptor.encode(ADAPTOR_SHA256)->isWrong()")
        EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
    }
    EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
}

TEST(sha256_string, hello) {
    std::string str("hello");
    std::string right("2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824");
    HashAdaptor adaptor(ADAPTOR_STRING_HASH, str);
    if (adaptor.encode(ADAPTOR_SHA256)->isWrong()) {
        ERROR("!adaptor.encode(ADAPTOR_SHA256)->isWrong()")
        EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
    }
    EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
}
#ifdef WIN32

#else
TEST(md5_file, simple) {
    std::string str("/home/tao/tx/【录用函来啦，回复指南看这里】.pdf");
    std::string right("bd6c56ac4ef00cc86314bb3b9935e12b");
    HashAdaptor adaptor(ADAPTOR_FILE_HASH, str);
    if (adaptor.encode(ADAPTOR_MD5)->isWrong()) {
        ERROR("!adaptor.encode(ADAPTOR_SHA256)->isWrong()")
        EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
    }
    EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
}
TEST(sha256_file, simple) {
    std::string str("/home/tao/tx/【录用函来啦，回复指南看这里】.pdf");
    std::string right("c19c1d2826cd11681e4523202788c02123d8d605e6a59d57c4d67922e653cfd2");
    HashAdaptor adaptor(ADAPTOR_FILE_HASH, str);
    if (adaptor.encode(ADAPTOR_SHA256)->isWrong()) {
        ERROR("!adaptor.encode(ADAPTOR_SHA256)->isWrong()")
        EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
    }
    EXPECT_STREQ(right.c_str(), adaptor.getResult().c_str());
}
#endif
int main(int argc, char **argv) {
    int result = 0;
    log_init();
    testing::InitGoogleTest(&argc, argv);
    result = RUN_ALL_TESTS();
    return result;
}