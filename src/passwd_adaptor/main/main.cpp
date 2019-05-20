//
// Created by tao on 19-1-17.
//
#include "common_includes.h"
#include "get_usr_passwd.h"
#include "file_key_controller.h"
#include "common_funcs.h"

TEST(md5_string, hello) {
    EXPECT_STREQ("1", "1");
}

void test_file_key() {
    FileKeyTable *fileKeyTable = FileKeyTable::Instance;
    for (int i = 0; i < 10; ++i) {
        FileKeyNode fileKeyNode;
        fileKeyNode.id = i;
        fileKeyNode.key = "sdfajshfasukldfhjlasdjfhklasdfjklasdfjladfjasl;df";
        fileKeyTable->insertKey(fileKeyNode);
    }
    if (fileKeyTable->savrKeyTab()) {
        ERROR_LOG("save faild")
    }
}

void test_file_key_dec() {
    FileKeyTable *fileKeyTable = FileKeyTable::Instance;
    if (fileKeyTable->loadKeyTab()) {
        ERROR_LOG("loadKeyTab faild")
        return;
    }
    DEBUG("final is:\n" << fileKeyTable->toYamlString())
}

void test_user_key() {
    UserKeyTable *pUserKeyTable = UserKeyTable::Instance;
    for (int i = 0; i < 10; ++i) {
        UserKeyNode node;
        char name[128];
        snprintf(name, sizeof(name),"name:%d",i);
        node.userName = name;
        node.userId = i;
        for (auto &n:node.tableIndex) {
            n[0] = rand()%30;
            n[1] = rand()%500;
        }
        for (auto &n:node.sortHave) {
            n = randChar();
        }
        node.userPasswdHash = "sdfajshfasukldfhjlasdjfhklasdfjklasdfjladfjasl;df";
        node.mask = rand();
        pUserKeyTable->insertKey(node);
    }
    if (pUserKeyTable->savrKeyTab()) {
        ERROR_LOG("save faild")
    }
}

void test_user_key_dec() {
    UserKeyTable *pUserKeyTable = UserKeyTable::Instance;
    if (pUserKeyTable->loadKeyTab()) {
        ERROR_LOG("loadKeyTab faild")
        return;
    }
    DEBUG("final is:\n" << pUserKeyTable->toYamlString())
}

int main(int argc, char **argv) {
//    int result = 0;
//    log_init();
//    testing::InitGoogleTest(&argc, argv);
//    result = RUN_ALL_TESTS();
//    INFO("now pwd:" << get_usr_key(0))
//    return result;

    FileKeyTable::Instance->userID=1;
    test_user_key();
    test_user_key_dec();

    test_file_key();
    test_file_key_dec();


}