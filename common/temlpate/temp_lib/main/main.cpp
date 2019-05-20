//
// Created by tao on 19-1-17.
//
#include "common_includes.h"
#include "get_usr_passwd.h"

TEST(md5_string, hello) {
    EXPECT_STREQ("1", "1");
}

int main(int argc, char **argv) {
    int result = 0;
    log_init();
    testing::InitGoogleTest(&argc, argv);
    result = RUN_ALL_TESTS();
    INFO("now pwd:" << get_usr_passwd(0))
    return result;
}