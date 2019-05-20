//
// Created by tao on 19-1-17.
//
#include "common_includes.h"
//#include "hash_adaptor.h"
#include <boost/filesystem.hpp>
TEST(test_test, 1) {
    EXPECT_EQ(1, 1);
}



int main(int argc, char **argv) {
    namespace bf=boost::filesystem;
    bf::path file("/home/tao");
    if(bf::exists(file)){
        INFO("exists")
    } else{
        INFO("don't exist")
    }
    int result = 0;
    log_init();
    testing::InitGoogleTest(&argc, argv);
    result = RUN_ALL_TESTS();
    return result;
}